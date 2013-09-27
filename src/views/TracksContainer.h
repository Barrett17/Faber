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

	void				MessageReceived(BMessage* message);

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
	void				UnselectAll();
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
//	void				Invalidate();

	void				ReindexTracks();
	void				RedrawTracks();

	void				SetDirty(bool dirty);

private:
	BGroupLayout*		fLayout;
	TrackViewList		fTrackViews;

};

#endif
