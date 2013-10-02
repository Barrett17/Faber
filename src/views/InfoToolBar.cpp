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

#include "InfoToolBar.h"

#include <GroupView.h>
#include <LayoutBuilder.h>
#include <TextView.h>

#include "FaberDefs.h"
#include "FaberMessages.h"
#include "FaberResources.h"


InfoToolBar::InfoToolBar()
	:
	BView("InfoToolBar", B_FOLLOW_ALL_SIDES | B_FOLLOW_BOTTOM
		| B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_PULSE_NEEDED)
{
	fPointerTextView = new BTextView("view", B_WILL_DRAW);

	fPointerTextView->SetExplicitMaxSize(BSize(1500, 20));
	fPointerTextView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BGroupView* zoomView = new BGroupView(B_HORIZONTAL);

	BLayoutBuilder::Group<>(zoomView, B_HORIZONTAL)
		.Add(_BuildButton(B_TRANSLATE("Zoom in"), 
			new BMessage(FABER_ZOOM_IN), kZoomInIcon))

		.Add(_BuildButton(B_TRANSLATE("Zoom out"),
			new BMessage(FABER_ZOOM_OUT), kZoomOutIcon))

		.Add(_BuildButton(B_TRANSLATE("Zoom to selection"),
			new BMessage(FABER_ZOOM_SELECTION), kZoomToSelectionIcon))

		.Add(_BuildButton(B_TRANSLATE("Zoom full wave"),
			new BMessage(FABER_ZOOM_FULL), kZoomFullWaveIcon))
	.End();

	//rgb_color backgroundColor = {120,120,120};
	//zoomView->SetViewColor(backgroundColor);

	BLayoutBuilder::Group<>(this, B_HORIZONTAL)
		//.Add(fPointerTextView)
		.AddGlue()
		.Add(zoomView)
		.AddStrut(5.0f)
	.End();
}


InfoToolBar::~InfoToolBar()
{
}


void
InfoToolBar::Pulse()
{
	/*int64 p = 0;
	if (Pool.sample_type){
		if (Pool.IsPlaying()){
			p = Pool.last_pointer;
		}else
			p = Pool.pointer;
	}

	if (p != pointer){
		pointer = p;
		fPointerTextView->SetText(BString() << p);
	}*/
}


IconButton*
InfoToolBar::_BuildButton(const char* tip, BMessage* message, int32 resourceID)
{
	IconButton* button = new IconButton(NULL, 0, NULL, message, this);
	// Well those could go into the constructor, but no reason for now.
	button->SetToolTip(tip);
	button->SetIcon(resourceID);
	button->TrimIcon();
	return button;
}
