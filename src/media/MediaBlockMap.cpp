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

// This is 1 MiB actually
#define BLOCK_SIZE_LIMIT 1048576


MediaBlockMap::MediaBlockMap()
	:
	BArchivable()
{
	fWriter = new MediaBlockMapWriter(this);
}


MediaBlockMap::MediaBlockMap(BMessage* from)
	:
	BArchivable(from)
{

}


MediaBlockMap::~MediaBlockMap()
{
	delete fWriter;
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


MediaBlockMapWriter*
MediaBlockMap::Writer() const
{
	return fWriter;
}


int64
MediaBlockMapVisitor::CountFrames() const
{
	return 0;
}


status_t
MediaBlockMapVisitor::SeekToFrame(int64* frame)
{
	return B_ERROR;
}


void
MediaBlockMapWriter::WriteFrames(void* buffer, int64 frameCount,
	int64 start, bool overWrite)
{
	MediaBlock* block = NULL;

	if (start == -1)
		block = fMap->LastBlock();

	BFile* destFile = NULL;
	BEntry* destEntry = NULL;

	bool addBlock = false;

	if (fMap->CountBlocks() == 0 || block->GetSize() >= BLOCK_SIZE_LIMIT) {
		printf("Creating a new block\n");
		destEntry = StorageUtils::BlockFileRequested();
		destFile = new BFile(destEntry, B_READ_WRITE | B_CREATE_FILE);
		status_t ret = destFile->InitCheck();
		printf("%s\n", strerror(ret));
		addBlock = true;
	} else {
		printf("Get a block\n");
		destFile = block->GetFile();
	}

	destFile->Write(buffer, StorageUtils::FramesToSize(frameCount));

	if (addBlock)
		fMap->AddBlock(new MediaBlock(destFile, destEntry));
}


void
MediaBlockMapReader::ReadFrames(void* buffer, int64 frameCount)
{

}
