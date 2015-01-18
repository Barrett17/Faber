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
#ifndef FABER_COMMAND_BUILDER_H
#define FABER_COMMAND_BUILDER_H

#include "FaberMessages.h"
#include "MessageBuilder.h"

#define FABER_COMMAND_CODE "faber:command_code"

class CommandBuilder : public MessageBuilder {
public:

	CommandBuilder(uint32 commandCode)
		:
		MessageBuilder(FABER_COMMAND)
	{
		msg->AddUInt32(FABER_COMMAND_CODE, commandCode);
	}

	uint32 commandCode;
};

#endif
