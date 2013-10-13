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

#include <LayoutBuilder.h>
#include <ScrollView.h>

#include "AudioTrackView.h"
#include "FaberMessages.h"
#include "FaberScrollBar.h"
#include "TrackView.h"
#include "WindowsManager.h"


TracksContainer::TracksContainer()
	:
	BGroupView(B_VERTICAL, 7.0f),
	fTrackViews(false)
{
	// TODO fix color schemes
	rgb_color backgroundColor = {120,120,120};
	rgb_color whiteBackgroundColor = { 240, 240, 240 };

	fView = new BGroupView(B_VERTICAL, 0);
	fLayout = new BGroupLayout(B_VERTICAL, 4.0f);

	BLayoutBuilder::Group<>(fView, B_VERTICAL, 0)
		.Add(fLayout)
		.AddGlue()
	.End();

	fView->SetViewColor(backgroundColor);

	BScrollView* verticalScrollView = new BScrollView("scrollviewV",
		fView, B_FOLLOW_NONE, false, true);

	fVerticalScroll = verticalScrollView->ScrollBar(B_VERTICAL);
	fVerticalScroll->SetRange(0, 0);
	fVerticalScroll->SetSteps(10, 200);

	// This is needed to stop the vertical scrollview to go out of the window
	verticalScrollView->SetExplicitMinSize(BSize(B_SIZE_UNSET, 200));

	fHorizontalScroll = new FaberScrollBar("scrollviewH",
		this, 0, 0, B_HORIZONTAL);

	fHorizontalScroll->SetRange(0,100);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(verticalScrollView)
		.Add(fHorizontalScroll)
	.End();
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


void
TracksContainer::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		case FABER_NEW_EMPTY_TRACK:
			AddTrack(new AudioTrack());
			break;

		case FABER_REMOVE_TRACK:
			//int32 id = message->FindInt32("track_id");
			// Track* track = TrackByID(id);
			// RemoveTrack(track);
			break;

		case B_SELECT_ALL:
			SelectAll();
			break;
		
		case FABER_UNSELECT_ALL:
			UnselectAll();
			break;

		case B_COPY:
			//fTrack->Copy();
			break;

		case FABER_COPY_SILENCE:
			//fTrack->CutAndSilence()
			break;

		case B_CUT:
			//fTrack->Cut(true);
			break;

		case B_PASTE:
			//fTrack->Paste();
			break;

		case FABER_DROP_PASTE:
			//fTrack->DropPaste();
			break;

		default:
			BView::MessageReceived(message);

	}
}


status_t
TracksContainer::AddTrack(TrackView* track, int32 index)
{
	fLayout->AddView(track, index);
	WindowsManager::MainWindow()->UpdateMenu();

	float max, min;
	if (Looper()->Lock()) {
		fVerticalScroll->GetRange(&min, &max);
		max += 250;
		fVerticalScroll->SetRange(min, max);
		Looper()->Unlock();
	}

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
TracksContainer::RemoveTrack(int32 index)
{
	TrackView* track = TrackAt(index);

	return RemoveTrack(track);
}


status_t
TracksContainer::RemoveTrack(TrackView* track)
{
	fLayout->RemoveView(track);

	float max, min;

	if (Looper()->Lock()) {
		fVerticalScroll->GetRange(&min, &max);
		max -= 250;
		fVerticalScroll->SetRange(min, max);

		Looper()->Unlock();
	}

	fTrackViews.RemoveItem(track);
	delete track;

	return B_OK;
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
	for (int i = 0; i < CountTracks(); i++) {
		TrackView* track = TrackAt(i);
		if (track->IsFocus())
			return track;
	}

	return NULL;
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
		TrackView* track = TrackAt(i);
		track->SelectAll();
	}
}


void
TracksContainer::UnselectAll()
{
	for (int i = 0; i < CountTracks(); i++) {
		TrackView* track = TrackAt(i);
		track->Unselect();
	}
}


void
TracksContainer::ZoomIn()
{
	for (int i = 0; i < CountTracks(); i++) {
		TrackView* track = TrackAt(i);
		track->ZoomIn();
	}
}


void
TracksContainer::ZoomOut()
{
	for (int i = 0; i < CountTracks(); i++) {
		TrackView* track = TrackAt(i);
		track->ZoomOut();
	}
}


void
TracksContainer::ZoomFull()
{
	for (int i = 0; i < CountTracks(); i++) {
		TrackView* track = TrackAt(i);
		track->ZoomFull();
	}	
}


void
TracksContainer::ZoomSelection()
{
	for (int i = 0; i < CountTracks(); i++) {
		TrackView* track = TrackAt(i);
		track->ZoomSelection();
	}
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
TracksContainer::UpdateTracksScroll(float newValue)
{
	float max, min;
	fVerticalScroll->GetRange(&min, &max);

	for (int i = 0; i < CountTracks(); i++) {
		AudioTrackView* track = (AudioTrackView*) TrackAt(i);
		track->UpdateScroll(newValue, max, min);
	}
}
