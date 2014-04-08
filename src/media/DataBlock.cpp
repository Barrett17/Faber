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


#include "DataBlock.h"


DataBlock::DataBlock(BPositionIO* data, FaberDataBlockKinds kind)
	:
	fData(data),
	fKind(kind)
{

}


uint32
DataBlock::Kind() const
{
	return fKind;
}


ssize_t
DataBlock::Read(void* buffer, size_t size)
{
	return fData->Read(buffer, size);
}


ssize_t
DataBlock::Write(const void* buffer, size_t size)
{
	return fData->Write(buffer, size);
}


ssize_t
DataBlock::ReadAt(off_t position, void* buffer, size_t size)
{
	return fData->ReadAt(position, buffer, size);
}


ssize_t
DataBlock::WriteAt(off_t position, const void* buffer, size_t size)
{
	return fData->WriteAt(position, buffer, size);
}


off_t
DataBlock::Seek(off_t position, uint32 seekMode)
{
	return fData->Seek(position, seekMode);
}


off_t
DataBlock::Position() const
{
	return fData->Position();
}


status_t
DataBlock::SetSize(off_t size)
{
	return fData->SetSize(size);
}


status_t
DataBlock::GetSize(off_t* size) const
{
	return fData->GetSize(size);
}
