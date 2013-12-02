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

#include "WaveRender.h"

#include <stdio.h>


WaveRender::WaveRender(AudioTrack* track)
	: 
	BView("", B_WILL_DRAW),
	fTrack(track),
	fUpdate(true)
{
	fWavePeak = new WavePeak(track);
	SetViewColor(60,60,60);
}	


void
WaveRender::Draw(BRect rect)
{
	float width = Bounds().Width();

	SetDrawingMode(B_OP_ADD);
	SetHighColor(55,57,62);

	int32 start = 0;
	int32 channels = fTrack->CountChannels();

	float center = Bounds().Height()/(channels*2);

	for (int32 i = 0; i < channels; i++) {
		printf("%d\n", i);
		float* buffer = fWavePeak->Preview()->ItemAt(i);
		_RenderChannel(buffer, center, 10.0f);
		center += center*2;
	}
}


void
WaveRender::_RenderChannel(float* buffer, float center, float max)
{
	int32 count = 0;

	int32 size = fWavePeak->FramesRead()/fTrack->CountChannels();

	for (int32 i = 0; i < size; i++) {
		float max = buffer[count];
		float min = buffer[count+1];

		BPoint point(i, 
			center);

		BPoint point2(i, 
			center-max*50);

		//printf("%f\n", max);

		BPoint point3(i, 
			center-min*50);

		StrokeLine(point, point2);
		StrokeLine(point, point3);

		count+=2;

		if (count >= size)
			break;

	}

}


void
WaveRender::MouseDown(BPoint point)
{

}


void
WaveRender::MouseUp(BPoint point)
{

}

void
WaveRender::MouseMoved(BPoint point, uint32, const BMessage* message)
{

}

void
WaveRender::FrameResized(float width, float height)
{

}

void
WaveRender::RedrawRequested()
{

}
void
WaveRender::UpdateRequested(BRect rect)
{

}

void
WaveRender::ScrollBy(int64 value)
{

}

bool
WaveRender::IsSelected()
{

}
void
WaveRender::CurrentSelection(int64 start, int64 end)
{

}

int64
WaveRender::Pointer()
{

}


void
WaveRender::ZoomIn()
{

}


void
WaveRender::ZoomOut()
{

}


void
WaveRender::ZoomFull()
{

}


void
WaveRender::ZoomSelection()
{

}		


void
WaveRender::_Render()
{

}


void
WaveRender::_UpdatePeaks()
{

}
