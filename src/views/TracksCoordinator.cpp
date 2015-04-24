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

#include "TracksContainer.h"
#include "Render.h"


TracksCoordinator::TracksCoordinator(TracksContainer* owner)
	:
	fOwner(owner),
	fPointer(0),
	fPlayPointer(0),
	fStart(0),
	fEnd(0),
	fSelectionStart(-1),
	fSelectionEnd(-1),
	fDuration(0),
	fZoomFactor(1),
	fPrimaryButton(false),
	fSecondaryButton(false)
{
}


bool
TracksCoordinator::SelectionActive() const
{
	return fSelectionQueue.CountItems() != 0;
}


void
TracksCoordinator::CurrentSelection(int64* fStart, int64* fEnd)
{
	*fStart = fSelectionStart;
	*fEnd = fSelectionEnd;
}


void
TracksCoordinator::SelectAll()
{
	fSelectionStart = fStart;
	fSelectionEnd = fEnd;
}


void
TracksCoordinator::UnselectAll()
{
	fSelectionStart = -1;
	fSelectionEnd = -1;
	_CleanupSelection();
}


void
TracksCoordinator::NotifySelect(int64 fStart, int64 fEnd, Render* who)
{
	_CleanupSelection();
	_UpdateSelection(fStart, fEnd);
	_AddToSelection(who);
}


void
TracksCoordinator::NotifySelectAll(Render* who)
{
}


void
TracksCoordinator::NotifyUnselect(Render* who)
{
}


void
TracksCoordinator::NotifyMouseDown(BPoint point, BMessage* message,
	Render* who)
{
	uint32 button = 0;
	uint32 click = 0;

	message->FindInt32("buttons", (int32*)&button);
	message->FindInt32("clicks", (int32*)&click);

	if (button == B_PRIMARY_MOUSE_BUTTON) {
		fPointer = fStart+ScreenToFrame(point.x);
		if (SelectionActive())
			_CleanupSelection();
		else
			fPrimaryButton = true;

		who->MakeFocus();
		if (fCurrentRender != NULL)
			fCurrentRender->Invalidate();
		who->Invalidate();
		fCurrentRender = who;
	}
}


void
TracksCoordinator::NotifyMouseUp(BPoint point, Render* who)
{
	if (fPrimaryButton)
		fPrimaryButton = false;
}


void
TracksCoordinator::NotifyMouseMoved(BPoint point, uint32 data,
	const BMessage* message, Render* who)
{
	if (fPrimaryButton) {
		if (!who->IsSelected())
			_AddToSelection(who);

		int64 frame = fStart+ScreenToFrame(point.x);
		if (frame < fPointer)
			_UpdateSelection(frame, fPointer);
		else if (frame > fPointer)
			_UpdateSelection(fPointer, frame);

		InvalidateSelection();
	}
}


void
TracksCoordinator::ZoomIn()
{
	fZoomFactor /= 2;

	if (fZoomFactor < 1) {
		fZoomFactor = 1;
		return;
	}
}


void
TracksCoordinator::ZoomOut()
{
	fZoomFactor *= 2;

	if (fZoomFactor > 40) {
		fZoomFactor = 40;
		return;
	}
}


void
TracksCoordinator::ZoomFull()
{
}


void
TracksCoordinator::ZoomSelection()
{
	if (!SelectionActive())
		return;

	fPointer = fSelectionStart;
	fEnd = fSelectionEnd;

	fZoomFactor = 20;
}


int64
TracksCoordinator::Pointer() const
{
	return fPointer;
}


int64
TracksCoordinator::ZoomFactor() const
{
	return fZoomFactor;
}


int64
TracksCoordinator::ScreenToFrame(int64 value)
{
	return value*128;
}


int64
TracksCoordinator::FrameToScreen(int64 value)
{
	return value/128;
}


void
TracksCoordinator::_UpdateSelection(int64 fStart, int64 fEnd)
{
	fSelectionStart = fStart;
	fSelectionEnd = fEnd;
}


void
TracksCoordinator::_AddToSelection(Render* who)
{
	fSelectionQueue.AddItem(who);
	who->SetSelected(true);
}


void
TracksCoordinator::_RemoveFromSelection(Render* who)
{
	fSelectionQueue.RemoveItem(who);
	who->SetSelected(false);
}


void
TracksCoordinator::InvalidateSelection()
{
	for (int32 i = 0; i < fSelectionQueue.CountItems(); i++)
		fSelectionQueue.ItemAt(i)->Invalidate();
}


void
TracksCoordinator::_CleanupSelection()
{
	_UpdateSelection(-1, -1);
	InvalidateSelection();
	for (int32 i = 0; i < fSelectionQueue.CountItems(); i++)
		_RemoveFromSelection(fSelectionQueue.ItemAt(i));
}
