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
#include <ScrollBar.h>

#include "CommandListener.h"
#include "AudioTrackView.h"


class TracksContainer : public BGroupView, public CommandListener {
public:
								TracksContainer();
	virtual 					~TracksContainer();

	// CommandListener
	virtual status_t			HandleCommand(BMessage* msg);


	// TracksContainer API

			int32 				CountTracks() const;
			TrackView*			TrackAt(int32 index) const;
			TrackView*			TrackByID(uint32 id);
			int32				IndexOf(TrackView* track) const;

			status_t 			AddTrack(TrackView* track, int32 index = -1);
			status_t			AddTrack(Track* track, int32 index = -1);

			status_t			RemoveTrack(int32 index);
			status_t			RemoveTrack(TrackView* track, bool deep = true);

			status_t			MoveTrack(TrackView* track, int32 index);

			void				SelectAll();
			void				UnselectAll();

			void				MoveUp(int32 id);
			void				MoveDown(int32 id);

			TrackView*			CurrentFocus();
			TrackViewList&		SelectedTracks();
		
			bool 				HasChanged();
		
			void	 			MuteAllTracks(bool mute);
			void				ReorderTracks(int reorder);
		
			void				CopyAndSilence();
			void				Copy();
			void				Cut();
			void				Paste();
			void				Clear();
		
			void				ZoomIn();
			void				ZoomOut();
			void				ZoomFull();
			void				ZoomSelection();
		
			// NOTE this will not update the scroll bar
			// don't use it, use the following method instead.
			void				UpdateTracksScroll(float newValue);
		
			void				UpdateRequested();
			void				UpdateRequested(BRect rect);

private:
			TrackView*			_FindTrack(BMessage* message);
			AudioTrackView*		_FindAudioTrack(BMessage* message);

			BGroupView*			fView;
			BGroupLayout*		fLayout;
		
			BScrollBar*			fVerticalScroll;
			BScrollBar*			fHorizontalScroll;
		
			TrackViewList		fTrackViews;
};

#endif
