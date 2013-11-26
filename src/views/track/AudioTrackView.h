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

#ifndef _AUDIO_TRACK_VIEW__H
#define _AUDIO_TRACK_VIEW__H

#include "AudioTrackView.h"

#include <PopUpMenu.h>
#include <View.h>

#include "AudioTrack.h"
#include "TrackView.h"
#include "WaveRender.h"


class AudioTrackView : public TrackView
{
public:
							AudioTrackView(const char* name = "AudioTrackView",
								AudioTrack* track = NULL,
								uint32 resizingMode = B_FOLLOW_LEFT_RIGHT |
									B_FRAME_EVENTS);

	virtual 				~AudioTrackView();

			void			FrameResized();

			// Utility methods for fast access to the
			// Track's values
			int32			CountChannels() const;
			int32			FrameRate() const;

			AudioTrack*		Track() const;

			// GUI interaction
			void			ZoomIn();
			void			ZoomOut();
			void			ZoomFull();
			void			ZoomSelection();

			void			UpdateScroll(float newValue,
								float max, float min);

			bool			IsSelected() const;


private:
			AudioTrack*		fAudioTrack;
			WaveRender*		fWaveRender;

			bool			fUpdateDrawCache;
			bool			fDirty;
};

#endif
