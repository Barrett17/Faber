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

#include <MediaDefs.h>

#include "MediaFormatBuilder.h"
#include "StorageUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


MediaBlock::MediaBlock(entry_ref& ref)
	:
	DataBlock(FABER_AUDIO_DATA),
	fEntry(ref),
	fFile(NULL),
	fFlushed(true),
	fDataIO(NULL),
	fPreviewIO(NULL)
{
	memset(&fMetaData, 0, sizeof(fMetaData));

	fData = new float[MEDIA_BLOCK_MAX_FRAMES];
	fPreview = new float[MEDIA_BLOCK_PREVIEW_MAX_FRAMES];

	fDataIO = new BMemoryIO(fData, MEDIA_BLOCK_MAX_SIZE);
	fPreviewIO = new BMemoryIO(fPreview, MEDIA_BLOCK_PREVIEW_MAX_SIZE);
}


MediaBlock::~MediaBlock()
{
	delete fDataIO;
	delete fPreviewIO;

	delete[] fData;
	delete[] fPreview;
}


int64
MediaBlock::WriteFrames(void* buf, int64 frames)
{
	if (frames+CurrentFrame() > MEDIA_BLOCK_MAX_FRAMES) {
		printf("ERROR mediablock overflow!!!");
		return -1;
	}

	size_t size = fDataIO->Write(buf, StorageUtils::FramesToSize(frames));

	if (fMetaData.frame_count <= frames+CurrentFrame())
		fMetaData.frame_count += frames;

	return StorageUtils::SizeToFrames(size);
}


int64
MediaBlock::ReadFrames(void* buf, int64 frames)
{
	if (frames+CurrentFrame() > MEDIA_BLOCK_MAX_FRAMES) {
		printf("ERROR mediablock overflow!!!");
		return -1;
	}

	size_t ret = fDataIO->Read(buf, StorageUtils::FramesToSize(frames));
	return StorageUtils::SizeToFrames(ret);
}


int64
MediaBlock::CountFrames() const
{
	return fMetaData.frame_count;
}


int64
MediaBlock::CurrentFrame() const
{
	return StorageUtils::SizeToFrames(fDataIO->Position());
}


status_t
MediaBlock::SeekToFrame(int64 frame)
{
	if (frame != 0 && frame > fMetaData.frame_count)
		return B_ERROR;

	return fDataIO->Seek(StorageUtils::FramesToSize(frame), SEEK_SET);
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


int64
MediaBlock::ReadPreview(void* buf, int64 frames, int64 start)
{
	fPreviewIO->Seek(StorageUtils::FramesToSize(start), SEEK_SET);

	size_t ret = fPreviewIO->Read(buf,
		StorageUtils::FramesToSize(frames));

	fPreviewIO->Seek(0, SEEK_SET);
	return StorageUtils::SizeToFrames(ret);
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
	// TODO: flushing means that we don't want to write
	// or read anymore. It should unload resources used by the
	// raw data. But should allow to reload them when some client,
	// such as the render want to read it directly.
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
	ReadAt(MEDIA_BLOCK_RAW_DATA_START, fData,
		StorageUtils::FramesToSize(fMetaData.frame_count));
}


void
MediaBlock::_FlushData()
{
	WriteAt(MEDIA_BLOCK_RAW_DATA_START, fData,
		StorageUtils::FramesToSize(fMetaData.frame_count));
}


void
MediaBlock::_LoadPreview()
{
	ReadAt(MEDIA_BLOCK_PREVIEW_START, fPreview,
		StorageUtils::FramesToSize(fMetaData.preview_frames));
}


void
MediaBlock::_FlushPreview()
{
	int64 frames = 0;
	int64 timer = 0;
	float detail = MEDIA_BLOCK_PREVIEW_DETAIL;
	int64 previewFrames = (fMetaData.frame_count/MEDIA_BLOCK_PREVIEW_DETAIL)*2;
	float* buf = (float*) fPreview;
	memset(buf, 0, MEDIA_BLOCK_PREVIEW_MAX_SIZE);

	SeekToFrame(0);
	float* dataBuf = (float*)fData;

	if (fMetaData.frame_count > MEDIA_BLOCK_MAX_FRAMES)
		printf("ehm %" B_PRId64 "\n", fMetaData.frame_count);

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
		StorageUtils::FramesToSize(frames));
	fMetaData.preview_frames = frames;
}


void
MediaBlock::_LoadMetaData()
{
	Read(&fMetaData, sizeof(fMetaData));
}


void
MediaBlock::_FlushMetaData()
{
	Write(&fMetaData, sizeof(fMetaData));
}
