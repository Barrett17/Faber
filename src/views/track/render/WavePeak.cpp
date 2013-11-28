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

#include "WavePeak.h"


WavePeak::WavePeak(AudioTrack* track)
	:
	fTrack(track)
{
	fPreview = new BObjectList<float>(true);
	_RenderPeaks(0, fTrack->CountFrames(), 256);
}


BObjectList<float>*
WavePeak::Preview()
{
	return fPreview;
}



void
WavePeak::UpdateRequested(int64 start, int64 end)
{

}



void
WavePeak::_RenderPeaks(int64 start, int64 end, int64 detail)
{
	media_format format = fTrack->Format();

	uint32 channels = fTrack->CountChannels();

	uint64 framesPerChannel = ((fTrack->CountFrames()/detail)/channels)*2;

	for (uint32 i = 0; i < channels; i++) {
		float* buf = new float[framesPerChannel*2];
		fPreview->AddItem(buf);
	}

	int64 frames = 0;
	float buffer[format.u.raw_audio.buffer_size 
		/ (format.u.raw_audio.format 
		& media_raw_audio_format::B_AUDIO_SIZE_MASK)];

	int64 timer = 0;

	while (fTrack->ReadFrames(buffer, &frames) == B_OK) {
		for (int i = 0; i < frames; i+=channels) {
			float max[channels];
			float min[channels];

			memset(&max, -1, channels);
			memset(&min, -1, channels);

			timer+=channels;
			for (int j = frames; j < frames+channels; j++) {
				float value = buffer[j];
	
				if (max[i] < value || max[i] == -1)
					max[i] = value;
	
				if (min[i] > value || min[i] == -1)
					min[i] = value;
			}

			if (timer >= detail) {
				timer = 0;
				fFrameCount+=2;

				for (uint32 i = 0; i < channels; i++) {
					float* buf = fPreview->ItemAt(i);
					buf[fFrameCount] = max[i];
					buf[fFrameCount+1] = min[i];
					max[i] = -1;
					min[i] = -1;
				}
			}

		}
	}
}


int64
WavePeak::FramesRead() const
{
	return fFrameCount;
}
