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


ExportWindow::ExportWindow()
	:
	BWindow(BRect(50, 50, 250, 100), "Faber" , B_TITLED_WINDOW,
		B_NOT_RESIZABLE | B_NOT_MINIMIZABLE | B_NOT_ZOOMABLE)
{

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
