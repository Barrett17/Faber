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


#include "CommandServer.h"

#include <Application.h>
#include <Roster.h>

#include "CommandBuilder.h"
#include "EffectWindow.h"
#include "FaberDefs.h"
#include "ParameterWindow.h"
#include "WindowsManager.h"

BObjectList<CommandListener> CommandServer::fExecutors;


void
CommandServer::AddCommandListener(CommandListener* listener)
{
	fExecutors.AddItem(listener);
}


void
CommandServer::RemoveCommandListener(CommandListener* listener)
{
	fExecutors.RemoveItem(listener);
}


CommandServer::CommandServer()
	:
	BMessageFilter(B_ANY_DELIVERY, B_ANY_SOURCE)
{
	fProjectManager = ProjectManager::Get();
	fAudioGate = AudioGate::Get();
}


filter_result
CommandServer::Filter(BMessage* message, BHandler **target)
{
	if (message->what != FABER_COMMAND)
		return B_DISPATCH_MESSAGE;

	uint32 commandCode;
	if (message->FindUInt32(FABER_COMMAND_CODE, &commandCode) != B_OK)
		return B_SKIP_MESSAGE;

	message->what = commandCode;

	filter_result result = B_DISPATCH_MESSAGE;
	bool skip = true;
	bool executed = false;

	//message->PrintToStream();

	for (int32 i = 0; i < fExecutors.CountItems(); i++) {
		if (fExecutors.ItemAt(i)->HandleCommand(message) == B_OK)
			executed = true;
	}

	if (executed == true)
		return B_SKIP_MESSAGE;

	switch (message->what) {

		case FABER_UPDATE_MENU:
			WindowsManager::MainWindow()->MainView()->UpdateMenu();
		break;

		case FABER_ABOUT:
			WindowsManager::ShowAbout();
		break;

		case FABER_OPEN_HOMEPAGE:
		{
			const char* homepage = FABER_HELP_HOMEPAGE;
			be_roster->Launch("text/html", 1,
				const_cast<char**>(&homepage));

			break;
		}

		case FABER_SETTINGS:
			WindowsManager::Get()->ShowSettings();
		break;

		case FABER_FILE_OPEN:
		{
			WindowsManager::GetOpenPanel()->Show();

			break;
		}

		case FABER_EXPORT_PROJECT:
		{
			WindowsManager::GetExportPanel()->Show();

			break;
		}

		case FABER_NEW_PROJECT:
		{
			app_info info;
			be_app->GetAppInfo(&info);
			be_roster->Launch(info.signature);

			break;
		}

		case FABER_SAVE_PROJECT:
		{
			if (!fProjectManager->WasSaved()) {
				WindowsManager::GetSavePanel()->Show();
				break;
			}
			if (fProjectManager->HasChanged())
				SendCommand(new BMessage(B_SAVE_REQUESTED));

			break;
		}

		case FABER_SAVE_AS_PROJECT:
		{	
			WindowsManager::GetSavePanel()->Show();

			break;
		}

		case B_SAVE_REQUESTED:
		{
			printf("Save requested\n");

			ProjectManager::Get()->SaveProject();	

			break;
		}

		case FABER_UNDO:
		{
			if (fProjectManager->HasUndo())
				fProjectManager->Undo();

			break;
		}

		case FABER_REDO:
		{
			if (fProjectManager->HasRedo())
				fProjectManager->Redo();

			break;
		}

		case FABER_OPEN_MIXER:
		{
			WindowsManager::GetFaberMixer()->Show();
			break;
		}

		case FABER_EFFECT_CALL:
		{
			FaberEffect* effect;
			if (message->FindPointer("faber:effect_pointer",
				(void**)&effect) != B_OK) {
				WindowsManager::SimpleError("can't find effect");
				break;
			}

			EffectWindow* win = new EffectWindow(effect);
			win->Show();
			break;
		}

		case FABER_EFFECT_EXECUTE:
		{
			
			break;
		}

		default:
			skip = false;
	}

	if (skip == true)
		result = B_SKIP_MESSAGE;

	return result;
}


status_t
CommandServer::SendCommand(BMessage* msg)
{
	BMessenger mess = WindowsManager::MainWindow();
	return mess.SendMessage(msg);
}
