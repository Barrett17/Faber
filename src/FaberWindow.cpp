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

#include "FaberWindow.h"

#include <LayoutBuilder.h>

#include "FaberView.h"
#include "MenuManager.h"


FaberWindow::FaberWindow(BRect rect)
	:
	BWindow(rect, "Faber" , B_TITLED_WINDOW, NULL)
{
	fFaberView = new FaberView();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(MenuManager::Get()->BuildMainMenuBar())
		.Add(fFaberView)
	.End();

}


FaberWindow::~FaberWindow()
{

}


bool
FaberWindow::QuitRequested()
{
	return true;
}


void
FaberWindow::MessageReceived(BMessage* message)
{

}


FaberView*
FaberWindow::MainView() const
{
	return fFaberView;
}
