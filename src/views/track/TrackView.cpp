/*
 * Copyright 2013 Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Dario Casalinuovo, b.vitruvio@gmail.com
 */
#include "TrackView.h"

#include <LayoutBuilder.h>


TrackView::TrackView(const char* name, uint32 resizingMode)
	:
	BView(name, resizingMode)
{
}


TrackView::~TrackView()
{
}


const BString&
TrackView::Name() const
{
	return fName;
}


void
TrackView::SetName(const char* name)
{
	fName.SetTo(name);
}


float
TrackView::Volume() const
{
	return fVolume;
}


void
TrackView::SetVolume(float volume)
{
	fVolume = volume;
}



void
TrackView::MuteTrack(bool mute)
{
	fMuted = mute;
}


bool
TrackView::IsMute() const
{
	return fMuted;
}


void
TrackView::SetSolo(bool solo)
{
	fSolo = solo;
}


bool
TrackView::IsSolo() const
{
	return fSolo;
}
