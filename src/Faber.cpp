/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "Faber.h"

#include "FaberWindow.h"
#include "TrackIO.h"
#include "WindowsManager.h"


int main()
{
	FaberApp* faberApp;
	faberApp = new FaberApp();

	faberApp->Run();

	delete faberApp;
	return 0;
}


FaberApp::FaberApp()
	:
	BApplication(FABER_MIMETYPE)
{
	BRect rect(50, 50, WINDOW_DEFAULT_SIZE_X, WINDOW_DEFAULT_SIZE_Y);

	fFaberWindow = WindowsManager::Get()->IstantiateMainWindow(rect);
	fFaberWindow->UpdateMenu();
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
	message->PrintToStream();
	switch (message->what)
	{
		case UPDATE_MENU:
			fFaberWindow->UpdateMenu();
		break;

		case DROP_PASTE:
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
	fFaberWindow->Container()->AddTrack(track);
}


void
FaberApp::Save(BMessage* message)
{
	Track* track = TrackIO::Get()->Save(message);
}
