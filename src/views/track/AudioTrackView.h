/*
 * Copyright 2013 Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Dario Casalinuovo
 */
#ifndef _AUDIO_TRACK_VIEW__H
#define _AUDIO_TRACK_VIEW__H

#include "AudioTrackView.h"

#include <View.h>

#include "Globals.h"
#include "SampleView.h"
#include "TrackView.h"


class AudioTrackView : public TrackView
{
public:
							AudioTrackView(const char* name = "AudioTrackView",
								uint32 resizingMode = B_FOLLOW_ALL_SIDES
								| B_WILL_DRAW);
	virtual 				~AudioTrackView();

			void			Pulse();
			void			Invalidate();
private:
			SampleView*		fSampleView;
};

#endif
