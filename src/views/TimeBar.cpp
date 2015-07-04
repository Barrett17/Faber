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


TimeBar::TimeBar(TracksCoordinator* coordinator)
	:
	BView("TimeBar", B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
	fCoordinator(coordinator)
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

	float end = Bounds().right;

	float countdown = 0;
	float position = 0;
	float minutes = 0;
	float seconds = 0;

	char* drawString = new char[20];

	SetDrawingMode(B_OP_OVER);
	SetHighColor(textColor);
	StrokeLine(BPoint(Bounds().left, Bounds().bottom-1),
		BPoint(Bounds().right, Bounds().bottom-1));

	float prop = 15.0f;
	int32 samples = 0;

	for (float index = 0; index <= end; index++) {
		position = index * prop;

		if(countdown > 0) {
			StrokeLine(BPoint(position, Bounds().bottom - LITTLE_PIN),
				BPoint(position, Bounds().bottom));

			countdown -= 1;
		} else {
			float bottom = Bounds().bottom - BIG_PIN;
			StrokeLine(BPoint(position, bottom),
				BPoint(position, Bounds().bottom-2));

			minutes = 0;
			seconds = (samples)/* / 192000.0f*/;

			//if (minutes == 0)
				sprintf(drawString, "%.2d", (int)seconds);
			//else {
			//	sprintf(drawString, "%d:%.2d", (int)minutes, (int)seconds);
			//}

			DrawString(drawString, BPoint(position, bottom - 3));

			countdown = 5;
		}

		samples += 512*fCoordinator->ZoomFactor();
	}

	//printf("%f\n", index);

	delete[] drawString;
}
