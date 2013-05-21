/*
 * Copyright 2013 Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Dario Casalinuovo, b.vitruvio@gmail.com
 */
#include "TrackAudio.h"

#include <LayoutBuilder.h>


TrackAudio::TrackAudio(const char* name, uint32 resizingMode)
		:
		BView(name, resizingMode)
{
	fSampleView = new SampleView();
	fSampleView->Init();

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
		.Add(fSampleView)
	.End();
}


TrackAudio::~TrackAudio()
{
}


void
TrackAudio::Invalidate()
{
	fSampleView->Invalidate();

	BView::Invalidate();
}
