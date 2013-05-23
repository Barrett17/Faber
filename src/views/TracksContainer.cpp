/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include "TracksContainer.h"

#include <LayoutBuilder.h>

#include "TrackView.h"
#include "AudioTrackView.h"


TracksContainer::TracksContainer()
	:
	BView("TracksContainer", B_FOLLOW_ALL)
{
	fTrackView = new AudioTrackView();
	fTimeBar = new TimeBarView();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
			.Add(fTimeBar)
			.Add(fTrackView)
	.End();
}


TracksContainer::~TracksContainer()
{
}


void
TracksContainer::Invalidate()
{
	fTrackView->Invalidate();
	fTimeBar->Invalidate();

	BView::Invalidate();
}


void
TracksContainer::Pulse()
{
	fTrackView->Pulse();
	fTimeBar->Pulse();
}
