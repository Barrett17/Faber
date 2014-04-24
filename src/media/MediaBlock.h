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


#include <Archivable.h>
#include <File.h>
#include <ObjectList.h>

#include "DataBlock.h"


/*
	Audio Block Structure :

		1 KiB - Reserved for general informations (size, blockstart,
			reference counter, etc)

		4096 bytes - 16 bit display preview (1048576/256)

		1 MiB - Audio data (B_AUDIO_FLOAT)
*/

#define MEDIA_BLOCK_PREVIEW_DETAIL 256

#define MEDIA_BLOCK_RAW_SIZE 1048576

#define MEDIA_BLOCK_RESERVED_SIZE 1024

#define MEDIA_BLOCK_PREVIEW_START MEDIA_BLOCK_RESERVED_SIZE

#define MEDIA_BLOCK_RAW_DATA_START MEDIA_BLOCK_PREVIEW_START + MEDIA_BLOCK_PREVIEW_SIZE

#define MEDIA_BLOCK_DEFAULT_SIZE MEDIA_BLOCK_RESERVED_SIZE + MEDIA_BLOCK_PREVIEW_SIZE

#define MEDIA_BLOCK_SIZE_LIMIT MEDIA_BLOCK_DEFAULT_SIZE + MEDIA_BLOCK_RAW_SIZE

#define MEDIA_BLOCK_RAW_FRAMES MEDIA_BLOCK_RAW_SIZE/sizeof(float)

#define MEDIA_BLOCK_PREVIEW_SIZE (MEDIA_BLOCK_RAW_FRAMES/MEDIA_BLOCK_PREVIEW_DETAIL)*2

#define MEDIA_BLOCK_PREVIEW_FRAMES MEDIA_BLOCK_PREVIEW_SIZE*sizeof(int16)


class MediaBlock : public DataBlock {
public:
									MediaBlock(BFile* file,
										BEntry* entry);	
	virtual 						~MediaBlock();

			int64					BeginFrame();
			int64					CountFrames() const;
			status_t				SeekToFrame(int64* frame);
			int64					CurrentFrame() const;

			MediaBlock*				CopyTo(BFile* file);

			//status_t				Acquire(TrackIndex* index);
			//status_t				Release(TrackIndex* index);

			const BEntry&			GetEntry() const;

			size_t					RawDataSize() const;

protected:
			friend class			MediaBlockMapWriter;
			friend class			MediaBlockMapReader;
			friend class			PreviewReader;

private:
			BFile*					fData;
			BEntry*					fEntry;
};

#endif
