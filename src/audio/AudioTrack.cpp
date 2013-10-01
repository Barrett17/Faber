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


status_t
AudioTrack::ApplyEffect(int32 id, int64 start, int64 end)
{

}
