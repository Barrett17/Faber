/*
 * Copyright 2013 Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Dario Casalinuovo, b.vitruvio@gmail.com
 */
#include "TrackView.h"


TrackView::TrackView(const char* name,
	Track* track, uint32 resizingMode)
	:
	BView(name, resizingMode),
	fTrack(track)
{
	SetExplicitMaxSize(BSize(120, 80));
}


TrackView::~TrackView()
{
}


const BString&
TrackView::Name() const
{
	return fTrack->Name();
}


void
TrackView::SetName(const char* name)
{
	fTrack->SetName(name);
}


float
TrackView::Volume() const
{
	return fTrack->Volume();
}


void
TrackView::SetVolume(float volume)
{
	fTrack->SetVolume(volume);
}


float
TrackView::Balance() const
{
	return fTrack->Balance();
}


void
TrackView::SetBalance(float balance)
{
	fTrack->SetBalance(balance);
}


void
TrackView::MuteTrack(bool mute)
{
	fTrack->MuteTrack(mute);
}


bool
TrackView::IsPlaying() const
{
	return fTrack->IsPlaying();
}


void
TrackView::SetPlaying(bool playing)
{
	fTrack->SetPlaying(playing);
}


bool
TrackView::IsRecording() const
{
	return fTrack->IsRecording();
}


void
TrackView::SetRecording(bool recording)
{
	fTrack->SetRecording(recording);
}


bool
TrackView::IsMute() const
{
	return fTrack->IsMute();
}


void
TrackView::SetSolo(bool solo)
{
	fTrack->SetSolo(solo);
}


bool
TrackView::IsSolo() const
{
	return fTrack->IsSolo();
}
