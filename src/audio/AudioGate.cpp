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

#include "AudioProtocolDefs.h"
#include "CommandServer.h"
#include "CommandBuilder.h"
#include "FaberDefs.h"

AudioGate* AudioGate::fInstance = NULL;


AudioGate::AudioGate()
	:
	MediaGate(),
	fLastID(-1)
{
	CommandServer::AddCommandListener(this);

	fAudioEngine = new AudioEngine(this);

	BMediaRoster::Roster()->RegisterNode(fAudioEngine);

	//fInputs = new MediaEndPointMap();
	//fOutputs = new MediaEndPointMap();
}

AudioGate::~AudioGate()
{
	BMediaRoster::Roster()->UnregisterNode(fAudioEngine);
	BMediaRoster::Roster()->ReleaseNode(fAudioEngine->Node());
}


AudioGate*
AudioGate::Get()
{
	if (fInstance == NULL)
		fInstance = new AudioGate();

	return fInstance;	
}


status_t
AudioGate::HandleCommand(BMessage* message)
{
	//message->PrintToStream();

	switch (message->what)
	{
		default:
			return B_ERROR;
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


BMessage*
AudioGate::ArchiveTracks()
{
	BMessage* msg = new BMessage();

	for (int i = 0; i < Get()->fTracks.CountItems(); i++) {
		BMessage* track = CommandBuilder(SAVE_AUDIOTRACK);
		Get()->fTracks.ItemAt(i)->Archive(track);
		msg->AddMessage(SAVE_AUDIOTRACK_NAME, track);
	}

	return msg;
}


status_t
AudioGate::UnarchiveTracks(BMessage* from)
{
	BMessage msg;
	for (int i = 0; from->FindMessage(SAVE_AUDIOTRACK_NAME, i, &msg)
		== B_OK; i++) {
		AudioTrack* track = (AudioTrack*)AudioTrack::Instantiate(&msg);
		RegisterTrack(track);
	}

	return B_OK;
}

