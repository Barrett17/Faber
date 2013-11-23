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

#ifndef _AUDIO_TRACK_H
#define _AUDIO_TRACK_H

#include <MediaFile.h>
#include <MediaTrack.h>
#include <String.h>

#include "Track.h"


class AudioTrack : public Track {
public:
							AudioTrack();
							AudioTrack(BMediaFile* mediaFile);
	
							~AudioTrack();

			status_t		InitCheck() const;

	virtual bool			IsAudio() const;

			void			SetFramerate(float framerate);
			int32			CountChannels() const;

			int64			Frames() const;

			bool			IsMono() const;
			bool			IsStereo() const;

			BMediaFile*		MediaFile() const;

			status_t		ApplyEffect(int32 id, int64 start, int64 end);

private:
			BMediaFile*		fMediaFile;
			

			bool			fInitCheck;
};

#endif
