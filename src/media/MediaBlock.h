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

#include <File.h>
#include <ObjectList.h>

#include "DataBlock.h"


class MediaBlock : public DataBlock {
public:
									MediaBlock(BFile* file);	

			int16*					ReadPreview(size_t* size);

			status_t				ReadFrames(void* buffer, int64* frameCount);
			int64					CountFrames() const;

			status_t				SeekToFrame(int64* frame);

			MediaBlock*				CopyTo(BFile* file);

			//status_t				Acquire(TrackIndex* index);
			//status_t				Release(TrackIndex* index);

private:
			int64					fFramesCount;
			BFile*					fData;
};


class MediaBlockNode {
public:
									MediaBlockNode(MediaBlock* block,
										bool isSentinel = false);

private:
		MediaBlock*					fBlock;
};


class MediaBlockMap {
public:
		int32						CountBlocks() const;

		MediaBlock*					BlockAt(int32 index);

		status_t					AddBlock(MediaBlock* block, int32 index = -1);

		status_t					RemoveBlock(int32 index);
		status_t					RemoveBlocks(int32 start, int32 end);

protected:
		//PreviewIterator*			GetPreview();
		//MediaBlockIterator*		GetBlocks();

private:
		BObjectList<MediaBlock>		fMap;
};


class MediaBlockMapWriter {
public:
									MediaBlockMapWriter();

		bool						IsValid() const;
		void						SetTo(MediaBlockMap* to);
		status_t					WriteFrames(void* buffer, int64 frameCount,
										int64 from = -1, bool overWrite = false);

private:
		MediaBlockMap*				fDestination;

};

#endif
