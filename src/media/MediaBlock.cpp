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


bool
MediaBlock::IsEmpty() const
{
}


void
MediaBlock::MakeEmpty()
{
}


status_t
MediaBlock::ReadFrames(void* buffer, int64* frameCount)
{
}


int64
MediaBlock::CountFrames() const
{
}


status_t
MediaBlock::WriteFrames(const void* data, int32 frameCount, int32 flags)
{
}


status_t
MediaBlock::ReplaceFrames(const void* buffer, int64* frameCount)
{
}


status_t
MediaBlock::SeekToTime(bigtime_t* _time, int32 flags)
{
}


status_t
MediaBlock::SeekToFrame(int64* _frame, int32 flags)
{
}


void
MediaBlock::ReplaceWith(MediaBlock* block)
{
}


status_t
MediaBlock::DetachFromFile()
{
}


status_t
MediaBlock::DeleteFile()
{
}
