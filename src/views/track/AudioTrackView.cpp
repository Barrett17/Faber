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

#include <Box.h>
#include <LayoutBuilder.h>
#include <MediaTrack.h>
#include <PopUpMenu.h>
#include <StringView.h>

#include "FaberDefs.h"
#include "IconButton.h"
#include "MenuManager.h"
#include "ToolButton.h"
#include "VolumeSlider.h"

#define HEIGHT_VAL_REF 75


AudioTrackView::AudioTrackView(const char* name, AudioTrack* track,
	uint32 resizingMode)
	:
	TrackView(name, track, resizingMode),
	fAudioTrack(track)
{
	fWaveRender = new WaveRender(fAudioTrack);

	BBox* box = new BBox("box");

	box->SetExplicitSize(BSize(150, HEIGHT_VAL_REF*track->CountChannels()));

	IconButton* muteButton = new IconButton(NULL, 0, NULL, NULL, this);
	muteButton->SetToolTip(B_TRANSLATE("Mute Track"));
	muteButton->SetIcon(kMuteIcon);
	muteButton->TrimIcon();
	muteButton->SetValue(fAudioTrack->IsMute());

	IconButton* recButton = new IconButton(NULL, 0, NULL, NULL, this);
	recButton->SetToolTip(B_TRANSLATE("Disable Recording"));
	recButton->SetIcon(kMiniRecordIcon);
	recButton->TrimIcon();

	IconButton* soloButton = new IconButton(NULL, 0, NULL, NULL, this);
	soloButton->SetToolTip(B_TRANSLATE("Solo"));
	soloButton->SetIcon(kSoloIcon);
	soloButton->TrimIcon();

	BString trackName(track->Name());
	if (trackName.Length() > 15) {
		trackName.Truncate(12);
		trackName.Append("...");
	}

	ToolButton* toolButton = new ToolButton(NULL, trackName.String(), NULL);
	toolButton->SetToolTip(B_TRANSLATE("Track Options"));
	toolButton->SetIcon(kMuteIcon);

	// Track menu
	BPopUpMenu* trackMenu = MenuManager::Get()->BuildTrackContextualMenu();
	toolButton->SetMenu(trackMenu);

	trackMenu->SetTargetForItems(this);

	BMessage* closeMsg = MessageBuilder(FABER_REMOVE_TRACK);
	closeMsg->AddUInt32("track_id", ID());

	IconButton* closeButton = new IconButton(NULL, 0, NULL,
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
				.Add(toolButton)
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
		.Add(fWaveRender)
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
AudioTrackView::UpdateScroll(float newValue, float min, float max)
{
	
}


bool
AudioTrackView::IsSelected() const
{
	return fWaveRender->IsSelected();
}


void
AudioTrackView::ZoomIn()
{
	fWaveRender->ZoomIn();
}


void
AudioTrackView::ZoomOut()
{
	fWaveRender->ZoomOut();
}


void
AudioTrackView::ZoomFull()
{
	fWaveRender->ZoomFull();

}


void
AudioTrackView::ZoomSelection()
{
	fWaveRender->ZoomSelection();
}


void
AudioTrackView::SelectAll()
{
	fWaveRender->SelectAll();
}


void
AudioTrackView::Unselect()
{
	fWaveRender->Unselect();
}


int64
AudioTrackView::Pointer() const
{
	return fWaveRender->fPointer;
}


void
AudioTrackView::CurrentSelection(int64* start, int64* end) const
{
	fWaveRender->CurrentSelection(start, end);
}


void
AudioTrackView::ScrollBy(int64 value)
{
	fWaveRender->ScrollBy(value);
}


void
AudioTrackView::Select(int64 start, int64 end)
{
	fWaveRender->Select(start, end);
}


void
AudioTrackView::SetPointer(int64 pointer)
{
	fWaveRender->fPointer = pointer;
}
