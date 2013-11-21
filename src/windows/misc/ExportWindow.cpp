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

#include "ExportWindow.h"

#include <Box.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <OutlineListView.h>
#include <ScrollView.h>

#include "FaberDefs.h"
#include <VolumeSlider.h>


ExportWindow::ExportWindow()
	:
	BWindow(BRect(100, 100, 500, 500), "Faber" , B_TITLED_WINDOW,
		B_NOT_RESIZABLE | B_NOT_MINIMIZABLE | B_NOT_ZOOMABLE)
{
	SetTitle(B_TRANSLATE("Export Project"));

	BBox* sessionBox = new BBox(B_FANCY_BORDER, _CreateSessionBox());
	sessionBox->SetLabel("Current Session");

	BBox* optionsBox = new BBox(B_FANCY_BORDER, _CreateOptionsBox());
	optionsBox->SetLabel("Options");

	BLayoutBuilder::Group<>(this, B_VERTICAL, 5.0f)
		.Add(optionsBox)
		.Add(sessionBox)
		.AddGroup(B_HORIZONTAL)
			.Add(new BButton(B_TRANSLATE("Export"), MessageBuilder(FABER_EXPORT_PROJECT)))
			.Add(new BButton(B_TRANSLATE("Cancel"), MessageBuilder(FABER_CANCEL)))
		.End()
	.End();
}


BView*
ExportWindow::_CreateSessionBox()
{
	BView* view = new BView("", B_WILL_DRAW);

	BOutlineListView* listView = new BOutlineListView("Tracks list");
	listView->SetExplicitSize(BSize(300, 200));

	BScrollView* scrollView = new BScrollView("scroll", listView,
		B_FOLLOW_ALL_SIDES, false, true, B_PLAIN_BORDER);

	BLayoutBuilder::Group<>(view, B_HORIZONTAL)
		.Add(scrollView)
		.AddGlue()
	.End();

	return view;
}


BView*
ExportWindow::_CreateOptionsBox()
{
	BView* view = new BView("", B_WILL_DRAW);

	VolumeSlider* slider = new VolumeSlider("slider", 0, 10, 7, NULL);
	slider->SetExplicitSize(BSize(200, 40));

	BLayoutBuilder::Group<>(view, B_VERTICAL)
		.AddGroup(B_HORIZONTAL)
			.Add(new BButton("Browse", NULL))
			.Add(new BStringView("", "path", B_WILL_DRAW))
			.AddGlue()
		.End()
		
		.Add(slider)
	.End();

	return view;
}


void
ExportWindow::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
	
		default:
			BWindow::MessageReceived(message);
	}
	
}
