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
#include <PopUpMenu.h>
#include <StringView.h>

#include "CommandBuilder.h"
#include "FaberDefs.h"
#include "VolumeSlider.h"

static float kPadding = be_control_look->DefaultItemSpacing();


ExportWindow::ExportWindow()
	:
	BWindow(BRect(100, 100, 500, 420), "Faber" , B_TITLED_WINDOW,
		B_NOT_MINIMIZABLE | B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	SetTitle(B_TRANSLATE("Export Project"));

	fTabView = new BTabView("");

	_AddTab(_CreateOptionsBox(), B_TRANSLATE("Output"));
	_AddTab(_BuildInfoView(), B_TRANSLATE("Additional Info"));

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(kPadding)
		.Add(fTabView)
		.AddGroup(B_HORIZONTAL)
			.Add(new BButton(B_TRANSLATE("Export"),
				CommandBuilder(FABER_EXPORT_PROJECT)))
			.Add(new BButton(B_TRANSLATE("Cancel"),
				CommandBuilder(FABER_CANCEL)))
		.End()
	.End();
}


BView*
ExportWindow::_CreateOptionsBox()
{
	BView* view = new BView("", B_WILL_DRAW);

	VolumeSlider* slider = new VolumeSlider("slider", 0, 10, 7);
	slider->SetExplicitSize(BSize(200, B_SIZE_UNSET));
	slider->SetLabel(B_TRANSLATE("Audio Quality"));
	slider->SetLimitLabels(B_TRANSLATE("Low"), B_TRANSLATE("High"));

	BMenuField* fFormatMenu = new BMenuField(NULL,
		B_TRANSLATE("File Format:"), new BPopUpMenu(""));

	fFormatMenu->SetExplicitSize(BSize(200, B_SIZE_UNSET));

	BMenuField*	fEncodingMenu = new BMenuField(NULL, B_TRANSLATE("Audio Encoding:"), 
				new BPopUpMenu(""));
	fEncodingMenu->SetExplicitSize(BSize(200, B_SIZE_UNSET));

	BLayoutBuilder::Group<>(view, B_VERTICAL)
		.SetInsets(kPadding)
		.Add(fFormatMenu)
		.Add(fEncodingMenu)
		.Add(slider)
	.End();

	return view;
}


BView*
ExportWindow::_BuildInfoView()
{
	BView* view = new BView("", B_WILL_DRAW);

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


void
ExportWindow::_AddTab(BView* view, const char* label)
{
	BTab* tab = new BTab();

	fTabView->AddTab(view, tab);
	tab->SetLabel(label);
}
