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


MediaBlock::MediaBlock(BFile* file, BEntry* entry)
	:
	DataBlock(file, FABER_AUDIO_DATA),
	fData(file),
	fEntry(entry),
	fFlushed(true),
	fPreviewCache(NULL),
	fPreviewSize(0),
	fCurrentRawSize(0)
{
	Seek(0, SEEK_SET);
}


MediaBlock::~MediaBlock()
{
	delete fData;
	free(fPreviewCache);
}


int64
MediaBlock::CountFrames() const
{
	return StorageUtils::SizeToFrames(MediaDataSize());
}

int64
MediaBlock::CurrentFrame() const
{
	return StorageUtils::SizeToFrames(fCurrentRawSize);
}


int64
MediaBlock::AvailableFrames() const
{
	return StorageUtils::SizeToFrames(MEDIA_BLOCK_RAW_SIZE)-CurrentFrame();	
}



int64
MediaBlock::SeekToFrame(int64 frame)
{
	off_t ret = Seek(StorageUtils::FramesToSize(frame)
		+MEDIA_BLOCK_RAW_DATA_START, SEEK_SET);

	return StorageUtils::SizeToFrames(ret-ReservedSize()-PreviewSize());
}


int64
MediaBlock::ReadFrames(void* buffer, int64 frameCount)
{
	size_t ret = Read(buffer, StorageUtils::FramesToSize(frameCount));
	fCurrentRawSize += ret;

	return StorageUtils::SizeToFrames(ret);
}


int64
MediaBlock::WriteFrames(void* buffer, int64 frameCount)
{
	size_t ret = Write(buffer, StorageUtils::FramesToSize(frameCount));
	fCurrentRawSize += ret;
	SetFlushed(false);

	return StorageUtils::SizeToFrames(ret);
}


MediaBlock*
MediaBlock::CopyTo(BFile* file)
{
	return NULL;
}


const BEntry&
MediaBlock::GetEntry() const
{
	return *fEntry;
}


off_t
MediaBlock::Size() const
{
	off_t size;
	GetSize(&size);
	return size;
}


size_t
MediaBlock::ReservedSize() const
{
	return MEDIA_BLOCK_RESERVED_SIZE;
}


size_t
MediaBlock::PreviewSize() const
{
	return fPreviewSize;
}


size_t
MediaBlock::MediaDataSize() const
{
	return fCurrentRawSize;
}


void
MediaBlock::Flush()
{
	int64 frames = 0;
	int64 timer = 0;
	int64 detail = MEDIA_BLOCK_PREVIEW_DETAIL;

	size_t size = MediaDataSize();

	float* from = (float*) malloc(size);

	memset(from, 0, size);
	ReadAt(MEDIA_BLOCK_RAW_DATA_START, from, size);

	size_t previewSize = MEDIA_BLOCK_PREVIEW_SIZE;
	float* buf = new float[previewSize];

	memset(buf, 0, previewSize);

	for (size_t i = 0; i < size/sizeof(float); i++) {
		float value = from[i];

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

	free(from);

	WriteAt(MEDIA_BLOCK_PREVIEW_START, (void*) buf, previewSize);
	SetFlushed(true);

	if (fPreviewCache != NULL)
		delete[] fPreviewCache;

	fPreviewCache = buf;
	fPreviewSize = previewSize;
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


size_t
MediaBlock::ReadPreview(float** preview)
{
	if (!WasFlushed())
		printf("Warning block was not flushed.");

	if (fPreviewCache == NULL) {
		float* buf = (float*)malloc(PreviewSize());

		ReadAt(MEDIA_BLOCK_PREVIEW_START, buf, PreviewSize());

		fPreviewCache = buf;
		*preview = buf;
	} else
		*preview = fPreviewCache;

	return PreviewSize();
}
