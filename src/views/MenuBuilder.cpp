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
#include "RecentItems.h"


FaberShortcut* gBind = FaberShortcut::Get();


BMenu*
MenuBuilder::BuildMenu(KeyBind* bind, BHandler* target)
{
	if (bind == NULL)
		return NULL;

	// The first item describe the menu
	BMenu* menu = new BMenu(bind[0].label);
	BObjectList<BMenu> menuList;
	menuList.AddItem(menu);

	for (int i = 1; bind[i].message != FABER_EOF; i++) {

		menu = menuList.ItemAt(menuList.CountItems()-1);

		if (bind[i].message == FABER_ITEM_START) {
			BMenu* subMenu = NULL;
			subMenu = new BMenu(bind[i].label);
			menu->AddItem(subMenu);
			menuList.AddItem(subMenu);
		}  else if (bind[i].message == FABER_ITEM_END) {
			if (menuList.CountItems() > 1)
				menuList.RemoveItemAt(menuList.CountItems()-1);
		} else {
			// NOTE This is a temporarily hack
			if (bind[i].message == FABER_FILE_OPEN) {
				menu->AddItem(new BMenuItem(BRecentFilesList::NewFileListMenu(
					B_TRANSLATE("Open"B_UTF8_ELLIPSIS), NULL, NULL, be_app, 9, true,
					NULL, FABER_SIGNATURE), CommandBuilder(FABER_FILE_OPEN)));
			} else {
				BMenuItem* item = BuildMenuItem(bind[i].message, bind[i].label);
				if (target != NULL)
					item->SetTarget(target);

				menu->AddItem(item);
			}
		}
	}
	return menuList.ItemAt(0);
}


BPopUpMenu*
MenuBuilder::BuildPopUpMenu(KeyBind* bind, BHandler* target)
{
	if (bind == NULL)
		return NULL;

	// The first item describe the menu
	BPopUpMenu* firstMenu = new BPopUpMenu(bind[0].label);
	BObjectList<BMenu> menuList;
	menuList.AddItem((BMenu*)firstMenu);

	BMenu* menu = NULL;

	for (int i = 1; bind[i].message != FABER_EOF; i++) {

		menu = menuList.ItemAt(menuList.CountItems()-1);

		if (bind[i].message == FABER_ITEM_START) {
			BMenu* subMenu = new BMenu(bind[i].label);
			menu->AddItem(subMenu);
			menuList.AddItem(subMenu);
		}  else if (bind[i].message == FABER_ITEM_END) {
			if (menuList.CountItems() > 1)
				menuList.RemoveItemAt(menuList.CountItems()-1);
		} else {
			BMenuItem* item = BuildMenuItem(bind[i].message, bind[i].label);
			if (target != NULL)
				item->SetTarget(target);

			menu->AddItem(item);
		}
	}

	return (BPopUpMenu*) menuList.ItemAt(0);
}


BMenuItem*
MenuBuilder::BuildMenuItem(uint32 message, const char* label)
{
	if (message == FABER_SPLITTER)
		return new BSeparatorItem();
	else
		return new BMenuItem(label, CommandBuilder(message),
			gBind->GetKey(message), gBind->GetMod(message));
}
