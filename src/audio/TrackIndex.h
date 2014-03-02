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

#include <Archivable.h>
#include <MediaDefs.h>
#include <ObjectList.h>

#include "MediaBlock.h"


class TrackIndex : public BArchivable {
public:
										TrackIndex();

		status_t						InitCheck() const;

		uint32							CountChannels() const;
		void							AddChannel(MediaBlockMap* tree);
		MediaBlockMap*					GetChannel(int32 index) const;

		BObjectList<MediaBlockMap>&		GetChannels() const;

		// The following method will detach and remove ownership of the channel
		// from the index.
		MediaBlockMap*					ExtractChannel(int32 index);

private:
		BObjectList<MediaBlockMap>*		fChannels;
		media_format					fOutputFormat;
		status_t						fInitErr;
};

#endif
