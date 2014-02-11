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

#include "AudioGate.h"

AudioGate* AudioGate::fInstance = NULL;

AudioGate::AudioGate()
	:
	MediaGate(),
	fLastID(-1)
{
	fAudioEngine = new AudioEngine(this);

	fRoster = BMediaRoster::Roster();
	fRoster->RegisterNode(fAudioEngine);

	//fInputs = new MediaEndPointMap();
	//fOutputs = new MediaEndPointMap();
}

AudioGate::~AudioGate()
{
}


AudioGate*
AudioGate::Get()
{
	if (fInstance == NULL)
		fInstance = new AudioGate();

	return fInstance;	
}


status_t
AudioGate::Start()
{

	return MediaGate::Start();
}


status_t
AudioGate::Stop()
{

	return MediaGate::Stop();
}


const MediaEndPointMap&
AudioGate::GetInputs()
{
	//return *fInputs;
}


const MediaEndPointMap&
AudioGate::GetOutputs()
{
	//return *fOutputs;
}


status_t
AudioGate::RegisterTrack(Track* track)
{
	if (track->IsAudio())
		return RegisterTrack((AudioTrack*)track);

	return B_ERROR;
}


status_t
AudioGate::UnregisterTrack(Track* track)
{
	if (track->IsAudio())
		return UnregisterTrack((AudioTrack*)track);

	return B_ERROR;
}


status_t
AudioGate::RegisterTrack(AudioTrack* track)
{
	Get()->fLastID++;

	track->SetID(Get()->fLastID);

	Get()->fTracks.AddItem(track);

	return B_OK;
}


status_t
AudioGate::UnregisterTrack(AudioTrack* track)
{
	Get()->fTracks.RemoveItem(track);
	delete track;

	return B_OK;
}


media_node
AudioGate::Node()
{
	return fAudioEngine->Node();
}
