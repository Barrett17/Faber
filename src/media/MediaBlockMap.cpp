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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StorageUtils.h"


MediaBlockMap::MediaBlockMap()
	:
	BArchivable()
{
	fWriter = new MediaBlockMapWriter(this);
	fReader = new MediaBlockMapReader(this);
	fMap = BObjectList<MediaBlock>(true);
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
	fMap.MakeEmpty();
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


off_t
MediaBlockMap::Size() const
{
	off_t ret = 0;
	for (int32 i = 0; i < CountBlocks(); i++) {
		MediaBlock* block = BlockAt(i);
		ret += block->Size();
	}

	return ret;
}


size_t
MediaBlockMap::ReservedSize() const
{
	return CountBlocks()*MEDIA_BLOCK_RESERVED_SIZE;
}


size_t
MediaBlockMap::PreviewSize() const
{
	size_t size = 0;
	for (int32 i = 0; i < CountBlocks(); i++) {
		MediaBlock* block = BlockAt(i);
		size += block->PreviewSize();
	}

	return size;
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
MediaBlockMap::LastBlock() const
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


int64
MediaBlockMapVisitor::SeekToFrame(int64 frame)
{
	int64 currentFrame = 0;
	for (int32 i = 0; i < fMap->CountBlocks(); i++) {
		MediaBlock* currentBlock = fMap->BlockAt(i);
		int64 nextFrames = currentFrame+currentBlock->CountFrames();

		if (frame >= currentFrame && frame < nextFrames+1) {
			fCurrentIndex = i;
			currentBlock->SeekToFrame(frame-currentFrame);
			fCurrentFrame = frame;
			return frame;
		}
		currentFrame = nextFrames;
	}

	return currentFrame;
}


int64
MediaBlockMapVisitor::CurrentFrame() const
{
	return fCurrentFrame;	
}


MediaBlock*
MediaBlockMapVisitor::CurrentBlock() const
{
	return fMap->BlockAt(fCurrentIndex);
}


int64
MediaBlockMapWriter::WriteFrames(void* buffer, int64 frameCount)
{
	MediaBlock* block = CurrentBlock();

	int64 remaining = frameCount;
	int64 totalWrite = 0;

	while (remaining != 0) {

 		if (block == NULL || block->AvailableFrames() == 0) {
 			if (fMap->CountBlocks()-1 > fCurrentIndex) {
 				block = fMap->BlockAt(fCurrentIndex+1);
 				fCurrentIndex += 1;
 			} else {
				BEntry* destEntry = StorageUtils::BlockFileRequested();
				BFile* destFile = new BFile(destEntry, B_READ_WRITE | B_CREATE_FILE);

				status_t ret = destFile->InitCheck();

				if (ret != B_OK) {
					printf("%s\n", strerror(ret));
					return -1;
				}

				block = new MediaBlock(destFile, destEntry);
				block->SeekToFrame(0);

				fMap->AddBlock(block);
				fCurrentIndex = fMap->CountBlocks()-1;
 			}
		}

		int64 writeSize = 0;
		int64 freeSize = block->AvailableFrames();

		if (freeSize < remaining) {
			writeSize = freeSize;
			remaining -= freeSize;
		} else {
			writeSize = remaining;
			remaining -= writeSize;
		}

		totalWrite += block->WriteFrames(buffer, writeSize);
		buffer += writeSize;
	}

	fCurrentFrame += totalWrite;

	return totalWrite;
}


int64
MediaBlockMapWriter::WriteFramesAt(void* buffer, int64 start, int64 frameCount)
{
	if (start < 0)
		return 0;

	SeekToFrame(start);
	return WriteFrames(buffer, frameCount);
}


void
MediaBlockMapWriter::Flush()
{
	for (int32 i = 0; i < fMap->CountBlocks(); i++) {
		MediaBlock* block = fMap->BlockAt(i);

		if (block->WasFlushed())
			continue;

		block->Flush();
	}
}


int64
MediaBlockMapReader::ReadFrames(void* buffer, int64 frameCount)
{
	MediaBlock* block = CurrentBlock();

	int64 remaining = frameCount;
	int64 readSize = 0;
	int64 totalRead = 0;

	while (remaining != 0) {

 		if (block == NULL)
 			return -1;

		if (fMap->CountBlocks()-1 > fCurrentIndex) {
 			block = fMap->BlockAt(fCurrentIndex+1);
 			fCurrentIndex += 1;
		} else {
			return -1;
		}

		int64 freeSize = block->AvailableFrames();

		if (freeSize < remaining) {
			readSize = freeSize;
			remaining -= freeSize;
		} else {
			readSize = remaining;
			remaining -= readSize;
		}

		totalRead += block->ReadFrames(buffer, readSize);
		buffer += readSize;
	}
	fCurrentFrame += totalRead;
	return totalRead;
}


int64
MediaBlockMapReader::ReadFramesAt(void* buffer, int64 start, int64 frameCount)
{
	if (start < 0)
		return 0;

	SeekToFrame(start);
	return ReadFrames(buffer, frameCount);
}


size_t
MediaBlockMapReader::ReadPreview(float** ret)
{
	MediaBlock* block = NULL;

	size_t size = fMap->PreviewSize();
	float* preview = (float*)malloc(size);

	size_t retIndex = 0;

	float* blockPreview = NULL;

	for (int32 i = 0; i < fMap->CountBlocks(); i++) {

		block = fMap->BlockAt(i);

		if (block == NULL || block->MediaDataSize() == 0)
			break;

		block->ReadPreview(&blockPreview);

		for (size_t j = 0; j < block->PreviewSize()/sizeof(float); j++) {
			preview[retIndex] = blockPreview[j];
			retIndex++;
		}
	}

	*ret = preview;

	return size;
}
