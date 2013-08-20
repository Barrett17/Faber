/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "OutputAudioGate.h"


OutputAudioGate::OutputAudioGate(TracksManager* fTracksManager)
	:
	MediaGate()
{
}

OutputAudioGate::~OutputAudioGate()
{
}


status_t
OutputAudioGate::Start()
{
	fStarted = true;

	fSoundPlayer->Start();
}


status_t
OutputAudioGate::StartFrom()
{
}


status_t
OutputAudioGate::Stop()
{
	fStarted = false;
}


void
OutputAudioGate::SetLoop(bool loop)
{
}


bool
OutputAudioGate::Loop() const
{
}


void
OutputAudioGate::SetFilterHook(FilterHook* hook)
{
}


void
OutputAudioGate::RemoveFilterHook(FilterHook* hook)
{
}


void
OutputAudioGate::SetFramerate(float framerate)
{
}


void
OutputAudioGate::SetFormat(media_format format)
{
}


media_format
OutputAudioGate::Format() const
{
	return fFormat;
}

void 
OutputAudioGate::_Init()
{
}
