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

#include "FaberDefs.h"


ExportWindow::ExportWindow()
	:
	BWindow(BRect(100, 100, 500, 500), "Faber" , B_TITLED_WINDOW,
		B_NOT_RESIZABLE | B_NOT_MINIMIZABLE | B_NOT_ZOOMABLE)
{
	SetTitle(B_TRANSLATE("Export Project"));
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(_CreateTracksBox())
		.Add(_CreateOutputBox())
		.AddGroup(B_HORIZONTAL)
			.Add(new BButton(B_TRANSLATE("Export"), GeneralMessage(FABER_EXPORT_PROJECT)))
			.Add(new BButton(B_TRANSLATE("Cancel"), GeneralMessage(FABER_CANCEL)))
		.End()
	.End();
}


BBox*
ExportWindow::_CreateTracksBox()
{
	BBox* box = new BBox("Tracks");

	return box;
}


BBox*
ExportWindow::_CreateOutputBox()
{
	BBox* box = new BBox("Tracks");


	return box;
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
