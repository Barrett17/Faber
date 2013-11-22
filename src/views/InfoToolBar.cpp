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
#include "ZoomWidget.h"
#include "VolumeSlider.h"


InfoToolBar::InfoToolBar()
	:
	BView("InfoToolBar", B_WILL_DRAW)
{
	fPointerTextView = new BTextView("view", B_WILL_DRAW);
	fPointerTextView->SetExplicitMaxSize(BSize(1500, 20));
	fPointerTextView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	VolumeSlider* slider = new VolumeSlider("slider", 0, 10, 7, NULL);
	slider->SetExplicitMaxSize(BSize(150, 40));
	//slider->SetExplicitSize(BSize(130, 40));

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
		.Add(slider)
		.AddGlue()
		.AddGlue()
		.Add(new ZoomWidget())
	.End();
}


InfoToolBar::~InfoToolBar()
{
}
