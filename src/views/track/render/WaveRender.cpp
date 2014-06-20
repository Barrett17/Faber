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
	fUpdate(true)
{
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
WaveRender::_RenderChannel(MediaBlockMap* channelMap, float center)
{
	PreviewReader* reader = channelMap->Preview();

	int64 end = Bounds().right;

	printf("%ld %ld\n", GetCoords().start, GetCoords().end);

	int64 count = _FrameToScreen(GetCoords().start);

	for (int64 i = 0; i < end; i++) {

		if (IsSelected() && i >= _FrameToScreen(GetCoords().selectionStart)
			&& i <= _FrameToScreen(GetCoords().selectionEnd)) {
			SetHighColor(255,255,255);
			SetLowColor(200,200,200);
		} else {
			SetHighColor(155,157,162);
		}

		//float max = buffer[count];
		//float min = buffer[count+1];

		float max = 1;
		float min = -1;

		BPoint pointMin(i, center-max*30.0f);
		BPoint pointMax(i, center-min*30.0f);

		StrokeLine(pointMin, pointMax);

		count+=2/**fZoomFactor*/;
	}
}


void
WaveRender::_RenderPointers(BRect rect)
{
	int64 pointer = GetCoords().pointer/128;

	if (IsFocus()) {
		SetHighColor(255,255,255);
		StrokeLine(BPoint(pointer, rect.bottom),
			BPoint(pointer, 0));
	}

	if (IsSelected()) {
		printf("rendering\n");
		int64 point;

		if(GetCoords().selectionStart < GetCoords().pointer)
			point = _FrameToScreen(GetCoords().selectionStart);
		else
			point = _FrameToScreen(GetCoords().selectionEnd);

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
		GetCoords().pointer = _ScreenToFrame(point.x);

		if (GetCoords().isSelected) {
			GetCoords().isSelected = false;
			GetCoords().selectionStart = -1;
			GetCoords().selectionEnd = -1;
			GetCoords().primaryButton = false;
		} else {
			GetCoords().primaryButton = true;
		}

		Invalidate();

	} else if (button == B_SECONDARY_MOUSE_BUTTON) {

	}
}


void
WaveRender::MouseUp(BPoint point)
{
	//printf("WaveRender::MouseUp\n");

	if (GetCoords().primaryButton)
		GetCoords().primaryButton = false;
}

void
WaveRender::MouseMoved(BPoint point, uint32, const BMessage* message)
{
	//printf("WaveRender::MouseMoved\n");

	if (GetCoords().primaryButton) {
		GetCoords().isSelected = true;

		int64 frame = _ScreenToFrame(point.x);

		if (frame < GetCoords().pointer)
			Select(frame, GetCoords().pointer);
		else if (frame > GetCoords().pointer)
			Select(GetCoords().pointer, frame);

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
	if (GetCoords().end+value > fTrack->CountFrames()
		|| GetCoords().pointer+value < 0)
		return;

	GetCoords().pointer += value;
	GetCoords().end += value;

	Invalidate();
}


void
WaveRender::Select(int64 start, int64 end)
{
	BRect rect = Bounds();

	if (start < GetCoords().selectionStart)
		rect.left = _FrameToScreen(start);
	else
		rect.left = _FrameToScreen(GetCoords().selectionStart);

	if (end < GetCoords().selectionEnd)
		rect.right = _FrameToScreen(GetCoords().selectionEnd);
	else
		rect.right = _FrameToScreen(end);

	GetCoords().selectionStart = start;
	GetCoords().selectionEnd = end;
	GetCoords().isSelected = true;

	Invalidate(rect);
}


bool
WaveRender::IsSelected()
{
	return GetCoords().isSelected;
}


void
WaveRender::CurrentSelection(int64* start, int64* end)
{
	*start = GetCoords().selectionStart;
	*end = GetCoords().selectionEnd;
}


void
WaveRender::SelectAll()
{
	Select(GetCoords().pointer, GetCoords().end);

	Invalidate();
}


void
WaveRender::Unselect()
{
	GetCoords().selectionStart = -1;
	GetCoords().selectionEnd = -1;
	GetCoords().isSelected = false;
	Invalidate();
}


int64
WaveRender::Pointer()
{
	return GetCoords().pointer;
}


void
WaveRender::ZoomIn()
{
	/*fZoomFactor /= 2;

	if (fZoomFactor < 1) {
		fZoomFactor = 1;
		return;
	}

	Invalidate();*/
}


void
WaveRender::ZoomOut()
{
	/*fZoomFactor *= 2;

	if (fZoomFactor > 40) {
		fZoomFactor = 40;
		return;
	}

	Invalidate();*/
}


void
WaveRender::ZoomFull()
{
	/*GetCoords().pointer = TrackStart();
	GetCoords().end = TrackEnd();
	fZoomFactor = 20;

	Invalidate();*/
}


void
WaveRender::ZoomSelection()
{
	/*if (!IsSelected())
		return;

	printf("%ld %ld\n", GetCoords().selectionStart, GetCoords().selectionEnd); 
	GetCoords().pointer = GetCoords().selectionStart;
	GetCoords().end = GetCoords().selectionEnd;

	fZoomFactor = _DisplaySize()/Bounds().right;

	Invalidate();*/
}


int64
WaveRender::_DisplaySize()
{
	return GetCoords().end-GetCoords().pointer;
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
