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

protected:
			friend class			MediaBlockMap;

									MediaBlockMapVisitor(
										MediaBlockMap* blockMap,
										off_t position)
										:
										fMap(blockMap),
										fCurrentFrame(0),
										fCurrentIndex(0)
										{}

			MediaBlockMap*			fMap;

			int64					fCurrentFrame;
			int32					fCurrentIndex;
};


class MediaBlockMapWriter : public MediaBlockMapVisitor {
public:
			int64					WriteFrames(void* buffer,
										int64 frameCount);

			// Seek to start before to read.
			int64					WriteFramesAt(void* buffer,
										int64 start,
										int64 frameCount);

			// This is used to get sure the audio data
			// and preview will get written.and calculated
			void					Flush();

protected:
			friend class			MediaBlockMap;

									MediaBlockMapWriter(
										MediaBlockMap* blockMap)
										:
										MediaBlockMapVisitor(blockMap,
											MEDIA_BLOCK_RAW_DATA_START)
										{}
private:
			float					fFramesAverage;
};


class MediaBlockMapReader : public MediaBlockMapVisitor {
public:
			int64					ReadFrames(void* buffer, int64 frameCount);

			// Seek to start before to read.
			int64					ReadFramesAt(void* buffer,
										int64 start, int64 frameCount);

			//int64					ReadPreview(void** buf,
			//							int64 start, int64 frameCount);

			size_t					ReadPreview(float** ret);

protected:
			friend class			MediaBlockMap;

									MediaBlockMapReader(
										MediaBlockMap* blockMap)
										:
										MediaBlockMapVisitor(blockMap,
											MEDIA_BLOCK_RAW_DATA_START)
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

			int64					CountFrames() const;

			off_t					Size() const;

			size_t					ReservedSize() const;
			size_t					PreviewSize() const;

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
