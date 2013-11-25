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

#include "TrackView.h"

#include <stdio.h>


TrackView::TrackView(const char* name,
	Track* track, uint32 resizingMode)
	:
	BGroupView(B_HORIZONTAL, resizingMode),
	fTrack(track),
	fStart(0),
	fEnd(0),
	fSelectionPointer(0),
	fPointer(0)
{
}


TrackView::~TrackView()
{
	delete fTrack;
}


Track*
TrackView::GetTrack() const
{
	return fTrack;
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


void
TrackView::SelectAll()
{

}


void
TrackView::Unselect()
{

}


bool
TrackView::IsSelected() const
{
	//return fSampleView->IsSelected();
	return false;
}


int64
TrackView::Pointer() const
{
	return fPointer;
}


int64
TrackView::SelectionPointer() const
{
	//printf("get sel pointer %lld\n", fSelectionPointer);
	return fSelectionPointer;
}


int64
TrackView::Start() const
{
	return fStart;
}


int64
TrackView::End() const
{
	return fEnd;
}


void
TrackView::SetPointer(int64 pointer)
{
	fPointer = pointer;
}


bigtime_t
TrackView::Duration() const
{
	return fTrack->Duration();
}


bool
TrackView::HasChanged() const
{
	return fTrack->HasChanged();
}


bool
TrackView::HasUndo() const
{
	return fTrack->HasUndo();
}


bool
TrackView::HasRedo() const
{
	return fTrack->HasChanged();
}


uint32
TrackView::ID() const
{
	return fTrack->ID();
}
