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

#include "AudioEffect.h"

#include "StorageUtils.h"


AudioEffect::AudioEffect(const char* name, uint32 flags)
	:
	FaberEffect(name, flags | FABER_AUDIO_EFFECT)
{
}


AudioEffect::~AudioEffect()
{
}


status_t
AudioEffect::FilterTrack(Track* track, int64 start, int64 end)
{
	// TODO investigate the need of an authomatic
	// conversion mechanism from Track to AudioTrack
	// using dynamic_casting.
	AudioTrack* audioTrack = NULL;
	if (track != NULL && track->IsAudio())
		audioTrack = (AudioTrack*) track;
	else
		return B_ERROR;

	TrackIndex* index = audioTrack->GetIndex();

	int64 frameCount = 44100;
	float buffer[StorageUtils::FramesToSize(frameCount)];

	for (uint32 i = 0; i < index->CountChannels(); i++) {
		MediaBlockMap* channel = index->ChannelAt(i);
		int64 read = 0;

		for (int64 j = start; j < end; j += read) {
			read = channel->Reader()->ReadFramesAt(buffer, j, frameCount);
			if (read < 1) {
				// TODO: Error
				break;
			}

			FilterBuffer(buffer, StorageUtils::FramesToSize(read));

			channel->Writer()->WriteFramesAt(buffer, j, read);
		}
	}

	return B_OK;
}
