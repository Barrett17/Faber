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

		case FABER_SETTINGS:
			WindowsManager::Get()->ShowSettings();
		break;

	}
}

