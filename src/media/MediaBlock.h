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

class TrackIndex;


enum FaberDataBlockKinds {
	FABER_RAW_DATA = 1001,
	FABER_MEDIA_DATA,
	FABER_MIDI_DATA
};


struct block_ref {
	int32 id;
	uint32 kind;
	entry_ref entry;

private:
	block_ref* next;
};


struct mediablock_ref : public block_ref {
	int64 frames;
	int64 start;

private:
	mediablock_ref* next;
};


class DataBlock {
public:
							DataBlock(TrackIndex* owner,
								BDataIO* data);	

		const block_ref*	GetRef();

	virtual	uint32			Kind() const = 0;

	/*
	virtual ssize_t			Read(void* buffer, size_t numBytes);
	virtual ssize_t			Read(void* buffer, size_t numBytes);

	virtual ssize_t			ReadAt(off_t position, void* buffer,
								size_t numBytes);

	virtual off_t			Seek(off_t position, int32 mode);
	virtual off_t			Position(void) const;*/

private:
			BDataIO*		fData;
			block_ref*		fRef;
};


class MediaBlock : public DataBlock {
public:
									MediaBlock(TrackIndex* owner,
										BMediaFile* file);	

	virtual	uint32					Kind() const;

	virtual const mediablock_ref*	GetRef();

			status_t				ReadFrames(void* buffer, int64* frameCount);
			int64					CountFrames() const;

			status_t				SeekToFrame(int64* _frame, int32 flags = 0);

private:

			int16*					fPreview;
			size_t					fPreviewSize;

			mediablock_ref*			fRef;
			BMediaFile*				fFile;
			BMediaTrack*			fTrack;
};


class MediaBlockMap {
public:
		int32						CountBlocks() const;

		const block_ref*			BlockAt(int32 index);

		status_t					AddBlock(int32 index, MediaBlock* block,
										bool saveAndFree = false);
		status_t					AddBlock(MediaBlock* block,
										bool saveAndFree = false);
		status_t					AddBlock(int32 index, block_ref* block);

		status_t					RemoveBlock(int32 index);
		status_t					RemoveBlocks(int32 start, int32 end);

		//PreviewIterator*			GetPreview();
		//MediaBlockIterator*		GetBlocks();
private:
		mediablock_ref*				fStart;
		mediablock_ref*				fEnd;
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
