/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include "FaberScrollBar.h"

#include <stdio.h>


FaberScrollBar::FaberScrollBar(const char* name, TracksContainer* target,
	float min, float max, orientation orientation)
	:
	BScrollBar(name, target, min, max, orientation),
	fTracksContainer(target)
{

}


FaberScrollBar::~FaberScrollBar()
{

}


void
FaberScrollBar::ValueChanged(float newValue)
{
	fTracksContainer->UpdateTracksScroll(newValue);
}
