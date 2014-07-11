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

#include "FaberWidget.h"

#include "FaberDefs.h"


FaberWidget::FaberWidget(orientation orientation)
	:
	BGroupView(orientation)
{
	rgb_color backgroundColor = {120,120,120};
	SetViewColor(backgroundColor);
}


FaberWidget::~FaberWidget()
{
}


void
FaberWidget::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		default:
			BGroupView::MessageReceived(message);
	}
}


BIconButton*
FaberWidget::BuildButton(const char* tip, BMessage* message, int32 resourceID)
{
	BIconButton* button = new BIconButton(NULL, NULL, message, this, true);
	// Well those could go into the constructor, but no reason for now.
	button->SetToolTip(tip);
	button->SetIcon(resourceID);
	button->TrimIcon();

	return button;
}
