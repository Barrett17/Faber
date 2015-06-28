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


int64
MediaBlockMap::BlockPlacement(int32 index) const
{
	if (index >= CountBlocks())
		return -1;

	int64 ret = 0;
	for (int32 i = 0; i < index; i++) {
		MediaBlock* block = BlockAt(i);
		ret += block->CountFrames();
	}

	return ret;
}


int32
MediaBlockMap::BlockForFrame(int64 frame, int64* startFrame) const
{
	int64 count = 0;
	for (int32 i = 0; i < CountBlocks(); i++) {
		MediaBlock* block = BlockAt(i);
		int64 nextCount = count+block->CountFrames();
		if (frame >= count && frame < nextCount) {
			*startFrame = count;
			return i;
		}
		count = nextCount;
	}

	return -1;
}


int64
MediaBlockMap::PreviewFrames() const
{
	size_t frames = 0;
	for (int32 i = 0; i < CountBlocks(); i++) {
		MediaBlock* block = BlockAt(i);
		frames += block->PreviewFrames();
	}

	return frames;
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
	if (frame < 0 || fMap->CountBlocks() == 0)
		return -1;

	if (frame == 0) {
		fCurrentIndex = 0;
		fCurrentBlockStart = 0;
		fMap->BlockAt(fCurrentIndex)->SeekToFrame(0);
		return 0;
	}

	// TODO review the following
	int64 blockStart = 0;
	int32 blockIndex = fMap->BlockForFrame(frame, &blockStart);
	if (blockIndex < 0)
		return -1;

	MediaBlock* currentBlock = fMap->BlockAt(blockIndex);
	currentBlock->SeekToFrame(frame-blockStart);
	fCurrentBlockStart = blockStart;
	return frame;
}


int64
MediaBlockMapVisitor::CurrentFrame() const
{
	return fCurrentBlockStart+fMap->BlockAt(fCurrentIndex)->CurrentFrame();
}


MediaBlock*
MediaBlockMapVisitor::CurrentBlock() const
{
	return fMap->BlockAt(fCurrentIndex);
}


MediaBlock*
MediaBlockMapVisitor::NextBlock()
{
	printf("nextblock\n");
	MediaBlock* block = fMap->BlockAt(fCurrentIndex+1);
	if (block != NULL) {
		block->SeekToFrame(0);
		fCurrentIndex++;
		fCurrentBlockStart = fMap->BlockPlacement(fCurrentIndex);		
	}
	return block;
}


MediaBlock*
MediaBlockMapWriter::RequestBlock()
{
	printf("request new block\n");
	entry_ref entry;
	if (StorageUtils::BlockEntryRequested(&entry) != B_OK)
		return NULL;
	MediaBlock* block = new MediaBlock(entry);
	fMap->AddBlock(block);
	block->SeekToFrame(0);
	fCurrentIndex = fMap->CountBlocks()-1;
	fCurrentBlockStart = fMap->BlockPlacement(fCurrentIndex);
	return block;
}


int64
MediaBlockMapWriter::WriteFrames(void* buffer, int64 frameCount)
{
	MediaBlock* block = CurrentBlock();

	int64 remaining = frameCount;
	int64 totalWrite = 0;
	int64 offset = 0;
	while (remaining > 0) {

 		if (block == NULL || block->IsFull()) {
 			block = NextBlock();

			if (block == NULL)
				block = RequestBlock();
		}

		printf("Got a block %d available space from block %"
			B_PRId64 " we want to write %" B_PRId64 "\n",
			fCurrentIndex, block->AvailableFrames(),
			remaining);

		int64 writeSize = 0;
		int64 freeSize = block->AvailableFrames();

		if (freeSize < remaining) {
			writeSize = freeSize;
			remaining -= freeSize;
		} else {
			writeSize = remaining;
			remaining -= writeSize;
		}
 
 		totalWrite += block->WriteFrames(buffer+offset, writeSize);
		offset += writeSize;
		block->SetFlushed(false);

 		printf("Data written %" B_PRId64 "available %"
 			B_PRId64 "\n", writeSize, block->AvailableFrames());
	}
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
		printf("flushing %d\n", i);
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
		if (block == NULL || fMap->CountBlocks()-1 > fCurrentIndex)
 			block = NextBlock();
		else
			return -1;

		int64 toRead = block->CountFrames();
		if (toRead < remaining) {
			readSize = toRead;
			remaining -= toRead;
		} else {
			readSize = remaining;
			remaining -= readSize;
		}
		totalRead += block->ReadFrames(buffer, readSize);
		buffer += readSize;
	}
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


int64
MediaBlockMapReader::ReadPreview(void** buf, int64 frameCount, int64 start)
{
	// TODO allow to read only a part of the preview.
	int64 totalFrames = fMap->PreviewFrames();
	//printf("%" B_PRId64 "\n", totalFrames);
	float* preview = new float[totalFrames];
	memset(preview, 0, StorageUtils::FramesToSize(totalFrames));
	SeekToFrame(0);
	int32 i = 0;

	int64 offset = 0;
	for (int32 i = 0; i < fMap->CountBlocks(); i++) {
		MediaBlock* nextBlock = fMap->BlockAt(i);
		int64 blockFrames = nextBlock->PreviewFrames();
		offset += nextBlock->ReadPreview(preview+offset, blockFrames);
	}

	*buf = preview;

	return offset;
}
