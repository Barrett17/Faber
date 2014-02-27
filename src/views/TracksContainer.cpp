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
#include <ScrollView.h>
#include <StringView.h>

#include "AudioTrackView.h"
#include "FaberDefs.h"
#include "FaberScrollBar.h"
#include "MenuBuilder.h"
#include "ProjectManager.h"
#include "TimeBar.h"
#include "AudioGate.h"
#include "TrackView.h"
#include "WidgetFrame.h"


TracksContainer::TracksContainer()
	:
	BGroupView(B_VERTICAL, 0),
	fTrackViews(false),
	fStart(0),
	fEnd(0),
	fDuration(0)
{
	// TODO fix color schemes
	rgb_color backgroundColor = {120,120,120};

	fView = new BGroupView(B_VERTICAL, 0);
	fLayout = new BGroupLayout(B_VERTICAL, 4.0f);

	BGroupLayoutBuilder(fView)
		.AddStrut(1.0f)
		.Add(fLayout)
		.AddGlue()
	.End();

	fView->SetViewColor(backgroundColor);

	BScrollView* verticalScrollView = new BScrollView("scrollviewV",
		fView, B_FOLLOW_NONE, false, true);
	// This is needed to stop the vertical scrollview to go out of the window
	verticalScrollView->SetExplicitMinSize(BSize(B_SIZE_UNSET, 200));

	fVerticalScroll = verticalScrollView->ScrollBar(B_VERTICAL);
	fVerticalScroll->SetRange(0, 0);
	fVerticalScroll->SetSteps(10, 200);

	fHorizontalScroll = new FaberScrollBar("scrollviewH",
		this, 0, 0, B_HORIZONTAL);
	fHorizontalScroll->SetRange(0,0);

	float scrollBarWidth = fVerticalScroll->Bounds().Width();

	WidgetFrame* timeBarLabel = new WidgetFrame(new BStringView("", " Timecode"));
	timeBarLabel->SetExplicitSize(BSize(150, 10));

	BGroupLayoutBuilder(this)
		.AddGroup(B_HORIZONTAL, 0.5f)
			.Add(timeBarLabel)
			.Add(new TimeBar())
		.End()
		.Add(verticalScrollView)
		.AddGroup(B_HORIZONTAL, 0)
			.AddStrut(150)
			.Add(fHorizontalScroll)
			.AddStrut(scrollBarWidth)
		.End()
	.End();

	timeBarLabel->SetViewColor(backgroundColor);
}


TracksContainer::~TracksContainer()
{
}


void
TracksContainer::MessageReceived(BMessage* message)
{
	message->PrintToStream();

	switch (message->what)
	{
		case FABER_NEW_AUDIO_TRACK:
		{
			AudioTrack* track = new AudioTrack();
			AddTrack(track);

			break;
		}

		case FABER_NEW_LABEL_TRACK:
		{
			//LabelTrack* track = new LabelTrack();
			//AddTrack(track);

			break;
		}

		case FABER_REMOVE_TRACK:
		{
			uint32 id;
			message->FindUInt32("track_id", &id);
			TrackView* track = TrackByID(id);
			if (track != NULL)
				RemoveTrack(track);
			else
				// TODO report error

			break;
		}

		case FABER_MUTE_ALL:
			MuteAllTracks(true);
			break;

		case FABER_UNMUTE_ALL:
			MuteAllTracks(false);
			break;

		case FABER_SORT_BY_NAME:
			ReorderTracks(0);
			break;

		case FABER_SORT_BY_DURATION:
			ReorderTracks(1);
			break;

		case B_SELECT_ALL:
			SelectAll();
			break;
		
		case FABER_UNSELECT_ALL:
			UnselectAll();
			break;

		case B_COPY:
			Copy();
			break;

		case FABER_COPY_SILENCE:
			CopyAndSilence();
			break;

		case B_CUT:
			Cut();
			break;

		case B_PASTE:
			Paste();
			break;

		case FABER_CLEAR:
			Clear();
			break;

		case FABER_ZOOM_IN:
			ZoomIn();
			break;
	
		case FABER_ZOOM_OUT:
			ZoomOut();
			break;
	
		case FABER_ZOOM_FULL:
			ZoomFull();
			break;

		case FABER_ZOOM_SELECTION:
			ZoomSelection();
			break;

		default:
			BGroupView::MessageReceived(message);

	}
}


status_t
TracksContainer::AddTrack(TrackView* track, int32 index)
{
	float max, min;

	if (Looper()->Lock()) {
		fVerticalScroll->GetRange(&min, &max);
		max += 170;
		fVerticalScroll->SetRange(min, max);
		Looper()->Unlock();
	}

	status_t ret;

	if (track->GetTrack()->IsAudio()) {
		fLayout->AddView(track);
		ret = fTrackViews.AddItem(track, index);
	}

	MenuBuilder::UpdateMenu();

	return ret;
}


status_t
TracksContainer::AddTrack(Track* track)
{
	if (track->IsAudio()) {
		AudioTrack* audioTrack = (AudioTrack*) track;

		ProjectManager::RegisterTrack(track);

		AudioTrackView* trackView =
			new AudioTrackView("AudioTrack", audioTrack);

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
	float max, min;

	if (Looper()->Lock()) {
		fVerticalScroll->GetRange(&min, &max);
		max -= 170;
		fVerticalScroll->SetRange(min, max);

		Looper()->Unlock();
	}

	fTrackViews.RemoveItem(track);
	fLayout->RemoveView(track);
	ProjectManager::UnregisterTrack(track->GetTrack());
	delete track;

	return B_OK;
}


TrackView*
TracksContainer::TrackAt(int32 index) const
{
	return fTrackViews.ItemAt(index);
}


TrackView*
TracksContainer::TrackByID(uint32 id)
{
	for (int i = 0; i < CountTracks(); i++) {
		TrackView* track = TrackAt(i);
		if (track->ID() == id)
			return track;
	}
	return NULL;
}


int32
TracksContainer::CountTracks() const
{
	return fTrackViews.CountItems();
}


TrackView*
TracksContainer::CurrentFocus()
{
	for (int i = 0; i < CountTracks(); i++) {
		TrackView* track = TrackAt(i);
		if (track->IsFocus())
			return track;
	}

	return NULL;
}


TrackViewList&
TracksContainer::SelectedTracks()
{
	TrackViewList* tracks = new TrackViewList(false);

	for (int i = 0; i < CountTracks(); i++) {
		TrackView* track = TrackAt(i);
		if (track->IsSelected())
			tracks->AddItem(track);
	}

	return *tracks;
}


bool
TracksContainer::HasChanged()
{
	for (int i = 0; i < CountTracks(); i++) {
		TrackView* track = TrackAt(i);
		if (track->HasChanged())
			return true;
	}
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

	if (Looper()->Lock()) {
		float max, min;
		fHorizontalScroll->GetRange(&min, &max);
		if (max == 0)
			max = 2;

		max *= 4;
		fHorizontalScroll->SetRange(min, max);
		Looper()->Unlock();
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
TracksContainer::MuteAllTracks(bool mute)
{
	for (int i = 0; i < CountTracks(); i++) {
		TrackView* track = TrackAt(i);
		track->MuteTrack(mute);
	}
}


void
TracksContainer::ReorderTracks(int order)
{

}


void
TracksContainer::Copy()
{

}


void
TracksContainer::CopyAndSilence()
{

}


void
TracksContainer::Cut()
{

}


void
TracksContainer::Paste()
{

}


void
TracksContainer::Clear()
{

}


void
TracksContainer::UpdateTracksScroll(float newValue)
{
	printf("UpdateScroll\n");
	float max, min;
	fVerticalScroll->GetRange(&min, &max);

	for (int i = 0; i < CountTracks(); i++) {
		AudioTrackView* track = (AudioTrackView*) TrackAt(i);
		track->UpdateScroll(newValue, max, min);
	}
}
