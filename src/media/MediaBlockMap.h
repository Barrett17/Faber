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
protected:
									MediaBlockMapVisitor(
										MediaBlockMap* blockMap,
										off_t position)
										:
										fMap(blockMap),
										fPosition(position)
										{}

			int64					CountFrames() const;
			status_t				SeekToFrame(int64* frame);
			int64					CurrentFrame() const;

			friend class			MediaBlockMap;

			MediaBlockMap*			fMap;

protected:
			// This will find the block containing the specified frame
			status_t				FindNearestBlock(int64 start,
										MediaBlock* block,
										int32* index);

			off_t					fPosition;
};


class MediaBlockMapWriter : public MediaBlockMapVisitor {
public:
									MediaBlockMapWriter(
										MediaBlockMap* blockMap)
										:
										MediaBlockMapVisitor(blockMap,
											MEDIA_BLOCK_RAW_DATA_START)
										{}

			void					WriteFrames(void* buffer, int64 frameCount,
										int64 from = -1,
										bool overWrite = false);

private:
			int16*					_RenderPreview(void* buffer,
										int64 frameCount, ssize_t* size);

			int16					fFramesAverage;
};


class MediaBlockMapReader : public MediaBlockMapVisitor {
public:
									MediaBlockMapReader(
										MediaBlockMap* blockMap)
										:
										MediaBlockMapVisitor(blockMap,
											MEDIA_BLOCK_RAW_DATA_START)
										{}

			void					ReadFrames(void* buffer, int64 frameCount);
};


class PreviewReader : public MediaBlockMapVisitor {
public:
									PreviewReader(
										MediaBlockMap* blockMap)
										:
										MediaBlockMapVisitor(blockMap,
											MEDIA_BLOCK_PREVIEW_START)
										{}

			int16*					ReadPreview(size_t* size,
										int64 start, int64 frameCount);
};


class MediaBlockMap : public BArchivable {
public:
									MediaBlockMap(BMessage* from);
									MediaBlockMap();
	virtual 						~MediaBlockMap();

	virtual	status_t 				Archive(BMessage* into,
										bool deep = true) const;

			static BArchivable*		Instantiate(BMessage* archive);

			int32					CountBlocks() const;

			MediaBlock*				BlockAt(int32 index);
			MediaBlock*				LastBlock();

			status_t				AddBlock(MediaBlock* block, int32 index);
			status_t				AddBlock(MediaBlock* block);

			MediaBlock*				RemoveBlockAt(int32 index);
			bool					RemoveBlock(MediaBlock* index);

			MediaBlockMapWriter*	Writer() const;
			MediaBlockMapReader*	Reader() const;
			PreviewReader*			Preview() const;
private:
			// TODO will be replaced by an AVL Tree.
			BObjectList<MediaBlock>	fMap;

			MediaBlockMapWriter*	fWriter;
			MediaBlockMapReader*	fReader;
			PreviewReader*			fPreview;
};

#endif
