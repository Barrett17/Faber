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

#include <Alert.h>
#include <Application.h>
#include <LayoutBuilder.h>

#include "CommandsServer.h"
#include "FaberDefs.h"
#include "FaberView.h"


FaberWindow::FaberWindow(BRect rect)
	:
	BWindow(rect, "Faber" , B_TITLED_WINDOW, 0)
{
	fFaberView = new FaberView();

	AddFilter(new CommandsServer());

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fFaberView)
	.End();

}


FaberWindow::~FaberWindow()
{
}


bool
FaberWindow::QuitRequested()
{
	if (ProjectManager::Get()->HasChanged()) {
		BString alertText = "Save changes to ";
		alertText << ProjectManager::Get()->Name();
		alertText << "?";

		BAlert* alert = new BAlert("Warning!", alertText.String(),
			"Cancel", "Don't save", "Save",
			B_WIDTH_AS_USUAL, B_OFFSET_SPACING,
			B_WARNING_ALERT);

		alert->SetShortcut(0, B_ESCAPE);
		int32 button_index = alert->Go();

		if (button_index == 2) {
			BMessenger msg(this);
			msg.SendMessage(new BMessage(FABER_SAVE_PROJECT));
			return false;
		} else if (button_index == 0)
			return false;
	}

	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


void
FaberWindow::MessageReceived(BMessage* message)
{
	//message->PrintToStream();
	switch (message->what)
	{
		case B_SIMPLE_DATA:
		case B_MIME_DATA:
			be_app->PostMessage(message);
			break;

		case FABER_NEW_AUDIO_TRACK:
		case FABER_NEW_LABEL_TRACK:
		case FABER_REMOVE_TRACK:
		case FABER_MUTE_ALL:
		case FABER_UNMUTE_ALL:
		case FABER_SORT_BY_DURATION:
		case FABER_SORT_BY_NAME:
		case B_SELECT_ALL:
		case FABER_UNSELECT_ALL:
		case B_COPY:
		case FABER_COPY_SILENCE:
		case B_CUT:
		case B_PASTE:
		case FABER_DROP_PASTE:
		case FABER_ZOOM_IN:
		case FABER_ZOOM_OUT:
		case FABER_ZOOM_FULL:
		case FABER_ZOOM_SELECTION:
		{
			BMessenger mess(fFaberView->Container());
			mess.SendMessage(message);
		}

		default:
			BWindow::MessageReceived(message);
	}
}


FaberView*
FaberWindow::MainView() const
{
	return fFaberView;
}
