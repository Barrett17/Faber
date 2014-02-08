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


class DataBlock
{
	virtual ssize_t			Read(void* buffer, size_t numBytes);
	virtual ssize_t			Read(void* buffer, size_t numBytes);

	virtual ssize_t			ReadAt(off_t position, void* buffer,
								size_t numBytes);

	virtual off_t			Seek(off_t position, int32 mode);
	virtual off_t			Position(void) const;

private:
			BDataIO*		fData;
};


class MediaBlock : public DataBlock
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

private:
			int64			fSize;

			int64			fStart;
			int64			fEnd;

			BMediaFile*		fFile;
			BMediaTrack*	fTrack;
};


// This block is meant to represent empty audio data.
// Useful to save space.

class EmptyDataBlock : public DataBlock
{

};


class EmptyMediaBlock : public DataBlock
{

};

// Meant to contain additional infos, just an idea right now

class AttributeBlock : public DataBlock
{

};
