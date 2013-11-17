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
#include "WidgetFrame.h"
#include "VolumeSlider.h"


InfoToolBar::InfoToolBar()
	:
	BView("InfoToolBar", B_WILL_DRAW)
{
	fPointerTextView = new BTextView("view", B_WILL_DRAW);

	fPointerTextView->SetExplicitMaxSize(BSize(1500, 20));
	fPointerTextView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	VolumeSlider* slider = new VolumeSlider("slider", 0, 10, 7, NULL);
	slider->SetExplicitMaxSize(BSize(150, B_SIZE_UNSET));

	WidgetFrame* zoomView = new WidgetFrame();

	BLayoutBuilder::Group<>(zoomView, B_HORIZONTAL, 0.5f)
		.AddStrut(7.0f)
		.Add(_BuildButton(B_TRANSLATE("Zoom in"), 
			FABER_ZOOM_IN, kZoomInIcon))

		.Add(_BuildButton(B_TRANSLATE("Zoom out"),
			FABER_ZOOM_OUT, kZoomOutIcon))

		.Add(_BuildButton(B_TRANSLATE("Zoom to selection"),
			FABER_ZOOM_SELECTION, kZoomToSelectionIcon))

		.Add(_BuildButton(B_TRANSLATE("Zoom full wave"),
			FABER_ZOOM_FULL, kZoomFullWaveIcon))
		.AddStrut(7.0f)
	.End();

	slider->SetExplicitSize(BSize(130, 40));

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
		.Add(slider)
		.AddGlue()
		.Add(zoomView)
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
	IconButton* button = new IconButton(NULL, 0, NULL, message, this, true);
	// Well those could go into the constructor, but no reason for now.
	button->SetToolTip(tip);
	button->SetIcon(resourceID);
	button->TrimIcon();
	return button;
}
