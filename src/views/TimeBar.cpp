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

#include "TimeBar.h"

#include <stdio.h>

#define LITTLE_PIN 3.0f
#define BIG_PIN 7.0f


TimeBar::TimeBar()
	:
	BView("TimeBar", B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
	fSpacing(15),
	fScale(1)
{
	rgb_color backgroundColor = {120,120,120};

	SetExplicitMinSize(BSize(B_SIZE_UNSET, 25));
	SetExplicitMaxSize(BSize(B_SIZE_UNSET, 25));

	SetViewColor(backgroundColor);
}


TimeBar::~TimeBar()
{
}


void
TimeBar::MessageReceived(BMessage* message)
{
	switch(message->what) {
	
		default:
			BView::MessageReceived(message);
	}
}


void
TimeBar::Draw(BRect rect)
{
	static rgb_color textColor = {0,0,0};
	int32 start = (int32)Bounds().left * fScale;
	int32 end = (int32)Bounds().right * fScale;

	start /= fSpacing;
	end /= fSpacing;

	int32 countdown = 0;
	int32 position = 0;
	int minutes = 0;
	int seconds = 0;

	char* drawString = new char[20];

	SetDrawingMode(B_OP_OVER);
	SetHighColor(textColor);
	StrokeLine(BPoint(Bounds().left, Bounds().bottom-1),
		BPoint(Bounds().right, Bounds().bottom-1));

	int32 prop = fSpacing / fScale;

	for (int32 index = start; index <= end; index++) {
		position = index * prop;

		if(countdown > 0) {
			StrokeLine(BPoint(position, Bounds().bottom - LITTLE_PIN),
				BPoint(position, Bounds().bottom));

			countdown -= 1;
		} else {
			float bottom = Bounds().bottom - BIG_PIN;
			StrokeLine(BPoint(position, bottom),
				BPoint(position, Bounds().bottom-2));

			minutes = index / 6;
			seconds = 10 * (index - minutes * 6);

			if (minutes == 0)
				sprintf(drawString, "%.2d", seconds);
			else {
				sprintf(drawString, "%d:%.2d", minutes, seconds);
				position -= 10;
				if (minutes > 9)
					position -= 6;
			}

			DrawString(drawString, BPoint(position, bottom - 3));

			countdown = 5;
		}
	}

	delete[] drawString;
}


void
TimeBar::SetDuration(bigtime_t duration)
{
}


bigtime_t
TimeBar::Duration()
{
}
