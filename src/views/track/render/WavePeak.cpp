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
	_RenderPeaks(0, fTrack->Frames(), 512);
}


BObjectList<BPositionIO>&
WavePeak::PreviewRequested()
{

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

	if (fPreview.CountItems() > 0)
		fPreview.MakeEmpty();

	_InitPreview(channels);

	for (int64 frame = start; frame < end; frame+=detail) {
		for (int c = 0; c < channels; c++) {
			BPositionIO* buf = fPreview.ItemAt(c);
			
		}
	}
}


void
WavePeak::_InitPreview(uint32 channels)
{

}
