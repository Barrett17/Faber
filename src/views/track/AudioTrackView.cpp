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
#include <Button.h>
#include <LayoutBuilder.h>
#include <PopUpMenu.h>
#include <StringView.h>

#include "FaberResources.h"
#include "IconButton.h"
#include "PeakView.h"
#include "Shortcut.h"
#include "ToolButton.h"
#include "VolumeSlider.h"

#define HEIGHT_VAL_REF 220


AudioTrackView::AudioTrackView(const char* name, AudioTrack* track,
	uint32 resizingMode)
	:
	TrackView(name, track, resizingMode),
	fTrack(track)
{
	fSampleView = new SampleView(this);
	fSampleView->Init();

	fSampleView->SetExplicitMinSize(BSize(200, HEIGHT_VAL_REF));
	fSampleView->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, HEIGHT_VAL_REF));

	BBox* box = new BBox("box");
	
	box->SetExplicitMinSize(BSize(200, HEIGHT_VAL_REF));
	box->SetExplicitMaxSize(BSize(200, HEIGHT_VAL_REF));

	IconButton* muteButton = new IconButton(NULL, 0, NULL, NULL, this);
	// Well those could go into the constructor, but no reason for now.
	muteButton->SetToolTip(B_TRANSLATE("Mute Track"));
	muteButton->SetIcon(kMuteIcon);
	muteButton->TrimIcon();
	muteButton->SetValue(fTrack->IsMute());

	IconButton* recButton = new IconButton(NULL, 0, NULL, NULL, this);
	recButton->SetToolTip(B_TRANSLATE("Disable Recording"));
	recButton->SetIcon(kRecordIcon);
	recButton->TrimIcon();

	IconButton* soloButton = new IconButton(NULL, 0, NULL, NULL, this);
	soloButton->SetToolTip(B_TRANSLATE("Disable Recording"));
	soloButton->SetIcon(kSoloIcon);
	soloButton->TrimIcon();


	ToolButton* toolButton = new ToolButton(NULL, track->Name(), NULL);
	recButton->SetToolTip(B_TRANSLATE("Track Options"));
	toolButton->SetIcon(kMuteIcon);

	// Track menu
	BPopUpMenu* trackMenu = new BPopUpMenu("TrackMenu");

	trackMenu->AddItem(new BMenuItem("Set Name...",
		NULL));

	trackMenu->AddItem(new BSeparatorItem());

	trackMenu->AddItem(new BMenuItem("Mono",
		NULL));

	trackMenu->AddItem(new BMenuItem("Stereo (right)",
		NULL));

	trackMenu->AddItem(new BMenuItem("Stereo (left)",
		NULL));

	trackMenu->AddItem(new BSeparatorItem());

	trackMenu->AddItem(new BMenuItem("Select All",
		NULL));

	trackMenu->AddItem(new BSeparatorItem());

	trackMenu->AddItem(new BMenuItem("Edit Left",
		NULL));

	trackMenu->AddItem(new BMenuItem("Edit Right",
		NULL));

	trackMenu->AddItem(new BMenuItem("Edit Both",
		NULL));

	trackMenu->AddItem(new BSeparatorItem());

	trackMenu->AddItem(new BMenuItem(B_TRANSLATE("Insert..."), new BMessage(INSERT),
		KeyBind.GetKey("FILE_INSERT"), KeyBind.GetMod("FILE_INSERT")));

	trackMenu->AddItem(new BMenuItem(B_TRANSLATE("Append..."), new BMessage(APPEND),
		KeyBind.GetKey("FILE_APPEND"), KeyBind.GetMod("FILE_APPEND")));

	toolButton->SetMenu(trackMenu);

	trackMenu->SetTargetForItems(this);

	IconButton* closeButton = new IconButton(NULL, 0, NULL, NULL, this);
	closeButton->SetToolTip(B_TRANSLATE("Close"));
	closeButton->SetIcon(kCloseTrackIcon);
	closeButton->TrimIcon();

	PeakView* peak = new PeakView("OutputPeakView", false, false);
	peak->SetExplicitMaxSize(BSize(200, 15));

	VolumeSlider* volumeSlider = new VolumeSlider("volumeSlider",
		0, 10, 7, NULL);
	volumeSlider->SetLabel("Volume");
	volumeSlider->SetToolTip(B_TRANSLATE(
		"Move vertically to set the track volume."));
	volumeSlider->SetValue(fTrack->Volume());

	VolumeSlider* balanceSlider = new VolumeSlider("balanceSlider",
		0, 10, 7, NULL);
	balanceSlider->SetLabel("Balance");
	balanceSlider->SetToolTip(B_TRANSLATE(
		"Move vertically to balance the sound."));
	balanceSlider->SetValue(fTrack->Balance());

	BLayoutBuilder::Group<>(box, B_VERTICAL, 2)
		.SetInsets(10, 10, 10, 10)

		.AddSplit(B_VERTICAL, 1.0f)

			.AddGroup(B_HORIZONTAL, 0)
				//.Add(closeButton)
				.Add(toolButton)
				.AddStrut(1.0f)
				.Add(peak)
			.End()

			.AddSplit(B_HORIZONTAL, 0)
					.Add(recButton)
					.Add(muteButton)
					.Add(soloButton)
			.End()

			.Add(volumeSlider)
			.Add(balanceSlider)

		.End()
	.End();

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
		.Add(box)
		.Add(fSampleView)
	.End();
}


AudioTrackView::~AudioTrackView()
{
}

/*
void
AudioTrackView::Invalidate()
{
	//fSampleView->Invalidate();

	BView::Invalidate();
}
*/

void
AudioTrackView::Pulse()
{
	fSampleView->Pulse();
}


AudioTrack*
AudioTrackView::Track() const
{
	return fTrack;
}


void
AudioTrackView::SetUpdateDrawCache(bool value)
{
	fUpdateDrawCache = value;
}


bool
AudioTrackView::UpdateDrawCache() const
{
	return fUpdateDrawCache;
}


void
AudioTrackView::SetDirty(bool dirty)
{
	fDirty = dirty;
}


bool
AudioTrackView::Dirty() const
{
	return fDirty;
}
