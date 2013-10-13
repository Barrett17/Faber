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

#include "FaberDefs.h"
#include "FaberMessages.h"
#include "FaberResources.h"
#include "IconButton.h"
#include "PeakView.h"
#include "Shortcut.h"
#include "ToolButton.h"
#include "VolumeSlider.h"

#define HEIGHT_VAL_REF 200


AudioTrackView::AudioTrackView(const char* name, AudioTrack* track,
	uint32 resizingMode)
	:
	TrackView(name, track, resizingMode),
	fTrack(track),
	fDirty(false),
	fUpdating(false)
{
	fEnd = track->Size();

	rgb_color barColor = { 0, 200, 0 };
	rgb_color fillColor = { 240, 240, 240 };

	fSampleView = new SampleView(this);

	fSampleView->SetExplicitMinSize(BSize(200, HEIGHT_VAL_REF));
	fSampleView->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, HEIGHT_VAL_REF));

	BBox* box = new BBox("box");

	box->SetExplicitMinSize(BSize(200, HEIGHT_VAL_REF));
	box->SetExplicitMaxSize(BSize(200, HEIGHT_VAL_REF));

	IconButton* muteButton = new IconButton(NULL, 0, NULL, NULL, this);
	muteButton->SetToolTip(B_TRANSLATE("Mute Track"));
	muteButton->SetIcon(kMuteIcon);
	muteButton->TrimIcon();
	muteButton->SetValue(fTrack->IsMute());

	IconButton* recButton = new IconButton(NULL, 0, NULL, NULL, this);
	recButton->SetToolTip(B_TRANSLATE("Disable Recording"));
	recButton->SetIcon(kMiniRecordIcon);
	recButton->TrimIcon();

	IconButton* soloButton = new IconButton(NULL, 0, NULL, NULL, this);
	soloButton->SetToolTip(B_TRANSLATE("Solo"));
	soloButton->SetIcon(kSoloIcon);
	soloButton->TrimIcon();

	BString trackName(track->Name());
	if (trackName.Length() > 27) {
		trackName.Truncate(24);
		trackName.Append("...");
	}

	ToolButton* toolButton = new ToolButton(NULL, trackName.String(), NULL);
	toolButton->SetToolTip(B_TRANSLATE("Track Options"));
	toolButton->SetIcon(kMuteIcon);

	// Track menu
	BPopUpMenu* trackMenu = _BuildMenu();
	toolButton->SetMenu(trackMenu);

	trackMenu->SetTargetForItems(this);

	BMessage* msg = new BMessage(FABER_REMOVE_TRACK);
	IconButton* closeButton = new IconButton(NULL, 0, NULL, msg, this);
	closeButton->SetToolTip(B_TRANSLATE("Close"));
	closeButton->SetIcon(kCloseTrackIcon);
	closeButton->TrimIcon();

	PeakView* peak = new PeakView("OutputPeakView", false, false);
	peak->SetExplicitMaxSize(BSize(200, 15));

	VolumeSlider* volumeSlider = new VolumeSlider("volumeSlider",
		0, 10, 7, NULL);
	volumeSlider->SetToolTip(B_TRANSLATE(
		"Move vertically to set the track volume."));
	volumeSlider->SetValue(fTrack->Volume());
	volumeSlider->SetHashMarkCount(10);
	volumeSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	volumeSlider->UseFillColor(true, &fillColor);

	VolumeSlider* balanceSlider = new VolumeSlider("balanceSlider",
		0, 10, 7, NULL);
	balanceSlider->SetToolTip(B_TRANSLATE(
		"Move vertically to balance the sound."));
	balanceSlider->SetValue(fTrack->Balance());
	balanceSlider->SetHashMarkCount(10);
	balanceSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	balanceSlider->UseFillColor(true, &fillColor);

	BLayoutBuilder::Group<>(box, B_VERTICAL, 1.0f)
		.SetInsets(10, 10, 10, 10)

		.AddSplit(B_VERTICAL, 0)
			.AddGroup(B_HORIZONTAL, 0)
				.Add(closeButton)
				.AddGlue()
				.Add(toolButton)
			.End()
			.AddGroup(B_HORIZONTAL, 0)
				.Add(peak)
				.Add(recButton)
				.Add(muteButton)
				.Add(soloButton)
			.End()
		.End()

		.AddGroup(B_HORIZONTAL, 0)
				.Add(new BStringView("", "v-", B_WILL_DRAW))
				.Add(volumeSlider)
				.Add(new BStringView("", "v+", B_WILL_DRAW))
		.End()

		.AddGroup(B_HORIZONTAL, 0)
			.Add(new BStringView("", "b-", B_WILL_DRAW))
			.Add(balanceSlider)
			.Add(new BStringView("", "b+", B_WILL_DRAW))
		.End()

	.End();

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
		.Add(box)
		.Add(fSampleView)
	.End();

	printf("%lld %lld\n", fStart, fEnd);

}


AudioTrackView::~AudioTrackView()
{
}


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


BPopUpMenu*
AudioTrackView::_BuildMenu()
{
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

	trackMenu->AddItem(new BMenuItem("Create two mono tracks",
		NULL));
	trackMenu->AddItem(new BMenuItem("Separate stereo track",
		NULL));
	trackMenu->AddItem(new BMenuItem("Merge adiacent track",
		NULL));

	trackMenu->AddItem(new BSeparatorItem());

	trackMenu->AddItem(new BMenuItem(B_TRANSLATE("Insert..."),
		new BMessage(FABER_INSERT), KeyBind.GetKey("FILE_INSERT"),
		KeyBind.GetMod("FILE_INSERT")));

	trackMenu->AddItem(new BMenuItem(B_TRANSLATE("Append..."),
		new BMessage(FABER_APPEND), KeyBind.GetKey("FILE_APPEND"),
		KeyBind.GetMod("FILE_APPEND")));

	return trackMenu;
}


void
AudioTrackView::ZoomIn()
{
	printf("AudioTrackView::ZoomIn\n");

	printf("%lld %lld\n", fStart, fEnd);

	int64 base = fEnd/2;

	fStart += base;
	fEnd -= base;

	if (fStart < 0)
		fStart = 0;

	if (fEnd > fTrack->Size())
		fEnd = fTrack->Size();

	fSampleView->Invalidate();
}


void
AudioTrackView::ZoomOut()
{
	int64 base = fEnd/2;

	fStart -= base;
	fEnd += base;

	if (fStart < 0)
		fStart = 0;

	if (fEnd > fTrack->Size())
		fEnd = fTrack->Size();

	fSampleView->Invalidate();
}


void
AudioTrackView::ZoomFull()
{
	fStart = 0;
	fEnd = fTrack->Size();

	fSampleView->Invalidate();
}


void
AudioTrackView::ZoomSelection()
{
	if (!IsSelected())
		return;

	fStart = fPointer;
	fEnd = fSelectionPointer;

	fSampleView->Invalidate();
}


void
AudioTrackView::UpdateScroll(float newValue, float min, float max)
{
	if (fUpdating)
		return;

	fUpdating = true;

	if (fStart == 0 && fEnd == fTrack->Size())
		return;

	int64 size = fTrack->Size();

	int64 base = fEnd/10;

	if (fOldScroll < newValue) {

	fStart += base;
	fEnd -= base;

	if (fStart < 0)
		fStart = 0;

	if (fEnd > fTrack->Size())
		fEnd = fTrack->Size();

	if (fEnd < fStart)
		fEnd = fStart;


	} else if (fOldScroll > newValue) {

	fStart -= base;
	fEnd += base;

	if (fStart < 0)
		fStart = 0;

	if (fEnd > fTrack->Size())
		fEnd = fTrack->Size();

	if (fEnd < fStart)
		fEnd = fStart;
	}

	printf("%lld %lld\n", fStart, fEnd);

	fOldScroll = newValue;

	fSampleView->Invalidate(); 

	fUpdating = false;
}


bool
AudioTrackView::IsSelected() const
{

}
