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
#include <ControlLook.h>
#include <LayoutBuilder.h>
#include <OutlineListView.h>
#include <ScrollView.h>

#include "FaberDefs.h"
#include "VolumeSlider.h"

static float kPadding = be_control_look->DefaultItemSpacing();


ExportWindow::ExportWindow()
	:
	BWindow(BRect(100, 100, 500, 420), "Faber" , B_TITLED_WINDOW,
		B_NOT_MINIMIZABLE | B_NOT_ZOOMABLE)
{
	SetTitle(B_TRANSLATE("Export Project"));

	BBox* optionsBox = new BBox(B_FANCY_BORDER, _CreateOptionsBox());
	optionsBox->SetLabel("Options");
	optionsBox->SetExplicitSize(BSize(350, 250));

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(kPadding)
		.Add(optionsBox)
		.AddGroup(B_HORIZONTAL)
			.Add(new BButton(B_TRANSLATE("Export"), MessageBuilder(FABER_EXPORT_PROJECT)))
			.Add(new BButton(B_TRANSLATE("Cancel"), MessageBuilder(FABER_CANCEL)))
		.End()
	.End();
}


BView*
ExportWindow::_CreateOptionsBox()
{
	BView* view = new BView("", B_WILL_DRAW);

	BStringView* fPathView = new BStringView("", "", B_WILL_DRAW);
	fPathView->SetExplicitSize(BSize(200, B_SIZE_UNSET));

	VolumeSlider* slider = new VolumeSlider("slider", 0, 10, 7);
	//slider->SetExplicitSize(BSize(150, B_SIZE_UNSET));
	slider->SetLabel(B_TRANSLATE("Audio Quality"));
	slider->SetLimitLabels(B_TRANSLATE("Low"), B_TRANSLATE("High"));

	BMenuField* fFormatMenu = new BMenuField(NULL, B_TRANSLATE("File Format:"), 
				new BPopUpMenu(""));

	fFormatMenu->SetExplicitMinSize(BSize(250, B_SIZE_UNSET));

	BMenuField*	fEncodingMenu = new BMenuField(NULL, B_TRANSLATE("Audio Encoding:"), 
				new BPopUpMenu(""));

	fEncodingMenu->SetExplicitMinSize(BSize(250, B_SIZE_UNSET));

	BTextControl* fFileNameControl = new BTextControl("File name :", "", NULL);
	fFileNameControl->SetExplicitSize(BSize(200, B_SIZE_UNSET));

	BLayoutBuilder::Group<>(view, B_VERTICAL)
		.SetInsets(kPadding)
		.AddGroup(B_HORIZONTAL)
			.AddGroup(B_VERTICAL)

				.Add(fFormatMenu)
				.Add(fEncodingMenu)
			.End()

			.AddGlue()
			
		.End()
		.Add(slider)

		.AddGroup(B_HORIZONTAL)
			.Add(new BButton(B_TRANSLATE("Output directory"), NULL))
			.Add(fPathView)
			//.AddGlue()
		.End()
		.Add(fFileNameControl)
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
