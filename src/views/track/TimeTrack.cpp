/*
 * Copyright 2011 Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#include <string.h>
#include <stdio.h>
#include "TrackRepository.h"
#include "TimeTrack.h"


TimeTrack::TimeTrack(BRect frame, const char* name, uint32 resizingMode, uint32 flags)
		:
		BView(frame, name, resizingMode, flags)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}

TimeTrack::~TimeTrack()
{
}


void TimeTrack::Draw(BRect updateRect)
{
	ResizeTo(updateRect.Width(), 27);
	
	int32 cur_x;
	char num[32];
	int32 size_y = (int32) (updateRect.bottom - 10);
	int32 size_x = (int32) (updateRect.right - updateRect.left);
	int32 distance = (int32) (TrackRepository::Instance()->ZoomLevel() * 5);
	int32 time = TrackRepository::Instance()->CurrentTime();
	int32 cursor_pos = TrackRepository::Instance()->CursorTime();
	int32 interval = 1;
	
	if(distance < 1)
		interval = 20;
	else if(distance < 5)
		interval = 10;
	else if(distance < 10)
		interval = 5;
	else if(distance < 20)
		interval = 2;
	
	/* Draw the cursor if visible */
	if((cursor_pos >= time) && (cursor_pos <= time + (size_x / distance)))
	{
		BPoint pl[3];
		pl[0] = BPoint(2 * distance * (cursor_pos - time) - 7, 0);
		pl[1] = BPoint(2 * distance * (cursor_pos - time) + 7, 0);
		pl[2] = BPoint(2 * distance * (cursor_pos - time), 10);
		
		SetHighColor(200, 10, 10, 255);
		FillPolygon(pl, 3);
		SetHighColor(0, 0, 0, 255);
	}
	
	
	/* Draw time meter */
	for(cur_x = 0; cur_x < size_x; cur_x += distance)
	{
		if(cur_x % (distance * 2) == 0)
		{
			StrokeLine(BPoint(cur_x, 0), BPoint(cur_x, 10));
			
			if(time % interval == 0)
			{
				sprintf(num, "%d", time);
			
				MovePenTo(BPoint(cur_x - (strlen(num) * 8 / 2), 22));
				DrawString(num);
			}
						
			time++;
		}
		else if(distance != 1)
			StrokeLine(BPoint(cur_x, 0), BPoint(cur_x, 4));
	}
	
}
