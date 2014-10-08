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

#include <stdlib.h>

#include "AudioTrackView.h"
#include "CommandBuilder.h"
#include "CommandServer.h"
#include "FaberDefs.h"
#include "FaberScrollBar.h"
#include "InputRequest.h"
#include "MenuBuilder.h"
#include "ProjectManager.h"
#include "TimeBar.h"
#include "AudioGate.h"
#include "TracksCommon.h"
#include "TrackView.h"
#include "WidgetFrame.h"


TracksContainer::TracksContainer()
	:
	BGroupView(B_VERTICAL, 0),
	fTrackViews(false)
{
	CommandServer::AddCommandListener(this);

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


status_t
TracksContainer::HandleCommand(BMessage* message)
{
	//message->PrintToStream();

	if (message->what != FABER_REMOVE_TRACK) {
		TrackView* track = _FindTrack(message);
		if (track != NULL) {
			if (message->AddPointer("tracks_container", this) == B_OK) {
				track->CommandForTrack(message);
				return B_OK;
			}
		}
	}

	switch (message->what)
	{
		case FABER_NEW_MONO_TRACK:
		{
			AudioTrack* track = new AudioTrack(1);
			AddTrack(track);
		}
		break;

		case FABER_NEW_STEREO_TRACK:
		{
			AudioTrack* track = new AudioTrack(2);
			AddTrack(track);
		}
		break;

		case FABER_NEW_MULTICHANNEL_TRACK:
		{
			InputRequest* request = new InputRequest(
				B_TRANSLATE("Set Channel Count"),
				B_TRANSLATE("Channels:"),
				"8",
				B_TRANSLATE("Ok"),
				B_TRANSLATE("Cancel"));

			char* input = NULL;
			int32 ret = request->Go(&input);
			ArrayDeleter<char> del(input);

			if (ret == 0) {
				// TODO check input for non numeric characters
				int32 channels = (int) strtol(input, (char**) NULL, 10);
				if (channels < 1)
					break;

				AudioTrack* track = new AudioTrack(channels);
				AddTrack(track);
			}
		}
		break;

		case FABER_NEW_LABEL_TRACK:
		{
			//LabelTrack* track = new LabelTrack();
			//AddTrack(track);
		}
		break;

		case FABER_REMOVE_TRACK:
		{
			TrackView* track = _FindTrack(message);

			if (track != NULL)
				RemoveTrack(track);
		}
		break;

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
			return B_ERROR;
	}

	return B_OK;
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

	if (index == -1)	
		index = CountTracks();

	fLayout->AddView(index, track);

	return fTrackViews.AddItem(track, index);
}


status_t
TracksContainer::AddTrack(Track* track, int32 index)
{
	if (track->IsAudio()) {
		AudioTrack* audioTrack = (AudioTrack*) track;

		ProjectManager::RegisterTrack(track);

		AudioTrackView* trackView =
			new AudioTrackView("AudioTrack", audioTrack);

		if (index == -1)
			index = CountTracks();

		status_t ret = AddTrack(trackView, index);

		CommandServer::SendCommand(CommandBuilder(FABER_UPDATE_MENU));

		return ret;
	}
	return B_ERROR;
}


status_t
TracksContainer::RemoveTrack(int32 index)
{
	return RemoveTrack(TrackAt(index));
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

	CommandServer::SendCommand(CommandBuilder(FABER_UPDATE_MENU));

	return B_OK;
}


TrackView*
TracksContainer::TrackAt(int32 index) const
{
	return fTrackViews.ItemAt(index);
}


int32
TracksContainer::IndexOf(TrackView* track) const
{
	return fTrackViews.IndexOf(track);
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
	/*TrackViewList* tracks = new TrackViewList(false);

	for (int i = 0; i < CountTracks(); i++) {
		TrackView* track = TrackAt(i);
		if (track->IsSelected())
			tracks->AddItem(track);
	}

	return *tracks;*/
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
TracksContainer::UpdateRequested()
{
	for (int i = 0; i < CountTracks(); i++)
		TrackAt(i)->UpdateRequested();
}


void
TracksContainer::UpdateRequested(BRect rect)
{
	for (int i = 0; i < CountTracks(); i++)
		TrackAt(i)->UpdateRequested(rect);
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
TracksContainer::SelectAll()
{
	GetCoords().SelectAll();
	UpdateRequested();
}


void
TracksContainer::UnselectAll()
{
	GetCoords().Unselect();
	UpdateRequested();
}


void
TracksContainer::UpdateTracksScroll(float newValue)
{
}


void
TracksContainer::ZoomIn()
{
	GetCoords().ZoomIn();
	UpdateRequested();
}


void
TracksContainer::ZoomOut()
{
	GetCoords().ZoomOut();
	UpdateRequested();
}


void
TracksContainer::ZoomFull()
{
	GetCoords().ZoomFull();
	UpdateRequested();
}


void
TracksContainer::ZoomSelection()
{
	GetCoords().ZoomSelection();
	UpdateRequested();
}


TrackView*
TracksContainer::_FindTrack(BMessage* message)
{
	uint32 id;
	if (message->FindUInt32(TRACK_MESSAGE_ID, &id) != B_OK)
		return NULL;

	return TrackByID(id);
}


AudioTrackView*
TracksContainer::_FindAudioTrack(BMessage* message)
{
	TrackView* track = _FindTrack(message);

	if (track == NULL)
		return NULL;

	if (track->GetTrack()->IsAudio())
		return (AudioTrackView*) track;

	return NULL;
}
