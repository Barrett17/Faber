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

#include "TracksCoordinator.h"

#include <stdio.h>

TracksCoordinator* TracksCoordinator::fInstance = NULL;


TracksCoordinator*
TracksCoordinator::Get()
{
	if (fInstance == NULL)
		fInstance = new TracksCoordinator();

	return fInstance;	
}


TracksCoordinator::TracksCoordinator()
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


TracksCoordinator::~TracksCoordinator()
{
}


void
TracksCoordinator::ScrollBy(int64 value)
{
}


void
TracksCoordinator::Select(int64 start, int64 end)
{
	selectionStart = start;
	selectionEnd = end;
	isSelected = true;
}


bool
TracksCoordinator::IsSelected()
{
	return isSelected;
}


void
TracksCoordinator::CurrentSelection(int64* start, int64* end)
{
	*start = selectionStart;
	*end = selectionEnd;
}


void
TracksCoordinator::SelectAll()
{
	Select(start, end);
}


void
TracksCoordinator::Unselect()
{
	selectionStart = -1;
	selectionEnd = -1;
	isSelected = false;
}


void
TracksCoordinator::ZoomIn()
{
	zoomFactor /= 2;

	if (zoomFactor < 1) {
		zoomFactor = 1;
		return;
	}
}


void
TracksCoordinator::ZoomOut()
{
	zoomFactor *= 2;

	if (zoomFactor > 40) {
		zoomFactor = 40;
		return;
	}
}


void
TracksCoordinator::ZoomFull()
{
	/*pointer = 0;
	end = fTrack->CountFrames()/fTrack->CountChannels();
	zoomFactor = 20;*/
}


void
TracksCoordinator::ZoomSelection()
{
	if (!IsSelected())
		return;

	printf("%ld %ld\n", selectionStart, selectionEnd); 
	pointer = selectionStart;
	end = selectionEnd;

	zoomFactor = 20;
}


int64
TracksCoordinator::Pointer()
{
	return pointer;
}


int64
TracksCoordinator::_DisplaySize()
{
	return end-pointer;
}
