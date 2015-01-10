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

#include "FaberDefs.h"
#include "FaberResources.h"
#include "FaberWindow.h"
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
	BApplication(FABER_SIGNATURE)
{
	// Load app keys
	FaberShortcut::LoadKeys();

	fFaberWindow = WindowsManager::MainWindow();
	fFaberWindow->Show();

	fProjectManager = ProjectManager::Get();
	fProjectManager->SetName(B_TRANSLATE("Untitled"));
}


void
FaberApp::MessageReceived(BMessage* message)
{
	//message->PrintToStream();
	switch (message->what)
	{
		case FABER_DROP_PASTE:
		case B_PASTE:
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
	entry_ref ref;
	int32 count = 0;
	while (message->FindRef("refs", count, &ref) == B_OK) {
		fProjectManager->LoadFile(ref);
		count++;
	}
}
