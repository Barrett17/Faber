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


MediaBlock::MediaBlock(BFile* file, BEntry* entry)
	:
	DataBlock(file, FABER_AUDIO_DATA),
	fData(file),
	fEntry(entry)
{
}


MediaBlock::~MediaBlock()
{
	delete fData;
}


int64
MediaBlock::CountFrames() const
{
	off_t size;
	fData->GetSize(&size);

	return StorageUtils::SizeToFrames(size);
}


status_t
MediaBlock::SeekToFrame(int64* frame)
{
	return B_ERROR;
}


MediaBlock*
MediaBlock::CopyTo(BFile* file)
{
	// return StorageUtils::CopyBlockInto(this, file);
	return NULL;
}


const BEntry&
MediaBlock::GetEntry() const
{
	return *fEntry;
}


BFile*
MediaBlock::GetFile() const
{
	return fData;
}


off_t
MediaBlock::GetSize()
{
	off_t size;
	fData->GetSize(&size);

	return size;
}
