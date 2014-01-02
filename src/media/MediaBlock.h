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


class MediaBlock
{

			bool			IsEmpty() const;
			void			CleanUp();

			status_t		ReadFrames(void* buffer, int64* frameCount);
			int64			CountFrames() const;

			status_t		WriteFrames(const void* data, int32 frameCount,
								int32 flags = 0);

			status_t		ReplaceFrames(const void* buffer,
								int64* frameCount);

			status_t		SeekToTime(bigtime_t* _time, int32 flags = 0);
			status_t		SeekToFrame(int64* _frame, int32 flags = 0);

			status_t		DetachFromFile();

			status_t		DeleteFile();

			status_t		CopyFrom(MediaBlock* block);

private:
			int64			fSize;

			int64			fStart;
			int64			fEnd;

			void*			fBuffer;

			BMediaFile*		fFile;
			BMediaTrack*	fTrack;
};


class MediaFileBlock
{

};

// the same as MediaFileBlock but in RAM.

class VirtualBlock
{

};

// This block is meant to represent empty audio data.
// Useful to save space.

class EmptySpaceBlock : public MediaBlock
{

};

// Meant to contain additional infos, not sure if it's needed.

class AttributeBlock : public MediaBlock
{

};
