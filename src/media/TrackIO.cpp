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

#include "MediaFormatBuilder.h"
#include "TrackIO.h"


AudioTrack*
TrackIO::ImportAudio(BMediaFile* mediaFile, const char* name)
{
	TrackIndex* index = new TrackIndex();

	media_format format;

	MediaFormatBuilder::BuildAudioBlockFormat(&format);

	// TODO check original file format to ask 
	// if the user want it to be resampled or not.
	// NOTE atm we are considering only the first track, may and probably will
	// change in future, or at least the user should be able to select which track
	// to import (eventually all).
	BMediaTrack* track = mediaFile->TrackAt(0);

	if (track->DecodedFormat(&format) != B_OK) {
		// Critical error show a popup
		return NULL;
	}

	for (uint32 i = 0; i < format.u.raw_audio.channel_count; i++) {
		MediaBlockTree* tree = new MediaBlockTree();
		index->AddChannel(tree);
	}

	int64 frames = 0;

	float buffer[format.u.raw_audio.buffer_size 
		/ (format.u.raw_audio.format 
		& media_raw_audio_format::B_AUDIO_SIZE_MASK)];

	while (track->ReadFrames(buffer, &frames) == B_OK)
		_BuildBlocks(buffer, frames, index, format.u.raw_audio.channel_count);

	AudioTrack* ret = new AudioTrack(index);
	// TODO Consider adding it in the constructor.
	ret->SetName(name);
 
	return ret;
}


status_t
TrackIO::_BuildBlocks(void* buffer, int64 frames, TrackIndex* index,
	uint32 channels)
{

	return B_OK;
}
