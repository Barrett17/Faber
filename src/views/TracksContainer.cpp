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

#include <stdlib.h>

#include "CommandBuilder.h"
#include "CommandServer.h"
#include "EffectWindow.h"
#include "FaberDefs.h"
#include "FaberScrollBar.h"
#include "InputRequest.h"
#include "ProjectManager.h"
#include "TimeBar.h"
#include "TracksCoordinator.h"
#include "WidgetFrame.h"


TracksContainer::TracksContainer()
	:
	BGroupView(B_VERTICAL, 0),
	fTracksCoordinator(this),
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


void
TracksContainer::HandleCommand(BMessage* message)
{
	message->PrintToStream();
	TrackView* track = _FindTrack(message);
	if (track != NULL) {
		if (message->AddPointer("tracks_container", this) == B_OK) {
			track->CommandForTrack(message);
		}
	}

	switch (message->what)
	{
		case FABER_ADD_TRACK:
		{
			Track* track = NULL;
			if (message->FindPointer("faber:track_pointer",
				(void**)&track) == B_OK)
				AddTrack(track);
		}		
		break;

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

		case FABER_REMOVE_TRACK:
		{
			TrackView* track = _FindTrack(message);

			if (track != NULL)
				RemoveTrack(track);
		}
		break;

		case FABER_TRACK_MOVE_UP:
		{
			if (CountTracks() <= 1)
				break;

			TrackView* track = _FindTrack(message);

			int32 index = IndexOf(track);

			if (index == 0)
				break;

			MoveTrack(track, index-1);
		}
		break;

		case FABER_TRACK_MOVE_DOWN:
		{
			if (CountTracks() <= 1)
				break;

			TrackView* track = _FindTrack(message);

			int32 index = IndexOf(track);

			if (CountTracks()-1 == index)
				break;

			MoveTrack(track, index+1);
		}
		break;

		case FABER_TRACK_MERGE_WITH:
		{
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
			fTracksCoordinator.SelectAll();
		break;
		
		case FABER_UNSELECT_ALL:
			fTracksCoordinator.UnselectAll();
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
			fTracksCoordinator.ZoomIn();
		break;
	
		case FABER_ZOOM_OUT:
			fTracksCoordinator.ZoomOut();
		break;
	
		case FABER_ZOOM_FULL:
			fTracksCoordinator.ZoomFull();
		break;

		case FABER_ZOOM_SELECTION:
			fTracksCoordinator.ZoomSelection();
		break;

		case FABER_EFFECT_CALL:
		{
			FaberEffect* effect;
			if (message->FindPointer("faber:effect_pointer",
				(void**)&effect) != B_OK) {
				WindowsManager::SimpleError("Error loading effect.");
				break;
			}

			EffectWindow* win = new EffectWindow(effect);
			win->Show();
			break;
		}

		case FABER_EFFECT_EXECUTE:
		{
			FaberEffect* effect;
			if (message->FindPointer("faber:effect_pointer",
				(void**)&effect) != B_OK) {
				WindowsManager::SimpleError("Error applying effect.");
				break;
			}

			int64 start = -1;
			int64 end = -1;
			fTracksCoordinator.CurrentSelection(&start, &end);
			TrackViewList* tracks = SelectedTracks();
			for (int32 i = 0; i < tracks->CountItems(); i++) {
				Track* item = tracks->ItemAt(i)->GetTrack();
				if (item == NULL)
					return;

				if (effect->FilterTrack(item,
					start, end) != B_OK)
					printf("Error with FilterTrack\n");
			}
			fTracksCoordinator.InvalidateSelection();
			delete tracks;
			break;
		}
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
			new AudioTrackView("AudioTrack", audioTrack, &fTracksCoordinator);

		if (index == -1)
			index = CountTracks();

		status_t ret = AddTrack(trackView, index);
		WindowsManager::PostMessage(MessageBuilder(FABER_UPDATE_MENU));
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
TracksContainer::RemoveTrack(TrackView* track, bool deep)
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

	if (deep) {
		ProjectManager::UnregisterTrack(track->GetTrack());
		delete track;
		WindowsManager::PostMessage(MessageBuilder(FABER_UPDATE_MENU));
	}
	return B_OK;
}


status_t
TracksContainer::MoveTrack(TrackView* track, int32 index)
{
	if (RemoveTrack(track, false) != B_OK)
		return B_ERROR;

	return AddTrack(track, index);
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


TrackViewList*
TracksContainer::SelectedTracks()
{
	TrackViewList* tracks = new TrackViewList(false);

	for (int i = 0; i < CountTracks(); i++) {
		TrackView* track = TrackAt(i);
		if (track->IsSelected())
			tracks->AddItem(track);
	}

	return tracks;
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
