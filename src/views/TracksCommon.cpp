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

#include "TracksCommon.h"

#include <stdio.h>

TracksCommon* TracksCommon::fInstance = NULL;


TracksCommon*
TracksCommon::Get()
{
	if (fInstance == NULL)
		fInstance = new TracksCommon();

	return fInstance;	
}


TracksCommon::TracksCommon()
	:
	pointer(0),
	playPointer(0),
	start(0),
	end(0),
	selectionStart(-1),
	selectionEnd(-1),
	duration(0),
	zoomFactor(1),
	primaryButton(false),
	secondaryButton(false),
	scrollButton(false),
	isSelected(false),
	multipleSelection(false)
{
}


TracksCommon::~TracksCommon()
{
}


void
TracksCommon::ScrollBy(int64 value)
{
}


void
TracksCommon::Select(int64 start, int64 end)
{
	selectionStart = start;
	selectionEnd = end;
	isSelected = true;
}


bool
TracksCommon::IsSelected()
{
	return isSelected;
}


void
TracksCommon::CurrentSelection(int64* start, int64* end)
{
	*start = selectionStart;
	*end = selectionEnd;
}


void
TracksCommon::SelectAll()
{
	Select(start, end);
}


void
TracksCommon::Unselect()
{
	selectionStart = -1;
	selectionEnd = -1;
	isSelected = false;
}


void
TracksCommon::ZoomIn()
{
	zoomFactor /= 2;

	if (zoomFactor < 1) {
		zoomFactor = 1;
		return;
	}
}


void
TracksCommon::ZoomOut()
{
	zoomFactor *= 2;

	if (zoomFactor > 40) {
		zoomFactor = 40;
		return;
	}
}


void
TracksCommon::ZoomFull()
{
	/*pointer = 0;
	end = fTrack->CountFrames()/fTrack->CountChannels();
	zoomFactor = 20;*/
}


void
TracksCommon::ZoomSelection()
{
	if (!IsSelected())
		return;

	printf("%ld %ld\n", selectionStart, selectionEnd); 
	pointer = selectionStart;
	end = selectionEnd;

	zoomFactor = 20;
}


int64
TracksCommon::Pointer()
{
	return pointer;
}


int64
TracksCommon::_DisplaySize()
{
	return end-pointer;
}
