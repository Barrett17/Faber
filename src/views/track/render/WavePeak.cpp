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

#include <stdio.h>


WavePeak::WavePeak(AudioTrack* track)
	:
	fTrack(track),
	fFrameCount(0)
{
	fPreview = new BObjectList<float>(true);
	_RenderPeaks(0, fTrack->CountFrames(), 128);
}


WavePeak::~WavePeak()
{
	delete fPreview;
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

	uint64 framesPerChannel = fTrack->CountFrames()/channels;

	for (uint32 i = 0; i < channels; i++) {
		float* buf = new float[framesPerChannel];
		//memset(buf, 0, framesPerChannel);
		fPreview->AddItem(buf);
	}

	int64 frames = 0;
	int64 timer = 0;

	float buffer[format.u.raw_audio.buffer_size 
		/ (format.u.raw_audio.format 
		& media_raw_audio_format::B_AUDIO_SIZE_MASK)];

	int chan = 0;

	while (fTrack->ReadFrames(buffer, &frames) == B_OK) {
		for (int i = 0; i < frames; i++) {
				float value = buffer[i];
				//printf("%f\n", value);
				float* buf = fPreview->ItemAt(chan);

				if (buf[fFrameCount] < value || buf[fFrameCount] == 0)
					buf[fFrameCount] = value;
	
				if (buf[fFrameCount+1] > value || buf[fFrameCount+1] == 0)
					buf[fFrameCount+1] = value;

				//printf("%f %f\n", buf[fFrameCount], buf[fFrameCount+1]);

				if (chan == channels-1)
					chan = 0;
				else
					chan++;

				timer++;

				if (timer == detail) {
					fFrameCount+=2;
					timer = 0;
					chan = 0;
				}

			}
		}
}


int64
WavePeak::FramesRead() const
{
	return fFrameCount;
}
