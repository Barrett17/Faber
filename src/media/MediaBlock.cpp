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
#include "StorageManager.h"


MediaBlock::MediaBlock(BFile* file)
	:
	DataBlock(file, FABER_AUDIO_DATA),
	fFramesCount(0)
{
}


int16*
MediaBlock::ReadPreview(size_t* size)
{
	return NULL;
}


status_t
MediaBlock::ReadFrames(void* buffer, int64* frameCount)
{
	return B_ERROR;
}


int64
MediaBlock::CountFrames() const
{
	return fFramesCount;
}


status_t
MediaBlock::SeekToFrame(int64* frame)
{
	return B_ERROR;
}


MediaBlock*
MediaBlock::CopyTo(BFile* file)
{
	// return StorageManager::CopyBlockInto(this, file);
	return NULL;
}


int32
MediaBlockMap::CountBlocks() const
{
	return fMap.CountItems();
}


MediaBlock*
MediaBlockMap::BlockAt(int32 index)
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


MediaBlockMapWriter::MediaBlockMapWriter()
	:
	fMap(NULL)
{
}


void
MediaBlockMapWriter::SetTo(MediaBlockMap* to) 
{
	fMap = to;
}


MediaBlockMap*
MediaBlockMapWriter::Current() const
{
	return fMap;
}


status_t
MediaBlockMapWriter::WriteFrames(void* buffer, int64 frameCount,
	int64 start, bool overWrite)
{
	MediaBlock* block = NULL;

	if (start == -1)
		block = fMap->LastBlock();

	BFile* destFile = NULL;

	if (block == NULL)
		destFile = StorageManager::BlockFileRequested();
	else
		destFile = block->GetFile();

	if (block == NULL)
		fMap->AddBlock(new MediaBlock(destFile));
}
