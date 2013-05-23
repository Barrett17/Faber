/*
 * Copyright 2013 Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Dario Casalinuovo, b.vitruvio@gmail.com
 */
#include "AudioTrackView.h"

#include <LayoutBuilder.h>


AudioTrackView::AudioTrackView(const char* name, uint32 resizingMode)
	:
	TrackView(name, resizingMode)
{
	fSampleView = new SampleView();
	fSampleView->Init();

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
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
