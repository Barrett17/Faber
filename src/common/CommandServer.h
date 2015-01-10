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
#ifndef COMMAND_SERVER_H
#define COMMAND_SERVER_H


#include <MessageFilter.h>
#include <ObjectList.h>

#include "AudioGate.h"
#include "FaberView.h"
#include "ProjectManager.h"


class CommandServer : public BMessageFilter {
public:
								CommandServer();

	virtual	filter_result		Filter(BMessage* message,
									BHandler** target);

			static void			AddCommandListener(
									CommandListener* listener);

			static void			RemoveCommandListener(
									CommandListener* listener);	

			static status_t		SendCommand(BMessage* msg);

private:
			ProjectManager*		fProjectManager;
			AudioGate*			fAudioGate;

			static BObjectList<CommandListener> fExecutors;
};

#endif
