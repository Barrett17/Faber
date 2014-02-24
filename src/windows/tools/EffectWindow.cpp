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

#include "EffectWindow.h"

#include <Box.h>
#include <Button.h>
#include <LayoutBuilder.h>

#include "FaberMessages.h"


EffectWindow::EffectWindow(FaberEffect* effect)
	:
	BWindow(BRect(50, 50, 250, 100), "Faber" , B_TITLED_WINDOW,
		B_NOT_RESIZABLE | B_NOT_MINIMIZABLE | B_NOT_ZOOMABLE),

	fEffect(effect)
{
	SetTitle(fEffect->Name());

	BView* panel = fEffect->SettingsPanel();

	BBox* box = new BBox(fEffect->Name(), B_WILL_DRAW, B_PLAIN_BORDER, panel);

	if (panel != NULL) {
		BLayoutBuilder::Group<>(this, B_VERTICAL, 1.0f)
			.SetInsets(10, 10)
			.Add(box)
			.AddGroup(B_HORIZONTAL)
				.Add(new BButton("Apply"))
				.Add(new BButton("Cancel"))
			.End()
		.End();
	}
	ResizeTo(panel->Bounds().Width(), panel->Bounds().Height()+50);
}


void
EffectWindow::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case FABER_EFFECT_EXECUTE:
		{

			break;
		}

		case FABER_EFFECT_ABORT:
		{

			break;
		}

		default:
			BWindow::MessageReceived(message);
	}
	
}
