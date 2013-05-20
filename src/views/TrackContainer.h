/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef TRACK_CONTAINER
#define TRACK_CONTAINER

#include <View.h>

#include "Track.h"


class TrackContainer : public BView {
public
						TrackContainer(const char* name = NULL);

	const BString&		Name();
	void 				SetName(const char* name);

	//void				Pulse();

						//CurrentSelection(*start, *end);

	void				SelectAt(start, end);
	void				SelectAll();
	void				Unselect();

	int32 				CountTracks() const;
	TrackView*			TrackAt(int32 index) const;
	status_t 			AddTrack(TrackView* track, int32 index = 0);
	status_t			RemoveTrack(TrackView* track = NULL,
							int32 index = 0);

	status_t 			MuteAllTracks(bool mute);
	status_t 			MuteTrack(bool mute);
	bool 				IsMute(int32 index);

	status_t			ReorderTracks(enum reorderType);

private:
}

#endif
