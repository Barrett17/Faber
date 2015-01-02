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

#include "FaberDefs.h"
#include "StorageUtils.h"
#include "TracksCoordinator.h"

#include <stdio.h>


WaveRender::WaveRender(AudioTrack* track)
	: 
	BView("", B_WILL_DRAW),
	fTrack(track)
{
	if (fTrack->CountChannels() == 0)
		GetCoords().end = 0; 
	else
		GetCoords().end = fTrack->CountFrames()/fTrack->CountChannels();

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
		float* preview = NULL;
		MediaBlockMap* channelMap = index->ChannelAt(i);
		MediaBlockMapReader* reader = channelMap->Reader();
		size_t size = reader->ReadPreview(&preview);
		if (preview == NULL) {
			printf("error\n");
			return;
		}
		_RenderChannel(preview, size, center);
		center += center*2;
		delete preview;
	}
}


void
WaveRender::_RenderChannel(float* buffer, size_t size, float center)
{
	int64 end = (int64)Bounds().right;

	//printf("%ld %ld %ld\n", GetCoords().start, GetCoords().end);

	int64 count = 0;

	for (int64 i = 0; i < end; i++) {
		if (count >= size) {
			//printf("Reached end of preview\n");
			return;
		}

		if (GetCoords().IsSelected() && i >= _FrameToScreen(GetCoords().selectionStart)
			&& i <= _FrameToScreen(GetCoords().selectionEnd)) {
			SetHighColor(255,255,255);
			SetLowColor(200,200,200);
		} else {
			SetHighColor(155,157,162);
		}

		float max = buffer[count];
		float min = buffer[count+1];

		max = center+max*150.0f;
		min = center+min*150.0f;

		BPoint pointMax(i, max);
		BPoint pointMin(i, min);

		StrokeLine(pointMin, pointMax);

		count+=2*GetCoords().zoomFactor;
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

	if (GetCoords().IsSelected()) {
		//printf("rendering\n");
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
		GetCoords().pointer = GetCoords().start+_ScreenToFrame(point.x);

		if (GetCoords().isSelected) {
			GetCoords().isSelected = false;
			GetCoords().selectionStart = -1;
			GetCoords().selectionEnd = -1;
			GetCoords().primaryButton = false;
		} else {
			GetCoords().primaryButton = true;
		}

		

	} else if (button == B_SECONDARY_MOUSE_BUTTON) {

	}
	Invalidate();
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

		int64 frame = GetCoords().start+_ScreenToFrame(point.x);

		if (frame < GetCoords().pointer)
			GetCoords().Select(frame, GetCoords().pointer);
		else if (frame > GetCoords().pointer)
			GetCoords().Select(GetCoords().pointer, frame);

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
	if (focused == false)
		GetCoords().Unselect();

	BView::MakeFocus(focused);
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
