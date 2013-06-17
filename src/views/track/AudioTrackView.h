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

#include "AudioTrack.h"
#include "Globals.h"
#include "SampleView.h"
#include "TrackView.h"


class AudioTrackView : public TrackView
{
public:
							AudioTrackView(const char* name = "AudioTrackView",
								//AudioTrack* track,
								uint32 resizingMode = B_FOLLOW_ALL_SIDES
								| B_WILL_DRAW);
	virtual 				~AudioTrackView();

	virtual const BString&	Name() const;
	virtual void 			SetName(const char* name);

	virtual float			Volume() const;
	virtual void			SetVolume(float volume);

	virtual void	 		MuteTrack(bool mute);
	virtual bool 			IsMute() const;

	virtual void			SetSolo(bool solo);
	virtual bool			IsSolo() const;

			// Utility methods for fast access to the
			// Track's values
			//int32			ChannelCount() const;
			//int32			FrameRate() const;

			//AudioTrack*		Track() const;

			// Drawing related methods
			void			Pulse();
			void			Invalidate();
private:
			SampleView*		fSampleView;
			AudioTrack*		fTrack;
};

#endif
