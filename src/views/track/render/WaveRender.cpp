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


WaveRender::WaveRender(AudioTrackView* owner, TracksCoordinator* coordinator)
	: 
	BView("", B_WILL_DRAW),
	fOwner(owner),
	fTrack(owner->GetTrack()),
	fCoordinator(coordinator)
{
	SetViewColor(60,60,60);
}


AudioTrackView*
WaveRender::GetOwner() const
{
	return fOwner;
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
			printf("WaveRender::_RenderTrack: Error preview is null\n");
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
	int64 count = 0;

	for (int64 i = 0; i < end; i++) {
		if (count >= size)
			return;

		int64 point, start, end;
		fCoordinator->CurrentSelection(&start, &end);
		if (fCoordinator->IsSelected()
			&& i >= fCoordinator->FrameToScreen(start)
				&& i <= fCoordinator->FrameToScreen(end)) {
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

		count+=2*fCoordinator->ZoomFactor();
	}
}


void
WaveRender::_RenderPointers(BRect rect)
{
	int64 pointer = fCoordinator->Pointer()/128;

	if (IsFocus()) {
		SetHighColor(255,255,255);
		StrokeLine(BPoint(pointer, rect.bottom),
			BPoint(pointer, 0));
	}

	if (fCoordinator->IsSelected()) {
		int64 point, start, end;
		fCoordinator->CurrentSelection(&start, &end);

		if(start < fCoordinator->Pointer())
			point = fCoordinator->FrameToScreen(start);
		else
			point = fCoordinator->FrameToScreen(end);

		SetHighColor(255,255,255);
		StrokeLine(BPoint(point, rect.bottom),
			BPoint(point, 0));
	}
}


void
WaveRender::MouseDown(BPoint point)
{
	fCoordinator->NotifyMouseDown(point, Window()->CurrentMessage(), this);
}


void
WaveRender::MouseUp(BPoint point)
{
	fCoordinator->NotifyMouseUp(point, this);
}


void
WaveRender::MouseMoved(BPoint point, uint32 data, const BMessage* message)
{
	fCoordinator->NotifyMouseMoved(point, data, message, this);
}


void
WaveRender::MakeFocus(bool focused)
{
	fCoordinator->NotifyMakeFocus(focused, this);
	BView::MakeFocus(focused);
}
