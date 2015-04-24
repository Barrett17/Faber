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
#include "FaberDefs.h"
#include "ParameterWindow.h"
#include "WindowsManager.h"

CommandServer* CommandServer::fInstance = NULL;


void
CommandServer::RunCommandServer()
{
	fInstance = new CommandServer();
}


status_t
CommandServer::AddFilterWindow(BWindow* window)
{
	if (fInstance != NULL) {
		window->AddFilter(fInstance);
		return B_OK;
	}

	return B_ERROR;	
}


status_t
CommandServer::AddCommandListener(CommandListener* listener)
{
	fInstance->fListeners.AddItem(listener);
	return B_OK;
}


status_t
CommandServer::RemoveCommandListener(CommandListener* listener)
{
	fInstance->fListeners.RemoveItem(listener);
	return B_OK;
}


CommandServer::CommandServer()
	:
	BMessageFilter(B_ANY_DELIVERY, B_ANY_SOURCE)
{
	if (fInstance == NULL)
		fInstance = this;
	else
		printf("Error attempting to create more than one CommandServer");
}


filter_result
CommandServer::Filter(BMessage* message, BHandler** target)
{
	if (message->what != FABER_COMMAND)
		return B_DISPATCH_MESSAGE;

	uint32 commandCode;
	if (message->FindUInt32(FABER_COMMAND_CODE, &commandCode) != B_OK)
		return B_SKIP_MESSAGE;

	message->what = commandCode;

	filter_result result = B_SKIP_MESSAGE;

	for (int32 i = 0; i < fListeners.CountItems(); i++)
		fListeners.ItemAt(i)->HandleCommand(message);

	switch (message->what) {

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
			WindowsManager::ShowSettings();
		break;

		case FABER_FILE_OPEN:
			WindowsManager::GetOpenPanel()->Show();
		break;

		case FABER_SAVE_AS_PROJECT:
			WindowsManager::GetSavePanel()->Show();
		break;

		case FABER_EXPORT_PROJECT:
			WindowsManager::GetExportPanel()->Show();
		break;

		case FABER_OPEN_MIXER:
			WindowsManager::GetFaberMixer()->Show();
		break;

		case FABER_NEW_PROJECT:
		{
			app_info info;
			be_app->GetAppInfo(&info);
			be_roster->Launch(info.signature);
			break;
		}

		case FABER_SAVE_PROJECT:
		{
			if (!ProjectManager::WasSaved()) {
				WindowsManager::GetSavePanel()->Show();
				break;
			}
			if (ProjectManager::HasChanged())
				SendCommand(new BMessage(B_SAVE_REQUESTED));

			break;
		}

		case B_SAVE_REQUESTED:
			ProjectManager::SaveProject();
		break;

		case FABER_UNDO:
		{
			if (ProjectManager::HasUndo())
				ProjectManager::Undo();

			break;
		}

		case FABER_REDO:
		{
			if (ProjectManager::HasRedo())
				ProjectManager::Redo();

			break;
		}

		default:
			result = B_DISPATCH_MESSAGE;
	}

	return result;
}


status_t
CommandServer::SendCommand(BMessage* msg)
{
	WindowsManager::PostMessage(msg);

	return B_OK;
}
