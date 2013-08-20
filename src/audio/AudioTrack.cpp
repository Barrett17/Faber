/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#include "AudioTrack.h"

#include "Globals.h"
#include "PeakFile.h"


AudioTrack::AudioTrack()
{
	fPeak = new CPeakFile(this);
}


bool
AudioTrack::IsAudio() const
{
	return true;
}


void
AudioTrack::SetArea(float* area, int64 size)
{
	fArea = area;
	fSize = size;
	fEnd = fSize;
}


int32
AudioTrack::CountChannels() const
{
	return fFormat.u.raw_audio.channel_count;
}


void
AudioTrack::InitPeakFile()
{
	bool flag = false;
	if (fFormat.u.raw_audio.channel_count == 1)
		flag = true;

	fPeak->Init(fSize+1, flag);
	fPeak->CreatePeaks(0, fSize+1, fSize+1);
}


CPeakFile*
AudioTrack::PeakFile() const
{
	return fPeak;
}


bool
AudioTrack::IsMono() const
{
	return fFormat.u.raw_audio.channel_count == 1;
}


bool
AudioTrack::IsStereo() const
{
	return fFormat.u.raw_audio.channel_count == 2;
}


int64
AudioTrack::Size() const
{
	return fSize;
}


float*
AudioTrack::Area() const
{
	return fArea;
}
