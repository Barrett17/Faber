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

#include "AudioTrackView.h"

#include <Alert.h>
#include <LayoutBuilder.h>
#include <MenuBar.h>

#include "CommandBuilder.h"
#include "FaberDefs.h"
#include "IconButton.h"
#include "InputRequest.h"
#include "MenuBuilder.h"
#include "TracksContainer.h"
#include "WaveRender.h"

#define HEIGHT_VAL_REF 80


class TrackMenuUpdater : public MenuFilter {
public:
			TrackMenuUpdater(AudioTrackView* owner)
				:
				fOwner(owner)
				{}

	void	FilterMenu(BMenu* menu, uint32 message)
	{
	}

	void	FilterItem(BMenuItem* item, uint32 message)
	{
		fOwner->AddDefaultAttributes(item->Message());
	}

private:
	AudioTrackView* fOwner;
};


AudioTrackView::AudioTrackView(const char* name, AudioTrack* track,
	TracksCoordinator* coordinator, uint32 resizingMode)
	:
	TrackView(name, track, coordinator, resizingMode),
	fAudioTrack(track)
{
	fRender = new WaveRender(this, coordinator);

	BBox* box = new BBox("box");

	float size = HEIGHT_VAL_REF;

	if (fAudioTrack->CountChannels() == 1)
		size += 20;
	else if (fAudioTrack->CountChannels() > 0)
		size *= (float)fAudioTrack->CountChannels();

	box->SetExplicitSize(BSize(150, size));

	BIconButton* muteButton = new BIconButton(NULL, NULL, NULL, this);
	muteButton->SetToolTip(B_TRANSLATE("Mute Track"));
	muteButton->SetIcon(kMuteIcon);
	muteButton->TrimIcon();
	muteButton->SetValue(fAudioTrack->IsMute());

	BIconButton* recButton = new BIconButton(NULL, NULL, NULL, this);
	recButton->SetToolTip(B_TRANSLATE("Disable Recording"));
	recButton->SetIcon(kMiniRecordIcon);
	recButton->TrimIcon();

	BIconButton* soloButton = new BIconButton(NULL, NULL, NULL, this);
	soloButton->SetToolTip(B_TRANSLATE("Solo"));
	soloButton->SetIcon(kSoloIcon);
	soloButton->TrimIcon();

	BString trackName(track->Name());
	if (trackName.Length() > 13) {
		trackName.Truncate(9);
		trackName.Append("...");
	}

	TrackMenuUpdater filter(this);

	// Track menu
	fTrackMenu = MenuBuilder::BuildMenu(kTrackContextualMenu, &filter);
	fTrackMenu->SetTargetForItems(this);
	fTrackMenu->SetName(trackName.String());

	fToolButton = new BMenuBar(trackName.String());
	fToolButton->AddItem(fTrackMenu);
	fToolButton->SetToolTip(B_TRANSLATE("Track Options"));

	BMessage* closeMsg = CommandBuilder(FABER_REMOVE_TRACK);
	AddDefaultAttributes(closeMsg);

	BIconButton* closeButton = new BIconButton(NULL, NULL,
		closeMsg, this);

	closeButton->SetToolTip(B_TRANSLATE("Close"));
	closeButton->SetIcon(kCloseTrackIcon);
	closeButton->TrimIcon();

	PeakView* peak = new PeakView("OutputPeakView", false, false);
	peak->SetExplicitMaxSize(BSize(200, 15));

	BLayoutBuilder::Group<>(box, B_VERTICAL, 10)
		.SetInsets(10, 10, 10, 10)
		.AddSplit(B_VERTICAL, 0)
			.AddGroup(B_HORIZONTAL, 0)
				.Add(closeButton)
				.AddGlue()
				.Add(fToolButton)
			.End()
			.AddGroup(B_HORIZONTAL, 0)
				.Add(recButton)
				.Add(muteButton)
				.Add(soloButton)
				.AddGlue()
			.End()
			.Add(peak)
		.End()
		.AddGlue()
	.End();

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
		.Add(box)
		.Add(fRender)
	.End();
}


AudioTrackView::~AudioTrackView()
{
}


AudioTrack*
AudioTrackView::GetTrack() const
{
	return fAudioTrack;
}


void
AudioTrackView::CommandForTrack(BMessage* command)
{
	switch (command->what) {
		case FABER_TRACK_SET_NAME:
		{
			InputRequest* request = new InputRequest(
				B_TRANSLATE("Set Name"),
				B_TRANSLATE("Name:"),
				Name(),
				B_TRANSLATE("Ok"),
				B_TRANSLATE("Cancel"));

			char* input = NULL;
			ArrayDeleter<char> del(input);
			int32 ret = request->Go(&input);

			if (ret == 0)
				UpdateName(input);

			if (request->LockLooper())
				request->Quit();
		}
		break;

		case FABER_TRACK_GET_INFO:
		{
			// TODO: improve track informations
			BString alertText = "Track Informations:\n\n";

			alertText << "ID: " << fAudioTrack->ID() << "\n"
			<< "Name: " << fAudioTrack->Name() << "\n"
			<< "Frames: " << fAudioTrack->CountFrames() << "\n"
			<< "Channels: " << fAudioTrack->CountChannels() << "\n"
			<< "Duration: " << fAudioTrack->Duration() << "\n";

			WindowsManager::SimpleAlert(alertText.String());
		}
		break;

		case FABER_TRACK_SPLIT_CHAN:
		{
			// TODO should be disabled in menu
			// for mono tracks.
			if (CountChannels() < 2)
				return;

			TracksContainer* container = NULL;
			if (command->FindPointer("tracks_container",
				(void**)&container) != B_OK)
				break;

			for (int32 i = 0; i < CountChannels(); i++) {
				MediaBlockMap* chan = RemoveChannelAt(i);
				AudioTrack* audioTrack = new AudioTrack();
				audioTrack->SetName(Name());
				audioTrack->GetIndex()->AddChannel(chan);
				container->AddTrack(audioTrack,
					container->IndexOf(this)+1);
			}
		}
		break;

		case FABER_TRACK_ADD_CHAN:
		{
			AddChannel(new MediaBlockMap());
		}
		break;

		case FABER_TRACK_RM_CHAN:
		{
		}
		break;

		case FABER_TRACK_MUP_CHAN:
		{
		}
		break;

		case FABER_TRACK_MDOWN_CHAN:
		{
		}
		break;

	}
}


void
AudioTrackView::InvalidateRender()
{
	fRender->Invalidate();
}


void
AudioTrackView::UpdateName(const char* name)
{
	SetName(name);
	fToolButton->ItemAt(0)->SetLabel(name);
}


bool
AudioTrackView::IsSelected() const
{
	return fRender->IsSelected();
}


void
AudioTrackView::SelectAll()
{
	return fCoordinator->NotifySelectAll(fRender);
}


void
AudioTrackView::Unselect()
{
	return fCoordinator->NotifyUnselect(fRender);
}


void
AudioTrackView::CurrentSelection(int64* start, int64* end) const
{
	if (IsSelected())
		fCoordinator->AbsoluteSelection(start, end);
	else {
		*start = -1;
		*end = -1;
	}
}


void
AudioTrackView::Select(int64 start, int64 end)
{
	fCoordinator->NotifySelect(start, end, fRender);
}


void
AudioTrackView::AddDefaultAttributes(BMessage* message)
{
	message->AddUInt32(TRACK_MESSAGE_ID, ID());
}


int32
AudioTrackView::CountChannels() const
{
	return fAudioTrack->CountChannels();
}


void
AudioTrackView::AddChannel(MediaBlockMap* channel)
{
	fAudioTrack->GetIndex()->AddChannel(channel);
}


void
AudioTrackView::RemoveChannel(MediaBlockMap* channel)
{
	// TODO implement this
	//fAudioTrack->GetIndex()->RemoveChannel(channel);
}


MediaBlockMap*
AudioTrackView::RemoveChannelAt(int32 index)
{
	return fAudioTrack->GetIndex()->RemoveChannelAt(index);
}
