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

#include "MediaBlockMap.h"
#include "StorageUtils.h"

#include <stdio.h>
#include <string.h>


MediaBlockMap::MediaBlockMap()
	:
	BArchivable()
{
	fWriter = new MediaBlockMapWriter(this);
	fReader = new MediaBlockMapReader(this);
	fPreview = new PreviewReader(this);
}


MediaBlockMap::MediaBlockMap(BMessage* from)
	:
	BArchivable(from)
{

}


MediaBlockMap::~MediaBlockMap()
{
	delete fWriter;
	delete fReader;
	delete fPreview;
}


status_t
MediaBlockMap::Archive(BMessage* into, bool deep) const
{
	BArchivable::Archive(into, deep);

	return B_OK;
}


BArchivable*
MediaBlockMap::Instantiate(BMessage* archive)
{
	if (!validate_instantiation(archive, "MediaBlockMap"))
		return NULL;

	return new MediaBlockMap(archive);      
}


// TODO various optimizations possible
int64
MediaBlockMap::CountFrames() const
{
	int64 ret = 0;
	for (int32 i = 0; i < CountBlocks(); i++) {
		MediaBlock* block = BlockAt(i);
		ret += block->CountFrames();
	}

	return ret;
}


int32
MediaBlockMap::CountBlocks() const
{
	return fMap.CountItems();
}


MediaBlock*
MediaBlockMap::BlockAt(int32 index) const
{
	return fMap.ItemAt(index);
}


MediaBlock*
MediaBlockMap::LastBlock()
{
	if (CountBlocks() == 0)
		return NULL;

	return BlockAt(CountBlocks()-1);
}


status_t
MediaBlockMap::AddBlock(MediaBlock* block, int32 index)
{
	return fMap.AddItem(block, index);
}


status_t
MediaBlockMap::AddBlock(MediaBlock* block)
{
	return fMap.AddItem(block);
}


MediaBlock*
MediaBlockMap::RemoveBlockAt(int32 index)
{
	return fMap.RemoveItemAt(index);
}


bool
MediaBlockMap::RemoveBlock(MediaBlock* block)
{
	return fMap.RemoveItem(block);
}


MediaBlockMapWriter*
MediaBlockMap::Writer() const
{
	return fWriter;
}


MediaBlockMapReader*
MediaBlockMap::Reader() const
{
	return fReader;
}


PreviewReader*
MediaBlockMap::Preview() const
{
	return fPreview;
}


status_t
MediaBlockMapVisitor::SeekToFrame(int64* frame)
{
	return B_ERROR;
}


// TODO optimize
status_t
MediaBlockMapVisitor::FindNearestBlock(int64 start,
	MediaBlock* block, int32* index)
{
	if (start < 0 || start > fMap->CountFrames())
		return B_ERROR;

	int64 frame = 0;
	for (int32 i = 0; i < fMap->CountBlocks(); i++) {
		MediaBlock* currentBlock = fMap->BlockAt(i);
		frame += currentBlock->CountFrames();
		if (start >= frame || start <= frame) {
			block = currentBlock;
			*index =  i;
			return B_OK;
		}
	}

	return B_ERROR;
}


void
MediaBlockMapWriter::WriteFrames(void* buffer, int64 frameCount,
	int64 start, bool overWrite)
{
	MediaBlock* block = NULL;

	off_t size = 0;
	if (start == -1 && fMap->CountBlocks() != 0) {
		block = fMap->LastBlock();
		block->GetSize(&size);
	} else if (start > -1) {
		//block = _FindBlockAt(start);
	}

	if (block == NULL || size >= MEDIA_BLOCK_SIZE_LIMIT) {
		printf("Creating a new block %lld \n", size);

		BEntry* destEntry = StorageUtils::BlockFileRequested();
		BFile* destFile = new BFile(destEntry, B_READ_WRITE | B_CREATE_FILE);

		status_t ret = destFile->InitCheck();

		if (ret != B_OK) {
			printf("%s\n", strerror(ret));
			return;
		}

		block = new MediaBlock(destFile, destEntry);

		fMap->AddBlock(block);
	}

	//block->WriteLock();

	if (start > -1)
		fPosition = start;

	if (block->Position() < MEDIA_BLOCK_RAW_DATA_START) {
		if (fPosition > MEDIA_BLOCK_RAW_DATA_START)
			block->Seek(fPosition+MEDIA_BLOCK_RAW_DATA_START, SEEK_SET);			
		else
			block->Seek(MEDIA_BLOCK_RAW_DATA_START, SEEK_SET);
	}

	block->Write(buffer, StorageUtils::FramesToSize(frameCount));

	ssize_t previewSize = 0;
	int16* preview = _RenderPreview(buffer, frameCount, &previewSize);

	block->WriteAt(MEDIA_BLOCK_PREVIEW_START+
		(fPosition/MEDIA_BLOCK_PREVIEW_DETAIL), (void*) preview, previewSize);

	delete[] preview;

	//block->WriteUnlock();
}


void
MediaBlockMapReader::ReadFrames(void* buffer, int64 frameCount)
{
	// block->ReadLock();

	// block->ReadUnlock();
}


int16*
MediaBlockMapWriter::_RenderPreview(void* buffer, int64 frameCount,
	ssize_t* size)
{
	float* from = (float*) buffer;

	int64 frames = 0;
	int64 timer = 0;
	int64 detail = MEDIA_BLOCK_PREVIEW_DETAIL;

	*size = (ssize_t)(frameCount/detail)*2;

	int16* buf = new int16[*size];

	for (int i = 0; i < frameCount; i++) {
		int16 value = (int16)from[i];

		if (buf[frames] < value || buf[frames] == 0)
			buf[frames] = value;

		if (buf[frames+1] > value || buf[frames+1] == 0)
			buf[frames+1] = value;

		timer++;

		if (timer == detail) {
			timer = 0;
			frames += 2;
		}
	}

	return buf;
}


int16*
PreviewReader::ReadPreview(size_t* size, int64 start, int64 frameCount)
{
	MediaBlock* block = NULL;
	int32 index;
	if (FindNearestBlock(start, block, &index) != B_OK)
		return NULL;

	int16* preview = NULL;

	block->ReadAt(MEDIA_BLOCK_PREVIEW_START, (void*) preview,
		MEDIA_BLOCK_PREVIEW_SIZE);

	return preview;
}
