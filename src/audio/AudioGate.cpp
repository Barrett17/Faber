/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "AudioGate.h"

AudioGate* AudioGate::fInstance = NULL;


AudioGate::AudioGate(/*TracksManager* fTracksManager*/)
	:
	MediaGate()
{
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
	fStarted = true;

}


status_t
AudioGate::StartFrom(int64 position)
{
}


status_t
AudioGate::Stop()
{
	fStarted = false;
}


void
AudioGate::SetPause(bool pause)
{
	fStarted = !pause;
}


bool
AudioGate::IsStarted() const
{
	return fStarted;
}


bool
AudioGate::IsPaused() const
{
	return !fStarted;
}


void
AudioGate::SetLoop(bool loop)
{
	fLoop = loop;
}


bool
AudioGate::Loop() const
{
	return fLoop;
}


void
AudioGate::SetFilterHook(FilterHook* hook)
{
}


void
AudioGate::RemoveFilterHook(FilterHook* hook)
{
}


void
AudioGate::SetFramerate(float framerate)
{
}


void
AudioGate::SetFormat(media_format format)
{
	fFormat = format;
}


media_format
AudioGate::Format() const
{
	return fFormat;
}


status_t
AudioGate::Init()
{
}


status_t
AudioGate::InitNode()
{
}
