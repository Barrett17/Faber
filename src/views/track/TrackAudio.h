/*
 * Copyright 2013 Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Dario Casalinuovo
 */
#ifndef TRACK_AUDIO__H
#define TRACK_AUDIO__H

#include <View.h>
#include "Globals.h"

#include "SampleView.h"


class TrackAudio : public BView
{
public:
							TrackAudio(const char* name = "TrackAudioMono",
								uint32 resizingMode = B_FOLLOW_ALL_SIDES
								| B_WILL_DRAW);
	virtual 				~TrackAudio();
			void			Invalidate();
private:
			SampleView*		fSampleView;
};

#endif
