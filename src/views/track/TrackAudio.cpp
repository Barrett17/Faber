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
	fIndexView = new IndexView();
	fIndexView->SetExplicitMaxSize(BSize(1500, 20));

	fSampleView = new SampleView();
	fSampleView->Init();

	fValuesView = new ValuesView();

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
		.AddGroup(B_VERTICAL, 0)
			.Add(fIndexView)
			.Add(fSampleView)
		.End()
		//.Add(fValuesView)
	.End();
}

TrackAudio::~TrackAudio()
{
}


void
TrackAudio::Invalidate()
{
	fIndexView->Invalidate();
	fSampleView->Invalidate();
	fValuesView->Invalidate();

	BView::Invalidate();
}
