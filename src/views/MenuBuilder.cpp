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

#include "MenuBuilder.h"

#include <Application.h>
#include <MenuItem.h>
#include <ObjectList.h>

#include "CommandBuilder.h"
#include "EffectsManager.h"
#include "FaberShortcut.h"


FaberShortcut* gBind = FaberShortcut::Get();


BMenu*
MenuBuilder::BuildMenu(KeyBind* bind, MenuFilter* filter,
	 CustomMenuCreator* custom)
{
	if (bind == NULL)
		return NULL;

	// The first item describe the menu
	BMenu* menu = new BMenu(bind[0].label);

	if (filter != NULL)
		filter->FilterMenu(menu, bind[0].message);

	BObjectList<BMenu> menuList;
	menuList.AddItem(menu);

	for (int i = 1; bind[i].itemType != FABER_EOF; i++) {
		menu = menuList.ItemAt(menuList.CountItems()-1);

		if (bind[i].itemType == FABER_ITEM_START) {
			BMenu* subMenu = NULL;
			subMenu = new BMenu(bind[i].label);

			if (filter != NULL)
				filter->FilterMenu(subMenu, bind[i].message);

			menu->AddItem(subMenu);
			menuList.AddItem(subMenu);

		} else if (bind[i].itemType == FABER_ITEM_END) {
			if (menuList.CountItems() > 1)
				menuList.RemoveItemAt(menuList.CountItems()-1);
		} else if (bind[i].itemType == FABER_CUSTOM_ITEM) {
			if (custom == NULL)
				continue;

			BMenu* customMenu = custom->CreateCustomMenu(bind[i].message);

			if (customMenu != NULL)
				menu->AddItem(customMenu);

		} else if (bind[i].itemType == FABER_SUBITEM) {
			BMenuItem* item = BuildMenuItem(bind[i].message, bind[i].label);

			if (filter != NULL)
				filter->FilterItem(item, bind[i].message);

			menu->AddItem(item);
		} else if (bind[i].itemType == FABER_SPLITTER)
			menu->AddItem(new BSeparatorItem());
	}
	return menuList.ItemAt(0);
}


BMenuItem*
MenuBuilder::BuildMenuItem(uint32 message, const char* label)
{
	KeyBind* bind = gBind->FindKeyBind(message);

	char key = 0;
	int32 mod = 0;

	if (bind != NULL) {
		key = bind->key;
		mod = bind->mod;	
	}

	return new BMenuItem(label, CommandBuilder(message),
		key, mod);
}
