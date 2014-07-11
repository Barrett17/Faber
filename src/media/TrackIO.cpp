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

#include "TrackIO.h"

#include <MediaFile.h>

#include "FaberDefs.h"
#include "MediaFormatBuilder.h"

#include <stdio.h>


AudioTrack*
TrackIO::ImportAudio(BMediaFile* mediaFile, const char* name)
{
	TrackIndex* index = new TrackIndex();

	media_format format;

	MediaFormatBuilder::BuildAudioBlockRawFormat(&format);

	// TODO check original file format to ask 
	// if the user want it to be resampled or not.
	// NOTE atm we are considering only the first track, may and probably will
	// change in future, or at least the user should be able to select which track
	// to import (eventually all and as separated track).
	BMediaTrack* track = mediaFile->TrackAt(0);

	if (track->DecodedFormat(&format) != B_OK) {
		// Critical error show a popup
		return NULL;
	}

	for (uint32 i = 0; i < format.u.raw_audio.channel_count; i++) {
		MediaBlockMap* channel = new MediaBlockMap();
		index->AddChannel(channel);
	}

	WindowsManager::ProgressUpdate(10.0f, "Loading audio data");

	int64 frames = 0;

	size_t size = (format.u.raw_audio.buffer_size 
		/ (format.u.raw_audio.format 
		& media_raw_audio_format::B_AUDIO_SIZE_MASK))*2;

	float buffer[size];

	while (track->ReadFrames(buffer, &frames) == B_OK) {
		_BuildBlocks((float*)buffer, size, index,
			format.u.raw_audio.channel_count);
	}

	WindowsManager::ProgressUpdate(50.0f, "Flushing data and preview");

	mediaFile->ReleaseTrack(track);

	// This is done to optimize writes, it currently only write
	// the preview, in future the whole data will have a caching mechanism.
	for (uint32 j = 0; j < index->CountChannels(); j++)
		index->GetChannels().ItemAt(j)->Writer()->Flush();

	return new AudioTrack(name, index);
}


status_t
TrackIO::_BuildBlocks(float* buffer, size_t size, TrackIndex* index,
	uint32 channels)
{
	BObjectList<MediaBlockMap> trackChannels = index->GetChannels();

	float temp[channels][size/channels];

	int64 count = 0;

	for (size_t i = 0; i < size;) {
		for (uint32 j = 0; j < channels; j++) {
			temp[j][count] = buffer[i];
			i++;
		}
		count++;
	}

	for (uint32 j = 0; j < channels; j++) {
		trackChannels.ItemAt(j)->Writer()->WriteFrames(
			(void*)&temp[j][0], size/channels);
	}

	return B_OK;
}
