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
#ifndef _MEDIA_BLOCK_MAP_H
#define _MEDIA_BLOCK_MAP_H


#include <Archivable.h>
#include <ObjectList.h>

#include "MediaBlock.h"


class MediaBlockMap;

class MediaBlockMapVisitor {
public:
			int64					SeekToFrame(int64 toFrame);
			int64					CurrentFrame() const;
			MediaBlock*				CurrentBlock() const;

			MediaBlock*				NextBlock();

protected:
			friend class			MediaBlockMap;

									MediaBlockMapVisitor(
										MediaBlockMap* blockMap)
										:
										fMap(blockMap),
										fCurrentBlockStart(0),
										fCurrentIndex(0)
										{}

			MediaBlockMap*			fMap;

			int64					fCurrentBlockStart;
			int32					fCurrentIndex;
};


class MediaBlockMapWriter : public MediaBlockMapVisitor {
public:
			int64					WriteFrames(void* buffer,
										int64 frameCount);

			int64					WriteFramesAt(void* buffer,
										int64 start,
										int64 frameCount);

			// Will request a new block and add it
			// to the end.
			MediaBlock*				RequestBlock();

			// This is used to get sure the audio data
			// and preview will get written.and calculated
			void					Flush();

protected:
			friend class			MediaBlockMap;

									MediaBlockMapWriter(
										MediaBlockMap* blockMap)
										:
										MediaBlockMapVisitor(blockMap)
										{}
};


class MediaBlockMapReader : public MediaBlockMapVisitor {
public:
			int64					ReadFrames(void* buffer, int64 frameCount);

			int64					ReadFramesAt(void* buffer,
										int64 start, int64 frameCount);

			int64					ReadPreview(void** buf,
										int64 frameCount = -1,
										int64 start = 0);

protected:
			friend class			MediaBlockMap;

									MediaBlockMapReader(
										MediaBlockMap* blockMap)
										:
										MediaBlockMapVisitor(blockMap)
										{}
};


class MediaBlockMap : public BArchivable {
public:
									MediaBlockMap(BMessage* from);
									MediaBlockMap();
	virtual 						~MediaBlockMap();

	virtual	status_t 				Archive(BMessage* into,
										bool deep = true) const;

			static BArchivable*		Instantiate(BMessage* archive);

			// Make the Map ready to store at least n frames
			status_t				EnsureCapacity(int64 frames);

			// Resize the Map to frames removing the exceding data
			// depending on the fromEnd value.
			status_t				ResizeCapacity(int64 frames,
										bool fromEnd = true);

			int64					CountFrames() const;
			int64					BlockPlacement(int32 index) const;
			int32					BlockForFrame(int64 frame,
										int64* startFrame) const;

			int64					PreviewFrames() const;

			int32					CountBlocks() const;
			MediaBlock*				BlockAt(int32 index) const;
			MediaBlock*				LastBlock() const;

			status_t				AddBlock(MediaBlock* block, int32 index);
			status_t				AddBlock(MediaBlock* block);

			MediaBlock*				RemoveBlockAt(int32 index);
			bool					RemoveBlock(MediaBlock* index);

			MediaBlockMapWriter*	Writer() const;
			MediaBlockMapReader*	Reader() const;

private:
			// TODO will be replaced by an AVL Tree + sentinels.
			BObjectList<MediaBlock>	fMap;

			MediaBlockMapWriter*	fWriter;
			MediaBlockMapReader*	fReader;
};

#endif
