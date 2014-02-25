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


AudioTrack*
TrackIO::ImportAudio(BMediaFile* mediaFile, const char* name)
{
#if 0
	TrackIndex* index = new TrackIndex();

	media_format format = SettingsManager::BuildAudioSessionFormat();
	format.buffer_size = SettingsManager::GetPreferredAudioBlockSize();

	// TODO check original file format to ask 
	// if the user want it to be resampled or not.

	int64 frames = 0;
	int64 timer = 0;

	float buffer[format.u.raw_audio.buffer_size 
		/ (format.u.raw_audio.format 
		& media_raw_audio_format::B_AUDIO_SIZE_MASK)];

	while (fTrack->ReadFrames(buffer, &frames) == B_OK) {
		MediaBlock* block = _BuildBlock(buffer, frames);
		index->AddBlock(block, true);
	}

	delete mediaFile;

	return new AudioTrack(index);
#endif
}

