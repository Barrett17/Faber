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


class MediaBlock : public DataBlock {
public:
									MediaBlock(BFile* file,
										BEntry* entry);	
	virtual 						~MediaBlock();

			int64					CountFrames() const;
			status_t				SeekToFrame(int64* frame);

			MediaBlock*				CopyTo(BFile* file);

			//status_t				Acquire(TrackIndex* index);
			//status_t				Release(TrackIndex* index);

			const BEntry&			GetEntry() const;

protected:
			friend class			MediaBlockMapWriter;

			off_t					GetSize();
			BFile*					GetFile() const;

private:
			BFile*					fData;
			BEntry*					fEntry;
};

#endif
