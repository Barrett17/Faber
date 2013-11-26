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
	fMediaFile(NULL)
{
	SetName(B_TRANSLATE("Empty Audio Track"));
}


AudioTrack::AudioTrack(BMediaFile* mediaFile)
	:
	Track(),
	fInitCheck(true),
	fMediaFile(mediaFile)
{
	fMediaTrack = fMediaFile->TrackAt(0);
	if (fMediaTrack == NULL) {
		fInitCheck = B_ERROR;
	}

	fMediaTrack->DecodedFormat(&fFormat);
}


AudioTrack::~AudioTrack()
{
	delete fMediaFile;
}


void
AudioTrack::SetFormat(media_format format)
{
	fFormat.SpecializeTo(&format);
}


media_format
AudioTrack::Format() const
{
	return fFormat;
}


bool
AudioTrack::IsAudio() const
{
	return true;
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
AudioTrack::Frames() const
{

}


BMediaFile*
AudioTrack::MediaFile() const
{
	return fMediaFile;
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
