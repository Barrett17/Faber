/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _TRACKS_CONTAINER
#define _TRACKS_CONTAINER

#include <View.h>

#include "TimeBarView.h"
#include "TrackView.h"


class TracksContainer : public BView
{
public:
						TracksContainer();
	virtual 			~TracksContainer();

						//CurrentSelection(*start, *end);

	/*void				SelectAt(start, end);
	void				SelectAll();
	void				Unselect();

	TrackView*			CurrentTrack();
	int32 				CountTracks() const;
	TrackView*			TrackAt(int32 index) const;
	status_t 			AddTrack(TrackView* track, int32 index = 0);

	status_t			RemoveTrack(TrackView* track = NULL,
							int32 index = 0);

	status_t 			MuteAllTracks(bool mute);
	//status_t			ReorderTracks(enum reorderType);*/


	void				Pulse();
	void				Invalidate();

private:
	//TrackViewList		fTrackViews;

	TrackView*			fTrackView;
	TimeBarView*		fTimeBar;
};

#endif
