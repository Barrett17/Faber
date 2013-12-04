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

#include <Window.h>

#include <stdio.h>


WaveRender::WaveRender(AudioTrack* track)
	: 
	BView("", B_WILL_DRAW),
	fTrack(track),
	fUpdate(true),
	fPointer(0),
	fStart(0),
	fSelectionLeft(-1),
	fSelectionRight(-1)
{
	fWavePeak = new WavePeak(track);
	fEnd = fTrack->CountFrames();

	SetViewColor(60,60,60);
}	


WaveRender::~WaveRender()
{
	delete fWavePeak;
}


void
WaveRender::Draw(BRect rect)
{
	_RenderTrack(rect);
	_RenderPointer(rect);
}


void
WaveRender::_RenderTrack(BRect rect)
{
	SetDrawingMode(B_OP_ALPHA);
	SetHighColor(155,157,162);

	int32 channels = fTrack->CountChannels();

	float center = Bounds().Height()/(channels*2);

	for (int32 i = 0; i < channels; i++) {
		float* buffer = fWavePeak->Preview()->ItemAt(i);
		_RenderChannel(buffer, center);
		center += center*2;
	}
}


void
WaveRender::_RenderChannel(float* buffer, float center)
{
	int32 count = 0;

	int64 size = fWavePeak->FramesRead()/fTrack->CountChannels();

	int32 start = (int32)Bounds().left;
	int32 end = (int32)Bounds().right;

	float width = Bounds().Width();

	int64 skip = 1;

	/*if (fEnd-fStart > (end-start)) {
		skip = (fEnd-fStart)/(width);
		//skip /= 2;
	}*/

	for (int32 i = start; i < end; i++) {

		if (IsSelected() && i >= fSelectionLeft
			&& i <= fSelectionRight) {
			SetHighColor(255,255,255);
			SetLowColor(200,200,200);
		} else {
			SetHighColor(155,157,162);
		}

		float max = buffer[count];
		float min = buffer[count+1];

		BPoint point(i, center);

		BPoint point2(i, center-max*30.0f);

		BPoint point3(i, center-min*30.0f);

		StrokeLine(point, point2);
		StrokeLine(point, point3);

		count+=2*skip;

		if (count >=  fEnd-fStart || count >= size)
			break;
	}

}


void
WaveRender::_RenderPointer(BRect rect)
{
	if (IsFocus()) {
		SetHighColor(255,255,255);
		StrokeLine(BPoint(fPointer, rect.bottom),
			BPoint(fPointer, 0));
	}

	if (IsSelected()) {
		int64 fPoint = fSelectionRight;

		if(fSelectionLeft < fPointer)
			fPoint = fSelectionLeft;

		SetHighColor(255,255,255);
		StrokeLine(BPoint(fPoint, rect.bottom),
			BPoint(fPoint, 0));
	}
}


void
WaveRender::MouseDown(BPoint point)
{
	printf("WaveRender::MouseDown\n");

	MakeFocus();

	BMessage* message = Window()->CurrentMessage();
	uint32 button = 0;
	uint32 click = 0;

	message->FindInt32("buttons", (int32*)&button);
	message->FindInt32("clicks", (int32*)&click);

	if (button == B_PRIMARY_MOUSE_BUTTON) {
		fPointer = fStart+point.x;

		if (fIsSelected) {
			fIsSelected = false;
			fSelectionLeft = -1;
			fSelectionRight = -1;
			fMousePrimary = false;
		} else {
			fMousePrimary = true;
		}

		Invalidate();

	} else if (button == B_SECONDARY_MOUSE_BUTTON) {

	}
}


void
WaveRender::MouseUp(BPoint point)
{
	printf("WaveRender::MouseUp\n");

	if (fMousePrimary)
		fMousePrimary = false;
}

void
WaveRender::MouseMoved(BPoint point, uint32, const BMessage* message)
{
	printf("WaveRender::MouseMoved\n");

	if (fMousePrimary) {
		fIsSelected = true;

		int64 frame = fStart+point.x;

		if (frame < fPointer) {
			fSelectionRight = fPointer;
			fSelectionLeft = frame;
		} else if (frame > fPointer) {
			fSelectionLeft = fPointer;
			fSelectionRight = frame;
		}
		Invalidate();
	}
}

void
WaveRender::FrameResized(float width, float height)
{
}


void
WaveRender::MakeFocus(bool focused)
{
	if (focused == false) {
		fIsSelected = false;
		Invalidate();
	}

	BView::MakeFocus(focused);
}


void
WaveRender::ScrollBy(int64 value)
{
	fStart += value;
	fEnd += value;
	Invalidate();
}


void
WaveRender::Select(int64 start, int64 end)
{
	fSelectionLeft = start;
	fSelectionRight = end;
	Invalidate();
}


bool
WaveRender::IsSelected()
{
	return fIsSelected;
}


void
WaveRender::CurrentSelection(int64* start, int64* end)
{
	*start = fSelectionLeft;
	*end = fSelectionRight;
}


void
WaveRender::SelectAll()
{
	fSelectionLeft = 0;
	//fSelectionRight = 
}


void
WaveRender::Unselect()
{
	fSelectionLeft = -1;
	fSelectionRight = -1;

}


int64
WaveRender::Pointer()
{
	return fPointer;
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
