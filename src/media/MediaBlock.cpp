/*
    Copyright 2013 Dario Casalinuovo. All rights reserved.

    This file is part of Faber.

    Faber is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Faber is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Faber.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "MediaBlock.h"
#include "StorageUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


MediaBlock::MediaBlock(entry_ref& ref)
	:
	DataBlock(FABER_AUDIO_DATA),
	fEntry(ref),
	fFlushed(true),
	fBuffer(NULL),
	fPreviewCache(NULL)
{
	memset(&fMetaData, 0, sizeof(fMetaData));
	fBuffer = new float[MEDIA_BLOCK_MAX_FRAMES];
	fPreviewCache = new float[MEDIA_BLOCK_PREVIEW_MAX_FRAMES];
	memset(fBuffer, 0, MEDIA_BLOCK_MAX_FRAMES);
	memset(fBuffer, 0, MEDIA_BLOCK_PREVIEW_MAX_FRAMES);
}


MediaBlock::~MediaBlock()
{
	delete[] (float*)fBuffer;
	delete[] (float*)fPreviewCache;
}


int64
MediaBlock::WriteFrames(void* buf, int64 frames)
{
	void* data = Data() + fOffset;
	memcpy(data, buf, StorageUtils::FramesToSize(frames));
	SetFramesUsed(CountFrames() + frames);
	SetOffset(fOffset + frames);
}


int64
MediaBlock::ReadFrames(void* buf, int64 count)
{
	memcpy(buf, Data() + fOffset, count);
	SetOffset(fOffset + count);
}


void*
MediaBlock::Data() const
{
	return fBuffer;
}


int64
MediaBlock::Offset() const
{
	return fOffset;
}


void
MediaBlock::SetOffset(int64 offset)
{
	fOffset = offset;
}


int64
MediaBlock::CountFrames() const
{
	return fMetaData.frame_count;
}


bool
MediaBlock::IsFull() const
{
	if (AvailableFrames() > 0)
		return false;

	return true;
}


int64
MediaBlock::AvailableFrames() const
{
	return MEDIA_BLOCK_MAX_FRAMES-fMetaData.frame_count;	
}


void
MediaBlock::SetFramesUsed(int64 frames)
{
	fMetaData.frame_count = frames;	
}


status_t
MediaBlock::CopyTo(MediaBlock* block)
{
	return B_ERROR;
}


entry_ref
MediaBlock::GetEntry() const
{
	return fEntry;
}


void*
MediaBlock::Preview() const
{
	return fPreviewCache;
}


int64
MediaBlock::PreviewFrames() const
{
	return fMetaData.preview_frames;
}


void
MediaBlock::Open(uint32 openMode)
{
	fFile = new BFile(&fEntry, openMode | B_CREATE_FILE);
	fFile->Seek(0, SEEK_SET);
	OpenIOContext(fFile);
	//TODO possibly we could lock the dataBlock depending on openMode.
}


void
MediaBlock::Close()
{
	CloseIOContext();
	delete fFile;
	fFile = NULL;
}


void
MediaBlock::Load()
{
	Open(B_READ_ONLY);
	_LoadMetaData();
	_LoadPreview();
	_LoadData();
	Close();
}


void
MediaBlock::Flush()
{
	Open(B_WRITE_ONLY);
	_FlushMetaData();
	_FlushPreview();
	_FlushData();
	SetFlushed(true);
	Close();
}


bool
MediaBlock::WasFlushed() const
{
	return fFlushed;
}


void
MediaBlock::SetFlushed(bool flushed)
{
	fFlushed = flushed;
}


void
MediaBlock::_LoadData()
{
	ReadAt(MEDIA_BLOCK_RAW_DATA_START, fBuffer,
		StorageUtils::FramesToSize(fMetaData.frame_count));
}


void
MediaBlock::_FlushData()
{
	WriteAt(MEDIA_BLOCK_RAW_DATA_START, fBuffer,
		StorageUtils::FramesToSize(fMetaData.frame_count));
}


void
MediaBlock::_LoadPreview()
{
	ReadAt(MEDIA_BLOCK_PREVIEW_START, fPreviewCache,
		StorageUtils::FramesToSize(fMetaData.preview_frames));
}


void
MediaBlock::_FlushPreview()
{
	int64 frames = 0;
	int64 timer = 0;
	float detail = MEDIA_BLOCK_PREVIEW_DETAIL;
	int64 previewFrames = (fMetaData.frame_count/MEDIA_BLOCK_PREVIEW_DETAIL)*2;
	float* buf = (float*) fPreviewCache;
	memset(buf, 0, MEDIA_BLOCK_PREVIEW_MAX_FRAMES);

	float* dataBuf = (float*)fBuffer;
	for (int64 i = 0; i < fMetaData.frame_count; i++) {
		float value = dataBuf[i];

		if (value > buf[frames])
			buf[frames] += value;

		if (value < buf[frames+1])
			buf[frames+1] += value;

		timer++;

		if (timer == detail) {
			buf[frames] = buf[frames] / detail;
			buf[frames+1] = buf[frames+1] / detail;

			buf[frames] *= 30.0f;
			buf[frames+1] *= 30.0f;

			timer = 0;
			frames += 2;
		}
	}
	WriteAt(MEDIA_BLOCK_PREVIEW_START, (void*) buf,
		StorageUtils::FramesToSize(previewFrames));
	fMetaData.preview_frames = previewFrames;
}


void
MediaBlock::_LoadMetaData()
{
	Seek(0, SEEK_SET);
	Read(&fMetaData, sizeof(fMetaData));
}


void
MediaBlock::_FlushMetaData()
{
	Seek(0, SEEK_SET);
	Write(&fMetaData, sizeof(fMetaData));
}
