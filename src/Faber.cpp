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

#include "Faber.h"

#include "FaberMessages.h"
#include "FaberWindow.h"
#include "TrackIO.h"
#include "WindowsManager.h"


int main()
{
	FaberApp* faber;
	faber = new FaberApp();

	faber->Run();

	return 0;
}


FaberApp::FaberApp()
	:
	BApplication(FABER_MIMETYPE)
{
	BRect rect(50, 50, WINDOW_DEFAULT_SIZE_X, WINDOW_DEFAULT_SIZE_Y);

	fFaberWindow = WindowsManager::Get()->IstantiateMainWindow(rect);
	fFaberWindow->Show();
}


bool
FaberApp::QuitRequested()
{
	if (fFaberWindow) {
		if (fFaberWindow->Lock() && fFaberWindow->QuitRequested()) {
			fFaberWindow->Quit();
			return true;
		}
	}
	return true;
}


void
FaberApp::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		case FABER_DROP_PASTE:
		case B_PASTE:
			fFaberWindow->PostMessage(message);
		break;

		case B_SIMPLE_DATA:
		case B_MIME_DATA:
			RefsReceived(message);
		break;

		default:
			BApplication::MessageReceived(message);	
	}
}


void
FaberApp::RefsReceived(BMessage* message)
{
	Track* track = TrackIO::Get()->Load(message);

	if (track == NULL)
		return;

	fFaberWindow->MainView()->AddTrack(track);
}


void
FaberApp::Save(BMessage* message)
{
	Track* track = TrackIO::Get()->Save(message);
}
