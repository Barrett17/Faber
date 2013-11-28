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
	fMediaFile(mediaFile)
{
	fFormat.type = B_MEDIA_RAW_AUDIO;
	fFormat.u.raw_audio.format = media_raw_audio_format::B_AUDIO_FLOAT;
	fFormat.u.raw_audio.frame_rate = 44100;
	fFormat.u.raw_audio.byte_order = 
		(B_HOST_IS_BENDIAN) ? B_MEDIA_BIG_ENDIAN : B_MEDIA_LITTLE_ENDIAN;

	fMediaTrack = fMediaFile->TrackAt(0);
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


status_t
AudioTrack::ReadFrames(void* buffer, int64* frameCount,
	media_header* header)
{
	return fMediaTrack->ReadFrames(buffer, frameCount, header);
}


status_t
AudioTrack::ReplaceFrames(const void* buffer,
int64* frameCount, const media_header* header)
{
	return fMediaTrack->ReplaceFrames(buffer, frameCount, header);
}


status_t
AudioTrack::SeekToTime(bigtime_t* time, int32 flags)
{
	return fMediaTrack->SeekToTime(time, flags);
}

status_t
AudioTrack::SeekToFrame(int64* frame, int32 flags)
{
	return fMediaTrack->SeekToTime(frame, flags);
}


status_t
AudioTrack::WriteFrames(const void* data, int32 frameCount,
	int32 flags)
{
	return fMediaTrack->WriteFrames(data, frameCount, flags);
}


int64
AudioTrack::CountFrames() const
{
	return fMediaTrack->CountFrames();
}


status_t
AudioTrack::InitCheck() const
{
	return fMediaTrack->InitCheck();
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
