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

#include "CommandServer.h"
#include "CommandBuilder.h"
#include "FaberDefs.h"


AudioGate::AudioGate()
	:
	MediaGate()
{
	//CommandServer::AddCommandListener(this);

	fAudioEngine = new AudioEngine(this);
	BMediaRoster::Roster()->RegisterNode(fAudioEngine);

	//fInputs = new MediaEndPointMap();
	//fOutputs = new MediaEndPointMap();
}

AudioGate::~AudioGate()
{
	if (fAudioEngine != NULL) {
		BMediaRoster::Roster()->UnregisterNode(fAudioEngine);
		BMediaRoster::Roster()->ReleaseNode(fAudioEngine->Node());
	}
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


media_node
AudioGate::Node()
{
	return fAudioEngine->Node();
}


status_t
AudioGate::ConnectProducer(AudioTrack* track, const media_node& node, 
	const media_output* output, const media_format* format)
{
	return B_ERROR;
}


status_t
AudioGate::ConnectConsumer(const media_node& node, 
	const media_input* input, const media_format* format)
{
	return B_ERROR;
}
