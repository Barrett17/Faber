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

#include "CommandsLooper.h"

#include <Application.h>
#include <Roster.h>

#include "EffectWindow.h"
#include "FaberDefs.h"

CommandsLooper* CommandsLooper::fInstance = NULL;


CommandsLooper*
CommandsLooper::Get()
{
	if (fInstance == NULL)
		fInstance = new CommandsLooper();

	return fInstance;	
}


CommandsLooper::CommandsLooper()
	:
	BLooper("Commands Looper")
{
	Run();
}


void
CommandsLooper::MessageReceived(BMessage* message)
{
	message->PrintToStream();

	switch (message->what)
	{
		case FABER_GENERAL_MESSAGE:
			_GeneralMessage(message);
		break;

		default:
			BLooper::MessageReceived(message);
	}

}


void
CommandsLooper::_GeneralMessage(BMessage* message)
{
	uint32 faber_what = 0;
	message->FindUInt32(FABER_WHAT, &faber_what);

	switch (faber_what)
	{
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

		case FABER_EXPORT_PROJECT:
		{
		
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
		
			break;
		}

		case FABER_FILE_OPEN:
		{
		
			break;
		}

		case FABER_SETTINGS:
			WindowsManager::Get()->ShowSettings();
		break;

		case FABER_EFFECT_CALL:
		{
			FaberEffect* effect;
			message->FindPointer("effect", (void**)&effect);
			EffectWindow* win = new EffectWindow(effect);
			win->Show();
			break;
		}

		case FABER_EFFECT_EXECUTE:
		{
		
			break;
		}

		case FABER_EFFECT_ABORT:
		{

			break;
		}


	}
}

