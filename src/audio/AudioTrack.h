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

			// BMediaFile-like API

			status_t		InitCheck() const;

			status_t		ReadFrames(void* buffer, int64* frameCount,
								media_header* header = NULL);

			status_t		ReplaceFrames(const void* buffer,
								int64* _frameCount,
								const media_header* header);

			status_t		SeekToTime(bigtime_t* _time, int32 flags = 0);
			status_t		SeekToFrame(int64* _frame, int32 flags = 0);

			status_t		WriteFrames(const void* data, int32 frameCount,
								int32 flags = 0);

			int64			CountFrames() const;

			 void 			SetFormat(media_format format);
			media_format 	Format() const;

			void			SetFramerate(float framerate);
			int32			CountChannels() const;

			// Other utils

			bool			IsAudio() const;

			bool			IsMono() const;
			bool			IsStereo() const;
			bool			IsMultichannel() const;

			BMediaFile*		MediaFile() const;

			status_t		ApplyEffect(int32 id, int64 start, int64 end);

private:
			media_format	fFormat;

			TrackIndex*		fTrackIndex;

};

typedef BObjectList<AudioTrack> AudioTrackList;

#endif
