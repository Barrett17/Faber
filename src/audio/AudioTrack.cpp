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

#include "FaberDefs.h"


AudioTrack::AudioTrack()
	:
	Track()
{
	SetName(B_TRANSLATE("Empty Audio Track"));
}


AudioTrack::AudioTrack(BMediaFile* mediaFile)
	:
	Track(),
	fInitCheck(true)
{
	//SetName(B_TRANSLATE("Empty Audio Track"));

	//fMediaFile = mediaFile;
}


AudioTrack::~AudioTrack()
{
	delete fArea;
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


status_t
AudioTrack::InitCheck() const
{
	return fInitCheck;
}
