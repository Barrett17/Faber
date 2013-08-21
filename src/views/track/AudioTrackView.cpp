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
#include <StringView.h>

#include "FaberResources.h"
#include "IconButton.h"
#include "PeakView.h"
#include "VolumeSlider.h"


AudioTrackView::AudioTrackView(const char* name, AudioTrack* track,
	uint32 resizingMode)
	:
	TrackView(name, track, resizingMode),
	fTrack(track)
{
	fSampleView = new SampleView(this);
	fSampleView->Init();

	BBox* box = new BBox("box");
	box->SetExplicitMaxSize(BSize(120, 400));

	IconButton* muteButton = new IconButton(NULL, 0, NULL, NULL, this);
	// Well those could go into the constructor, but no reason for now.
	muteButton->SetToolTip(B_TRANSLATE("Mute Track"));
	muteButton->SetIcon(kMuteIcon);
	muteButton->TrimIcon();

	IconButton* recButton = new IconButton(NULL, 0, NULL, NULL, this);
	recButton->SetToolTip(B_TRANSLATE("Disable Recording"));
	recButton->SetIcon(kRecordIcon);
	recButton->TrimIcon();

	PeakView* peak = new PeakView("OutputPeakView", false, false);
	peak->SetExplicitMaxSize(BSize(200, 15));

	VolumeSlider* slider = new VolumeSlider("slider", 0, 10, 7, NULL);
	slider->SetLabel("Volume");
	
	BLayoutBuilder::Group<>(box, B_VERTICAL, 2)
		.SetInsets(10, 10, 10, 10)
		.Add(new BStringView("", "Track name"))
		.AddGroup(B_HORIZONTAL, 0)
			.Add(recButton)
			.Add(muteButton)
			.Add(new BButton("S"))
		.End()
		.Add(new BStringView("", "Input"))
		.Add(peak)
		.Add(slider)
		.Add(new BStringView("", "Balance"))
		.Add(new VolumeSlider("slider", 0, 10, 7, NULL))
		.AddGlue()
	.End();

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
		.Add(box)
		.Add(fSampleView)
	.End();
}


AudioTrackView::~AudioTrackView()
{
}


void
AudioTrackView::Invalidate()
{
	fSampleView->Invalidate();

	BView::Invalidate();
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
