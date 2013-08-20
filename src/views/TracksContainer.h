/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _TRACKS_CONTAINER
#define _TRACKS_CONTAINER

#include <GroupLayout.h>
#include <GroupView.h>

#include "TrackView.h"


class TracksContainer : public BGroupView
{
public:
						TracksContainer();
	virtual 			~TracksContainer();


	TrackView*			CurrentTrack();
	TrackView*			CurrentSelection();
	bool				IsSelected();
	int32 				CountTracks() const;
	TrackView*			TrackAt(int32 index) const;
	status_t 			AddTrack(TrackView* track, int32 index = 0);
	status_t			AddTrack(Track* track);

	status_t			RemoveTrack(TrackView* track = NULL);
	bool 				HasChanged();

	void				SelectAll();
	void				DeSelectAll();
	/*
	status_t 			MuteAllTracks(bool mute);
	//status_t			ReorderTracks(enum reorderType);*/

	void				ZoomIn();
	void				ZoomOut();
	void				ZoomFull();
	void				ZoomSelection();
//	void				ZoomRight();
//	void				ZoomLeft();

	void				Pulse();
	void				Invalidate();

	void				ReindexTracks();
	void				RedrawTracks();

	void				SetDirty(bool dirty);

private:
	BGroupLayout*		fLayout;
	TrackViewList		fTrackViews;

};

#endif
