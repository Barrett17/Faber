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

		status_t						InitCheck() const {return B_OK;};

		int32							CountChannels() const { return 1;};
		void							AddChannel(MediaBlockTree* tree);
		MediaBlockTree*					GetChannel(int32 index) const;
		MediaBlockTree*					ExtractChannel(int32 index);

private:
		BFile*							fFile;
		BObjectList<MediaBlockTree*>	fChannels;
		media_format					fOutputFormat;
};

#endif
