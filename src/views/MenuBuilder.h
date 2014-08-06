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

#ifndef _MENUBUILDER_H_
#define _MENUBUILDER_H_

#include <Menu.h>
#include <MenuBar.h>
#include <PopUpMenu.h>

#include "DefaultKeymap.h"
#include "FaberDefs.h"


class CustomMenuCreator {
public:
	virtual BMenu*				CreateCustomMenu(uint32 message) {};
};

class MenuFilter {
public:
	virtual void				FilterMenu(BMenu* menu,
									uint32 message) {};

	virtual void				FilterItem(BMenuItem* item,
									uint32 message) {};
};

class MenuBuilder {
public:

			static BMenu*		BuildMenu(KeyBind* bind,
									MenuFilter* filter = NULL,
									CustomMenuCreator* custom = NULL);

			static void			FilterMenu(BMenu* target,
									MenuFilter* filter = NULL);

			static BMenuItem*	BuildMenuItem(uint32 message,
									const char* label);
};

#endif	// _MenuBuilder_H_
