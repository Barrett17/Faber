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

#ifndef _TRACK_INDEX_H
#define _TRACK_INDEX_H

#include <File.h>
#include <Path.h>

#include "MediaBlock.h"


class TrackIndex {
public:
							TrackIndex() {};

		//					CountRevisions() const;

		int32				CountBlocks() const;

		const block_ref*	BlockAt(int32 index);

		status_t			AddBlock(int32 index, MediaBlock* block,
								bool saveAndFree = false);

		status_t			AddBlock(MediaBlock* block,
								bool saveAndFree = false);

		status_t			AddBlock(int32 index, block_ref* block);

		status_t			RemoveBlock(int32 index);

		status_t			RemoveBlocks(int32 start, int32 end);
private:
		BFile*				fFile;
};

#endif
