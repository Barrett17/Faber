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
	fCurrentSize(0)
{
	SetSize(MEDIA_BLOCK_DEFAULT_SIZE);
	Seek(0, SEEK_SET);
}


MediaBlock::~MediaBlock()
{
	printf("delete\n");
	delete fData;
	free(fPreviewCache);
}


int64
MediaBlock::CountFrames() const
{
	return StorageUtils::SizeToFrames(AudioDataSize());
}


status_t
MediaBlock::SeekToFrame(int64* frame)
{
	return B_ERROR;
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
	return MEDIA_BLOCK_PREVIEW_SIZE;
	//return fPreviewSize;
}


size_t
MediaBlock::AudioDataSize() const
{
	return fCurrentSize;
}


void
MediaBlock::Flush()
{
	int64 frames = 0;
	int64 timer = 0;
	int64 detail = MEDIA_BLOCK_PREVIEW_DETAIL;

	size_t size = AudioDataSize();

	float* from = (float*) malloc(size);

	memset(from, 0, size);
	ReadAt(MEDIA_BLOCK_RAW_DATA_START, from, size);

	size_t previewSize = PreviewSize();
	float* buf = new float[previewSize];

	memset(buf, 0, previewSize);

	for (size_t i = 0; i < size/sizeof(float); i++) {
		float value = from[i];

		/*if (buf[frames] > value || buf[frames] == 0)
			buf[frames] = value;

		if (buf[frames+1] < value || buf[frames+1] == 0)
			buf[frames+1] = value;*/

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

			//printf("test %f %f\n", buf[frames], buf[frames+1]);

			timer = 0;
			frames += 2;
		}
	}

	free(from);

	WriteAt(MEDIA_BLOCK_PREVIEW_START, (void*) buf, previewSize);
	SetFlushed(true);

	if (fPreviewCache == NULL)
		free(fPreviewCache);

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
	if (fPreviewCache == NULL) {

		if (!WasFlushed())
			printf("Warning was not flushed.");

		float* buf = (float*)malloc(PreviewSize());

		ReadAt(MEDIA_BLOCK_PREVIEW_START, buf, PreviewSize());

		fPreviewCache = buf;
		*preview = buf;
	} else
		*preview = fPreviewCache;

	return PreviewSize();
}


size_t
MediaBlock::FreeSpace() const
{
	return MEDIA_BLOCK_RAW_SIZE - fCurrentSize;
}


bool
MediaBlock::IsFull() const
{
	return FreeSpace() == 0;
}
