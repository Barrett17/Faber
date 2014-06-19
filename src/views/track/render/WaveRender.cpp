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

#include "FaberDefs.h"
#include "TracksCommon.h"

#include <stdio.h>


WaveRender::WaveRender(AudioTrack* track)
	: 
	BView("", B_WILL_DRAW),
	fTrack(track),
	fUpdate(true),
	fPointer(0),
	fStart(0),
	fSelectionStart(-1),
	fSelectionEnd(-1),
	fZoomFactor(20)
{
	fEnd = GetCoords()->start/fTrack->CountChannels();

	SetViewColor(60,60,60);
}	


WaveRender::~WaveRender()
{
}


void
WaveRender::Draw(BRect rect)
{
	_RenderTrack(rect);
	_RenderPointers(rect);
}


void
WaveRender::_RenderTrack(BRect rect)
{
	SetDrawingMode(B_OP_ALPHA);
	SetHighColor(155,157,162);

	TrackIndex* index = fTrack->GetIndex();
	int32 channels = index->CountChannels();

	float center = Bounds().Height()/(channels*2);

	for (int32 i = 0; i < channels; i++) {
		MediaBlockMap* channelMap = index->GetChannel(i);
		_RenderChannel(channelMap, center);
		center += center*2;
	}
}


void
WaveRender::_RenderChannel(MediaBlockMap* map, float center)
{
	/*int64 end = Bounds().right;

	printf("%ld %ld %ld\n", fStart, fEnd);

	int64 count = _FrameToScreen(fStart);

	for (int64 i = 0; i < end; i++) {

		if (IsSelected() && i >= _FrameToScreen(fSelectionStart)
			&& i <= _FrameToScreen(fSelectionEnd)) {
			SetHighColor(255,255,255);
			SetLowColor(200,200,200);
		} else {
			SetHighColor(155,157,162);
		}

		float max = buffer[count];
		float min = buffer[count+1];

		BPoint pointMin(i, center-max*30.0f);
		BPoint pointMax(i, center-min*30.0f);

		StrokeLine(pointMin, pointMax);

		count+=2*fZoomFactor;
	}*/

}


void
WaveRender::_RenderPointers(BRect rect)
{
	int64 pointer = fPointer/128;

	if (IsFocus()) {
		SetHighColor(255,255,255);
		StrokeLine(BPoint(pointer, rect.bottom),
			BPoint(pointer, 0));
	}

	if (IsSelected()) {
		printf("rendering\n");
		int64 point;

		if(fSelectionStart < fPointer)
			point = _FrameToScreen(fSelectionStart);
		else
			point = _FrameToScreen(fSelectionEnd);

		SetHighColor(255,255,255);
		StrokeLine(BPoint(point, rect.bottom),
			BPoint(point, 0));
	}
}


void
WaveRender::MouseDown(BPoint point)
{
	//printf("WaveRender::MouseDown\n");

	MakeFocus();

	BMessage* message = Window()->CurrentMessage();
	uint32 button = 0;
	uint32 click = 0;

	message->FindInt32("buttons", (int32*)&button);
	message->FindInt32("clicks", (int32*)&click);

	if (button == B_PRIMARY_MOUSE_BUTTON) {
		fPointer = fStart+_ScreenToFrame(point.x);

		if (fIsSelected) {
			fIsSelected = false;
			fSelectionStart = -1;
			fSelectionEnd = -1;
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
	//printf("WaveRender::MouseUp\n");

	if (fMousePrimary)
		fMousePrimary = false;
}

void
WaveRender::MouseMoved(BPoint point, uint32, const BMessage* message)
{
	//printf("WaveRender::MouseMoved\n");

	if (fMousePrimary) {
		fIsSelected = true;

		int64 frame = fStart+_ScreenToFrame(point.x);

		if (frame < fPointer)
			Select(frame, fPointer);
		else if (frame > fPointer)
			Select(fPointer, frame);

	}
}


void
WaveRender::FrameResized(float width, float height)
{
}


void
WaveRender::MakeFocus(bool focused)
{
	if (focused == false)
		Unselect();

	BView::MakeFocus(focused);
}


void
WaveRender::ScrollBy(int64 value)
{
	if (fEnd+value > TrackEnd()
		|| fStart+value < TrackStart())
		return;

	fStart += value;
	fEnd += value;

	Invalidate();
}


void
WaveRender::Select(int64 start, int64 end)
{
	fSelectionStart = start;
	fSelectionEnd = end;
	fIsSelected = true;
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
	*start = fSelectionStart;
	*end = fSelectionEnd;
}


void
WaveRender::SelectAll()
{
	Select(fStart, fEnd);

	Invalidate();
}


void
WaveRender::Unselect()
{
	fSelectionStart = -1;
	fSelectionEnd = -1;
	fIsSelected = false;
	Invalidate();
}


int64
WaveRender::Pointer()
{
	return fPointer;
}


void
WaveRender::ZoomIn()
{
	fZoomFactor /= 2;

	if (fZoomFactor < 1) {
		fZoomFactor = 1;
		return;
	}

	Invalidate();
}


void
WaveRender::ZoomOut()
{
	fZoomFactor *= 2;

	if (fZoomFactor > 40) {
		fZoomFactor = 40;
		return;
	}

	Invalidate();
}


void
WaveRender::ZoomFull()
{
	fStart = TrackStart();
	fEnd = TrackEnd();
	fZoomFactor = 20;

	Invalidate();
}


void
WaveRender::ZoomSelection()
{
	if (!IsSelected())
		return;

	printf("%ld %ld\n", fSelectionStart, fSelectionEnd); 
	fStart = fSelectionStart;
	fEnd = fSelectionEnd;

	fZoomFactor = _DisplaySize()/Bounds().right;

	Invalidate();
}


int64
WaveRender::TrackStart() const
{
	return 0;
}


int64
WaveRender::TrackEnd() const
{
	//return fTrack->CountFrames()/fTrack->CountChannels();
}


int64
WaveRender::_DisplaySize()
{
	return fEnd-fStart;
}


int64
WaveRender::_ScreenToFrame(int64 value)
{
	return value*128;
}


int64
WaveRender::_FrameToScreen(int64 value)
{
	return value/128;
}
