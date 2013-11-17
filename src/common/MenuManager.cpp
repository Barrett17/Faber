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

#include "MenuManager.h"

#include <MenuItem.h>
#include <ObjectList.h>

#include "EffectsManager.h"
#include "FaberEffect.h"

MenuManager* MenuManager::fInstance = NULL;


MenuManager*
MenuManager::Get()
{
	if (fInstance == NULL)
		fInstance = new MenuManager();

	return fInstance;	
}


MenuManager::MenuManager()
{
	fKeyBind = FaberShortcut::Get();
	fKeyBind->CreateDefaultKeys();
}


MenuManager::~MenuManager()
{

}


BMenuBar*
MenuManager::BuildMainMenuBar()
{
	BMenuBar* menuBar = new BMenuBar("MainMenuBar");

	menuBar->AddItem(BuildFileMenu());
	menuBar->AddItem(BuildEditMenu());
	menuBar->AddItem(BuildTracksMenu());
	menuBar->AddItem(BuildEffectsMenu());
	menuBar->AddItem(BuildGenerateMenu());
	menuBar->AddItem(BuildHelpMenu());

	return menuBar;
}


BMenu*
MenuManager::BuildFileMenu()
{
	return _BuildMenu(kFileMenu, B_TRANSLATE("File"));
}


BMenu*
MenuManager::BuildEditMenu()
{
	return _BuildMenu(kEditMenu, B_TRANSLATE("Edit"));
}


BMenu*
MenuManager::BuildTracksMenu()
{
	return _BuildMenu(kTracksMenu, B_TRANSLATE("Tracks"));
}


BMenu*
MenuManager::BuildEffectsMenu()
{
	BMenu* menu = new BMenu(B_TRANSLATE("Effects"));
	EffectsManager* effectsManager = EffectsManager::Get();

	for (int i = 0; i < effectsManager->CountEffects(); i++) {
		FaberEffect* effect = effectsManager->GetEffect(i);
		if (effect->Flags() & FABER_FILTER)
			menu->AddItem(effect->BuildItem());
	}

	return menu;
}


BMenu*
MenuManager::BuildGenerateMenu()
{
	BMenu* menu = new BMenu(B_TRANSLATE("Generate"));
	EffectsManager* effectsManager = EffectsManager::Get();

	for (int i = 0; i < effectsManager->CountEffects(); i++) {
		FaberEffect* effect = effectsManager->GetEffect(i);
		if (effect->Flags() & FABER_PRODUCER)
			menu->AddItem(effect->BuildItem());
	}

	return menu;
}


BMenu*
MenuManager::BuildHelpMenu()
{
	return _BuildMenu(kHelpMenu, "Help");
}


BPopUpMenu*
MenuManager::BuildTrackContextualMenu()
{
	BPopUpMenu* trackMenu = new BPopUpMenu("TrackMenu");

	trackMenu->AddItem(new BMenuItem("Set Name...",
		NULL));

	trackMenu->AddItem(new BSeparatorItem());

	trackMenu->AddItem(new BMenuItem("Mono",
		NULL));

	trackMenu->AddItem(new BMenuItem("Stereo (right)",
		NULL));

	trackMenu->AddItem(new BMenuItem("Stereo (left)",
		NULL));

	trackMenu->AddItem(new BSeparatorItem());

	trackMenu->AddItem(new BMenuItem("Select All",
		NULL));

	trackMenu->AddItem(new BSeparatorItem());

	trackMenu->AddItem(new BMenuItem("Create two mono tracks",
		NULL));
	trackMenu->AddItem(new BMenuItem("Separate stereo track",
		NULL));
	trackMenu->AddItem(new BMenuItem("Merge adiacent track",
		NULL));

	/*trackMenu->AddItem(new BSeparatorItem());

	trackMenu->AddItem(new BMenuItem(B_TRANSLATE("Insert..."),
		new BMessage(FABER_INSERT), fKeyBind->GetKey("FILE_INSERT"),
		fKeyBind->GetMod("FILE_INSERT")));

	trackMenu->AddItem(new BMenuItem(B_TRANSLATE("Append..."),
		new BMessage(FABER_APPEND), fKeyBind->GetKey("FILE_APPEND"),
		fKeyBind->GetMod("FILE_APPEND")));*/

	return trackMenu;
}


void
MenuManager::UpdateMenu()
{
}


BMenu*
MenuManager::_BuildMenu(KeyBind* bind, const char* name)
{
	// The first item describe the menu
	BMenu* menu = new BMenu(bind[0].label);
	BObjectList<BMenu> menuList(false);
	menuList.AddItem(menu);

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
			printf("%s %d\n", bind[i].label, bind[i].message.code);
			menu->AddItem(_BuildMenuItem(bind[i].message, bind[i].label));
		}
	}
	return menuList.ItemAt(0);
}


BMenuItem*
MenuManager::_BuildMenuItem(FaberMessage message, const char* label)
{
	if (message == FABER_SPLITTER)
		return new BSeparatorItem();
	else
		return new BMenuItem(label, message,
			fKeyBind->GetKey(message), fKeyBind->GetMod(message));
}
