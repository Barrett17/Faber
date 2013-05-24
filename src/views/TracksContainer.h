/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _TRACKS_CONTAINER
#define _TRACKS_CONTAINER

#include <GroupLayout.h>
#include <GroupView.h>

#include "TimeBarView.h"
#include "TrackView.h"


class TracksContainer : public BGroupView
{
public:
						TracksContainer();
	virtual 			~TracksContainer();


	TrackView*			CurrentTrack();
	int32 				CountTracks() const;
	TrackView*			TrackAt(int32 index) const;
	status_t 			AddTrack(TrackView* track, int32 index = 0);

	status_t			RemoveTrack(TrackView* track = NULL);
/*
	status_t 			MuteAllTracks(bool mute);
	//status_t			ReorderTracks(enum reorderType);*/


	void				Pulse();
	void				Invalidate();

private:
	BGroupLayout*		fLayout;
	TrackViewList		fTrackViews;

	//TrackView*		fTrackView;
	TimeBarView*		fTimeBar;
};

#endif
