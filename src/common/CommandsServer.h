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

#ifndef COMMANDS_SERVER_H
#define COMMANDS_SERVER_H

#include <MessageFilter.h>

#include "FaberView.h"


class CommandsServer : public BMessageFilter {
public:
										CommandsServer();

			static CommandsServer* 		Get();

	virtual	filter_result				Filter(BMessage* message,
											BHandler** target);				
private:
			void						_GeneralMessage(BMessage* message);

			static CommandsServer*		fInstance;
			FaberView*					fFaberView;
};

#endif
