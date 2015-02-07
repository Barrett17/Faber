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

#include "AudioProtocolDefs.h"
#include "MessageBuilder.h"
#include "Track.h"
#include "MediaAgent.h"

AudioGate* MediaAgent::fAudioGate = new AudioGate();
uint32 MediaAgent::fLastID = 0;
BObjectList<Track> MediaAgent::fTracks;

AudioGate*
MediaAgent::GetAudioGate()
{
	return fAudioGate;
}


status_t
MediaAgent::RegisterTrack(Track* track)
{
	fLastID++;

	track->SetID(fLastID);

	fTracks.AddItem(track);

	return B_OK;
}


status_t
MediaAgent::UnregisterTrack(Track* track)
{
	fTracks.RemoveItem(track);
	delete track;

	return B_OK;
}


BMessage*
MediaAgent::ArchiveTracks()
{
	BMessage* msg = new BMessage();

	for (int i = 0; i < fTracks.CountItems(); i++) {
		BMessage* track = MessageBuilder(SAVE_AUDIOTRACK);
		fTracks.ItemAt(i)->Archive(track);
		msg->AddMessage(SAVE_AUDIOTRACK_NAME, track);
	}

	return msg;
}


status_t
MediaAgent::UnarchiveTracks(BMessage* from)
{
	BMessage msg;
	for (int i = 0; from->FindMessage(SAVE_AUDIOTRACK_NAME, i, &msg)
		== B_OK; i++) {
		AudioTrack* track = (AudioTrack*)AudioTrack::Instantiate(&msg);
		RegisterTrack(track);
	}

	return B_OK;
}
