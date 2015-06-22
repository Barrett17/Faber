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

#include "FaberDefs.h"
#include "StorageUtils.h"
#include "TracksCoordinator.h"


WaveRender::WaveRender(AudioTrackView* owner, TracksCoordinator* coordinator)
	: 
	Render(owner, coordinator)
{
	SetViewColor(60,60,60);
	fPreviewCache = new float[MEDIA_BLOCK_PREVIEW_MAX_FRAMES];
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
	SetHighColor(155,157,162);

	TrackIndex* index = fTrack->GetIndex();
	int32 channels = index->CountChannels();

	float center = Bounds().Height()/(channels*2);
	memset(fPreviewCache, 0, MEDIA_BLOCK_PREVIEW_MAX_SIZE);

	for (int32 i = 0; i < channels; i++) {
		MediaBlockMap* channelMap = index->ChannelAt(i);
// TODO read preview block per block
#if 0
		int64 start = 0;
		for (int32 j = 0; j < channelMap->CountBlocks(); j++) {
			MediaBlock* block = channelMap->BlockAt(j);
			int64 frames = block->PreviewFrames();
			printf("block %d %" B_PRId64 "\n", j, start);
			block->ReadPreview(fPreviewCache, frames);
			_RenderChannel(fPreviewCache, start, frames, center);
			start += frames;
		}
#endif
		int64 frames = channelMap->PreviewFrames(); 
		float* preview;
		channelMap->Reader()->ReadPreview((void**)&preview);
		_RenderChannel(preview, 0, frames, center);
		center += center*2;
		delete[] preview;
	}
}


void
WaveRender::_RenderChannel(float* buffer, int64 start, int64 frames, float center)
{
	int64 end = (int64)Bounds().right;
	int64 count = start;
	int64 startFrame, endFrame;
	fCoordinator->CurrentSelection(&startFrame, &endFrame);

	//printf("WaveRender: start %" B_PRId64 " end %" B_PRId64 "\n",
	//	start, start+frames);

	for (int64 i = start; i < frames+start; i++) {
		if (count < frames+start) {

			if (IsSelected()
				&& i >= fCoordinator->FrameToScreen(startFrame)
					&& i <= fCoordinator->FrameToScreen(endFrame)) {
				SetHighColor(255,255,255);
				SetLowColor(200,200,200);
			} else
				SetHighColor(155,157,162);

			float max = buffer[count];
			float min = buffer[count+1];

			max = center+max*150.0f;
			min = center+min*150.0f;

			BPoint pointMax(i, max);
			BPoint pointMin(i, min);

			StrokeLine(pointMin, pointMax);

			// TODO We shouldn't skip frames, instead
			// we should downsample the preview.
			count+=2*fCoordinator->ZoomFactor();
		} else {
			SetHighColor(140,123,45);
			BPoint pointMax(i, center);
			BPoint pointMin(i, center);
			StrokeLine(pointMin, pointMax);	
		}
	}
}


void
WaveRender::_RenderPointers(BRect rect)
{
	int64 pointer = fCoordinator->FrameToScreen(fCoordinator->Pointer());

	if (IsSelected()) {
		SetHighColor(255,255,255);
		StrokeLine(BPoint(pointer, rect.bottom),
			BPoint(pointer, 0));

		int64 point, start, end;
		fCoordinator->CurrentSelection(&start, &end);

		if(start < fCoordinator->Pointer())
			point = fCoordinator->FrameToScreen(start);
		else
			point = fCoordinator->FrameToScreen(end);

		SetHighColor(255,255,255);
		StrokeLine(BPoint(point, rect.bottom),
			BPoint(point, 0));
	} else if (IsFocus()) {
		SetHighColor(255,255,255);
		StrokeLine(BPoint(pointer, rect.bottom),
			BPoint(pointer, 0));
	}
}

