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

#ifndef _MEDIA_BLOCK_H
#define _MEDIA_BLOCK_H

#include <Entry.h>

//#include "HashMap.h"

class TrackIndex;


enum FaberDataBlockKinds {
	FABER_RAW_DATA = 1001,
	FABER_AUDIO_DATA,
	FABER_MIDI_DATA
};


class DataBlock {
public:
									DataBlock(BDataIO* data,
										FaberDataBlockKinds kind);	

	virtual	uint32					Kind() const = 0;

	/*
	virtual ssize_t					Read(void* buffer, size_t numBytes);
	virtual ssize_t					Read(void* buffer, size_t numBytes);

	virtual ssize_t					ReadAt(off_t position, void* buffer,
										size_t numBytes);

	virtual off_t					Seek(off_t position, int32 mode);
	virtual off_t					Position(void) const;*/

private:
			BDataIO*				fData;
};


class MediaBlock : public DataBlock {
public:
									MediaBlock(BFile* file);	

	virtual	uint32					Kind() const;

			int16*					ReadPreview(size_t* size);

			status_t				ReadFrames(void* buffer, int64* frameCount);
			int64					CountFrames() const;

			status_t				SeekToFrame(int64* _frame, int32 flags = 0);

			MediaBlock*				CopyTo(BFile* file);

private:
			BFile*					fData;
};


class MediaBlockMap {
public:
		int32						CountBlocks() const;

		MediaBlock*					BlockAt(int32 index);

		status_t					AddBlock(int32 index, MediaBlock* block,
										bool saveAndFree = false);

		status_t					AddBlock(MediaBlock* block,
										bool saveAndFree = false);

		status_t					RemoveBlock(int32 index);
		status_t					RemoveBlocks(int32 start, int32 end);

protected:
		friend class				TrackIO;

		status_t					WriteFrames(void* buffer, int64 frameCount);

		//PreviewIterator*			GetPreview();
		//MediaBlockIterator*		GetBlocks();
private:
		//HashMap<MediaBlock, int64>			fMap;
};


// This block is meant to represent empty data.
// Useful to save space.
/*
class EmptyDataBlock : public DataBlock {

};


class EmptyMediaBlock : public MediaBlock {

};
*/

#endif
