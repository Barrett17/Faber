/*
 * Copyright 2013 Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Dario Casalinuovo, b.vitruvio@gmail.com
 */
#include "TrackAudio.h"

#include <LayoutBuilder.h>

#include "ValuesView.h"
#include "SampleView.h"
#include "IndexView.h"


TrackAudio::TrackAudio(const char* name, uint32 resizingMode)
		:
		BView(name, resizingMode)
{
	IndexView* indexView = new IndexView();
	indexView->SetExplicitMaxSize(BSize(1500, 20));

	SampleView* sampleView = new SampleView();
	sampleView->Init();
	//sampleView->SetExplicitMaxSize(BSize(400, 600));

	ValuesView* valuesView = new ValuesView();
	//valuesView->SetExplicitMaxSize(BSize(20, 1000));

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
		//.Add(valuesView)
		.AddGroup(B_VERTICAL, 0)
			.Add(indexView)
			.Add(sampleView)
		.End()
	.End();
}

TrackAudio::~TrackAudio()
{
}
