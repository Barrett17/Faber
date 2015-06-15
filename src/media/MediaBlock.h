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
#include <Entry.h>
#include <File.h>
#include <ObjectList.h>

#include "DataBlock.h"

typedef struct {
	//uint32 media_block_map_id;
	int64 frame_count;
	int64 preview_frames;
} media_block_meta_data;


#define MEDIA_BLOCK_RESERVED_SIZE sizeof(media_block_meta_data)
#define MEDIA_BLOCK_PREVIEW_DETAIL 256
#define MEDIA_BLOCK_MAX_FRAMES 204800
#define MEDIA_BLOCK_MAX_SIZE MEDIA_BLOCK_MAX_FRAMES*sizeof(float)
#define MEDIA_BLOCK_PREVIEW_MAX_FRAMES (MEDIA_BLOCK_MAX_FRAMES/MEDIA_BLOCK_PREVIEW_DETAIL)*2
#define MEDIA_BLOCK_PREVIEW_MAX_SIZE MEDIA_BLOCK_PREVIEW_MAX_FRAMES*sizeof(float)
#define MEDIA_BLOCK_PREVIEW_START MEDIA_BLOCK_RESERVED_SIZE
#define MEDIA_BLOCK_RAW_DATA_START MEDIA_BLOCK_PREVIEW_START + MEDIA_BLOCK_PREVIEW_MAX_SIZE


class MediaBlock : public DataBlock {
public:
									MediaBlock(entry_ref& ref);	
	virtual 						~MediaBlock();
protected:
			int64					WriteFrames(void* buf, int64 count);
			int64					ReadFrames(void* buf, int64 count);

			void*					Data() const;
			int64					Offset() const;
			void					SetOffset(int64 offset);
			void					SetFramesUsed(int64 frames);			
			int64					CountFrames() const;

			bool					IsFull() const;
			int64					AvailableFrames() const;

			void*					Preview() const;
			int64					PreviewFrames() const;

			// Enable the block to be written
			void					Open(uint32 openMode);
			void					Close();
			// Load all from the file
			void					Load();
			// Update file content with changes and calculate preview
			void					Flush();

			void					SetFlushed(bool flushed);
			bool					WasFlushed() const;

			status_t				CopyTo(MediaBlock* block);
			entry_ref				GetEntry() const;

	friend class MediaBlockMap;
	friend class MediaBlockMapVisitor;
	friend class MediaBlockMapWriter;
	friend class MediaBlockMapReader;

private:
			void					_LoadMetaData();
			void					_LoadData();
			void					_LoadPreview();

			void					_FlushMetaData();
			void					_FlushData();
			void					_FlushPreview();

			entry_ref				fEntry;
			BFile*					fFile;

			bool					fFlushed;

			void*					fBuffer;
			void*					fPreviewCache;

			int64					fOffset;
			media_block_meta_data	fMetaData;
};

#endif
