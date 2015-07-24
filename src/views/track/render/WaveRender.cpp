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

	float center = Bounds().Height()/((float)channels*2.0f);
	memset(fPreviewCache, 0, MEDIA_BLOCK_PREVIEW_MAX_SIZE);

	for (int32 i = 0; i < channels; i++) {
		MediaBlockMap* channelMap = index->ChannelAt(i);

#ifdef WAVERENDER_READBLOCK
		// TODO read preview block per block
		int64 offset = 0;
		int64 start = 0;
		int64 count = 0;
		for (int32 j = 0; j < channelMap->CountBlocks(); j++) {
			MediaBlock* block = channelMap->BlockAt(j);
			int64 frames = block->PreviewFrames();
			printf("block %d %" B_PRId64 "\n", j, offset);
			offset += block->ReadPreview(fPreviewCache, frames);
			_RenderChannel(fPreviewCache, start, start+frames, &count, center);
			start = offset;
		}
		center += center*2;
#else

		int64 frames = channelMap->PreviewFrames(); 
		float* preview;
		channelMap->Reader()->ReadPreview((void**)&preview);
		int64 count = 0;
		_RenderChannel(preview, 0, frames, &count, center);
		center += center*2;
		delete[] preview;
#endif
	}
}


void
WaveRender::_RenderChannel(float* buffer, int64 start,
	int64 end, int64* count, float center)
{
	int64 screenEnd = (int64)Bounds().right;

	//printf("WaveRender: start %" B_PRId64 " end %" B_PRId64 "\n",
	//	start, start+frames);

	int64 startFrame, endFrame;
	fCoordinator->CurrentSelection(&startFrame, &endFrame);

	for (int64 i = *count+(int64)Bounds().left; i < screenEnd; i++) {
		if (*count < end) {
			if (IsSelected() && i >= fCoordinator->FrameToScreen(startFrame)
					&& i <= fCoordinator->FrameToScreen(endFrame)) {
				SetHighColor(255,255,255);
				SetLowColor(200,200,200);
				//printf("Selection %" B_PRId64 " %" B_PRId64 "\n",
				//	startFrame, endFrame);	
			} else
				SetHighColor(155,157,162);

			float max = buffer[*count];
			float min = buffer[*count+1];

			max = center+max*150.0f;
			min = center+min*150.0f;

			BPoint pointMax((float)i, max);
			BPoint pointMin((float)i, min);

			StrokeLine(pointMin, pointMax);

			// TODO We shouldn't skip frames, instead
			// we should downsample the preview.
			*count+=2*fCoordinator->ZoomFactor();
		} else {
			SetHighColor(140,123,45);
			BPoint pointMax((float)i, center);
			BPoint pointMin((float)i, center);
			StrokeLine(pointMin, pointMax);	
		}
	}
}


void
WaveRender::_RenderPointers(BRect rect)
{
	float pointer = fCoordinator->FrameToScreen(fCoordinator->Pointer());

	if (IsSelected()) {
		SetHighColor(255,255,255);
		StrokeLine(BPoint(pointer, rect.bottom),
			BPoint(pointer, 0));

		int64 start, end;
		float point;
		fCoordinator->CurrentSelection(&start, &end);

		if(start < fCoordinator->Pointer())
			point = fCoordinator->FrameToScreen(start);
		else
			point = fCoordinator->FrameToScreen(end);

		SetHighColor(255,255,255);
		StrokeLine(BPoint(point, rect.bottom),
			BPoint(point, 0.0f));
	} else if (IsFocus()) {
		SetHighColor(255,255,255);
		StrokeLine(BPoint(pointer, rect.bottom),
			BPoint(pointer, 0.0f));
	}
}

