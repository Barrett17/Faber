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

#include "Render.h"

#include <Window.h>

#include "FaberDefs.h"
#include "StorageUtils.h"
#include "TracksCoordinator.h"

#include <stdio.h>


Render::Render(AudioTrackView* owner, TracksCoordinator* coordinator)
	: 
	BView("Render", B_WILL_DRAW),
	fOwner(owner),
	fTrack(owner->GetTrack()),
	fCoordinator(coordinator),
	fIsSelected(false)
{
	SetViewColor(60,60,60);
}


AudioTrackView*
Render::GetOwner() const
{
	return fOwner;
}


bool
Render::IsSelected() const
{
	return fIsSelected;
}


void
Render::SetSelected(bool selected)
{
	fIsSelected = selected;
}


void
Render::MouseDown(BPoint point)
{
	fCoordinator->NotifyMouseDown(point, Window()->CurrentMessage(), this);
}


void
Render::MouseUp(BPoint point)
{
	fCoordinator->NotifyMouseUp(point, this);
}


void
Render::MouseMoved(BPoint point, uint32 data, const BMessage* message)
{
	fCoordinator->NotifyMouseMoved(point, data, message, this);
}
