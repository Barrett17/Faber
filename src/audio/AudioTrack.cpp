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

#include "AudioProtocolDefs.h"
#include "FaberDefs.h"


AudioTrack::AudioTrack()
	:
	Track(),
	fTrackIndex()
{
	SetName(B_TRANSLATE("Empty Audio Track"));
}


AudioTrack::AudioTrack(const char* name, TrackIndex* index)
	:
	Track(),
	fTrackIndex(index)
{
	SetName(name);
}


AudioTrack::AudioTrack(BMessage* from)
	:
	Track(from)
{
	BMessage msg;
	from->FindMessage(SAVE_AUDIOTRACK_INDEX_NAME, &msg);
	fTrackIndex = (TrackIndex*)TrackIndex::Instantiate(&msg);
}


AudioTrack::~AudioTrack()
{
	delete fTrackIndex;
}


status_t
AudioTrack::Archive(BMessage* into, bool deep) const
{
	Track::Archive(into, deep);

	BMessage* msg = MessageBuilder(SAVE_AUDIOTRACK_INDEX, fTrackIndex);

	return into->AddMessage(SAVE_AUDIOTRACK_INDEX_NAME, msg);
}



BArchivable*
AudioTrack::Instantiate(BMessage* archive)
{
	if (!validate_instantiation(archive, "Track"))
		return NULL;

	return new AudioTrack(archive);      
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


TrackIndex*
AudioTrack::GetIndex() const
{
	return fTrackIndex;
}
