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
#include <ObjectList.h>
#include <String.h>

#include "Track.h"
#include "TrackIndex.h"


class AudioTrack : public Track {
public:
							AudioTrack();
							AudioTrack(TrackIndex* index);

							~AudioTrack();

			status_t		InitCheck() const;

			int32			CountChannels() const;

			// TODO generalize a bit this notion creating a base type
			// of objects suited to modify the track, FaberEffect
			// will be derived from it.
			//status_t		ApplyEffect(int32 id, int64 start, int64 end);

			//status_t		ExecuteOperation(AudioTrackOperation* operation);

			// Other utils
			bool			IsAudio() const;

			bool			IsMono() const;
			bool			IsStereo() const;
			bool			IsMultichannel() const;

private:
			TrackIndex*		fTrackIndex;

};

typedef BObjectList<AudioTrack> AudioTrackList;

#endif
