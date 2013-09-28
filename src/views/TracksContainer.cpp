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

#include "TracksContainer.h"

#include <GroupLayoutBuilder.h>

#include "AudioTrackView.h"
#include "TrackView.h"
#include "WindowsManager.h"


TracksContainer::TracksContainer()
	:
	BGroupView(B_VERTICAL, 5.0f),
	fTrackViews(false)
{
	SetViewColor(Prefs.back_color);
	BGroupLayoutBuilder(GroupLayout()).AddGlue(5.0f);
}


TracksContainer::~TracksContainer()
{
}


void
TracksContainer::Pulse()
{
	for (int i = 0; i < CountTracks(); i++)
		TrackAt(i)->Pulse();
}


status_t
TracksContainer::AddTrack(TrackView* track, int32 index)
{
	GroupLayout()->AddView(track);
	WindowsManager::MainWindow()->UpdateMenu();

	return fTrackViews.AddItem(track, index);
}


status_t
TracksContainer::AddTrack(Track* track)
{
	if (track->IsAudio()) {
		AudioTrackView* trackView = new AudioTrackView("AudioTrack",
			(AudioTrack*) track);
		return AddTrack(trackView);
	}
	return B_ERROR;
}


status_t
TracksContainer::RemoveTrack(TrackView* track)
{
	GroupLayout()->RemoveView(track);
	return fTrackViews.RemoveItem(track);
}


TrackView*
TracksContainer::TrackAt(int32 index) const
{
	return fTrackViews.ItemAt(index);
}


int32
TracksContainer::CountTracks() const
{
	return fTrackViews.CountItems();
}


TrackView*
TracksContainer::CurrentTrack()
{
	
}


bool
TracksContainer::IsSelected()
{
	
}


bool
TracksContainer::HasChanged()
{
	return false;
}


void
TracksContainer::SelectAll()
{
	for (int i = 0; i < CountTracks(); i++) {
		AudioTrackView* track = (AudioTrackView*) TrackAt(i);
		track->SelectAll();
	}
}


void
TracksContainer::UnselectAll()
{
	for (int i = 0; i < CountTracks(); i++) {
		AudioTrackView* track = (AudioTrackView*) TrackAt(i);
		track->Unselect();
	}
}


void
TracksContainer::ZoomIn()
{
	
}


void
TracksContainer::ZoomOut()
{
	
}


void
TracksContainer::ZoomFull()
{
	
}


void
TracksContainer::ZoomSelection()
{
	
}


void
TracksContainer::SetDirty(bool dirty)
{
	for (int i = 0; i < CountTracks(); i++) {
		AudioTrackView* track = (AudioTrackView*) TrackAt(i);
		track->SetDirty(dirty);
	}
}


void
TracksContainer::MessageReceived(BMessage* message)
{
	//if (CountTracks() == 0)
		//	return;

	switch (message->what)
	{

		case B_SELECT_ALL:
			//SelectAll();
			break;
		
		case UNSELECT_ALL:
			//UnselectAll();
			break;

		case ZOOM_IN:
			ZoomIn();
			break;
	
		case ZOOM_OUT:
			ZoomOut();
			break;
	
		case ZOOM_FULL:
			ZoomFull();
			break;

		case ZOOM_SELECTION:
			ZoomSelection();
			break;

		case B_COPY:
			//fTrack->Copy();
			break;

		case COPY_SILENCE:
			//fTrack->CutAndSilence()
			break;

		case B_CUT:
			//fTrack->Cut(true);
			break;

		case B_PASTE:
			//fTrack->Paste();
			break;

		case DROP_PASTE:
			//fTrack->DropPaste();
			break;

		default:
			BView::MessageReceived(message);

	}
}
