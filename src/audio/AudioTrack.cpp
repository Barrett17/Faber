/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#include "AudioTrack.h"

#include "Globals.h"


const BString&
AudioTrack::Name() const
{
	return fName;
}


void
AudioTrack::SetName(const char* name)
{
	fName.SetTo(name);
}


float
AudioTrack::Volume() const
{
	return fVolume;
}


void
AudioTrack::SetVolume(float volume)
{
	fVolume = volume;
}


float
AudioTrack::Balance() const
{
	return fBalance;
}


void
AudioTrack::SetBalance(float balance)
{
	fBalance = balance;
}


void
AudioTrack::MuteTrack(bool mute)
{
	fMuted = mute;
}


bool
AudioTrack::IsRecording() const
{
	return fRecording;
}


void
AudioTrack::SetRecording(bool recording)
{
	fRecording = recording;
}


bool
AudioTrack::IsMute() const
{
	return fMuted;
}


void
AudioTrack::SetSolo(bool solo)
{
	fSolo = solo;
}


bool
AudioTrack::IsSolo() const
{
	return fSolo;
}
