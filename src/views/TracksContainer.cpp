/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include "TracksContainer.h"

#include "TrackView.h"
#include "AudioTrackView.h"


TracksContainer::TracksContainer()
	:
	BGroupView(B_VERTICAL, 0),
	fTrackViews(false)
{
	fTimeBar = new TimeBarView();
	GroupLayout()->AddView(fTimeBar);

	AddTrack(new AudioTrackView());
	//AddTrack(new AudioTrackView());
}


TracksContainer::~TracksContainer()
{
}


void
TracksContainer::Invalidate()
{
	for (int i = 0; i < fTrackViews.CountItems(); i++) {
		TrackAt(i)->Invalidate();
	}
	fTimeBar->Invalidate();

	BView::Invalidate();
}


void
TracksContainer::Pulse()
{
	for (int i = 0; i < fTrackViews.CountItems(); i++) {
		TrackAt(i)->Pulse();
	}
	fTimeBar->Pulse();
}


status_t
TracksContainer::AddTrack(TrackView* track, int32 index)
{
	GroupLayout()->AddView(track);
	return fTrackViews.AddItem(track, index);
}


status_t
TracksContainer::RemoveTrack(TrackView* track)
{
	GroupLayout()->RemoveView(track);
	return fTrackViews.RemoveItem(track);
}


TrackView*
TracksContainer::TrackAt(int32 index) const
{
	return fTrackViews.ItemAt(index);
}


int32
TracksContainer::CountTracks() const
{
	return fTrackViews.CountItems();
}


TrackView*
TracksContainer::CurrentTrack()
{
	
}
