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

#define BLOCK_SIZE_LIMIT 1024200


class MediaBlockMap : public BArchivable {
public:
									MediaBlockMap(BMessage* from);
									MediaBlockMap();
	virtual 						~MediaBlockMap();

	virtual	status_t 				Archive(BMessage* into,
										bool deep = true) const;

			static 	BArchivable*	Instantiate(BMessage* archive);

			int32					CountBlocks() const;

			MediaBlock*				BlockAt(int32 index);
			MediaBlock*				LastBlock();

			status_t				AddBlock(MediaBlock* block, int32 index);
			status_t				AddBlock(MediaBlock* block);

			MediaBlock*				RemoveBlockAt(int32 index);
			bool					RemoveBlock(MediaBlock* index);

protected:
			//MediaBlockWriter*		GetBlockWriter();
			//PreviewReader*		GetPreviewReader();
			//MediaBlockReader*		GetBlockReader();

private:
			// Well, that will be replaced by an AVL Tree.
			// For now it's fine anyway.
			BObjectList<MediaBlock>	fMap;
};


class MediaBlockMapWriter {
public:
									MediaBlockMapWriter();

			MediaBlockMap*			Current() const;
			void					SetTo(MediaBlockMap* to);
			void					WriteFrames(void* buffer, int64 frameCount,
									int64 from = -1, bool overWrite = false);

private:
			mutable MediaBlockMap*	fMap;
};

#endif
