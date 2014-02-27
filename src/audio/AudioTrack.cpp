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
	Track(),
	fTrackIndex()
{
	SetName(B_TRANSLATE("Empty Audio Track"));
}


AudioTrack::AudioTrack(TrackIndex* index)
	:
	Track(),
	fTrackIndex(index)
{
}


AudioTrack::~AudioTrack()
{
	delete fTrackIndex;
}


bool
AudioTrack::IsAudio() const
{
	return true;
}


int32
AudioTrack::CountChannels() const
{
	return fTrackIndex->CountChannels();
}


bool
AudioTrack::IsMono() const
{
	return CountChannels() == 1;
}


bool
AudioTrack::IsStereo() const
{
	return CountChannels() == 2;
}


status_t
AudioTrack::InitCheck() const
{
	return fTrackIndex->InitCheck();
}
