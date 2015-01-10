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

#include "SettingsWindow.h"

#include <LayoutBuilder.h>

#include "SettingsView.h"


SettingsWindow::SettingsWindow()
	:
	BWindow(BRect(100, 100, 500, 600),
		"Settings",
		B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_MINIMIZABLE)
{
	SettingsView* settingsView = new SettingsView();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(settingsView)
	.End();
}


SettingsWindow::~SettingsWindow()
{

}


bool
SettingsWindow::QuitRequested()
{
	return true;
}


void
SettingsWindow::MessageReceived(BMessage* message)
{

}
