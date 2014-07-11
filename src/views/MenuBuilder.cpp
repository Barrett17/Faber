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

#include "EffectsManager.h"
#include "FaberEffect.h"
#include "RecentItems.h"

MenuBuilder* MenuBuilder::fInstance = NULL;


MenuBuilder*
MenuBuilder::Get()
{
	if (fInstance == NULL)
		fInstance = new MenuBuilder();

	return fInstance;	
}


MenuBuilder::MenuBuilder()
{
	fKeyBind = FaberShortcut::Get();
	fKeyBind->CreateDefaultKeys();
}


MenuBuilder::~MenuBuilder()
{

}


BMenu*
MenuBuilder::BuildMenu(KeyBind* bind, BView* target)
{
	if (bind == NULL)
		return NULL;

	// The first item describe the menu
	BMenu* menu = new BMenu(bind[0].label);
	BObjectList<BMenu> menuList(false);
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
				printf("a\n");
				menu->AddItem(new BMenuItem(BRecentFilesList::NewFileListMenu(
					B_TRANSLATE("Open"B_UTF8_ELLIPSIS), NULL, NULL, be_app, 9, true,
					NULL, FABER_SIGNATURE), new BMessage(FABER_FILE_OPEN)));
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
MenuBuilder::BuildPopUpMenu(KeyBind* bind, BView* target)
{
	if (bind == NULL)
		return NULL;

	// The first item describe the menu
	BPopUpMenu* menu = new BPopUpMenu(bind[0].label);
	BObjectList<BPopUpMenu> menuList(false);
	menuList.AddItem(menu);

	for (int i = 1; bind[i].message != FABER_EOF; i++) {

		menu = menuList.ItemAt(menuList.CountItems()-1);

		if (bind[i].message == FABER_ITEM_START) {
			BPopUpMenu* subMenu = new BPopUpMenu(bind[i].label);
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
	return menuList.ItemAt(0);
}


BMenuItem*
MenuBuilder::BuildMenuItem(uint32 message, const char* label)
{
	if (message == FABER_SPLITTER)
		return new BSeparatorItem();
	else
		return new BMenuItem(label, MessageBuilder(message),
			fKeyBind->GetKey(message), fKeyBind->GetMod(message));
}


BMenuBar*
MenuBuilder::BuildMainMenuBar()
{
	BMenuBar* menuBar = new BMenuBar("MainMenuBar");

	menuBar->AddItem(BuildFileMenu());
	menuBar->AddItem(BuildEditMenu());
	menuBar->AddItem(BuildProjectMenu());
	menuBar->AddItem(BuildTracksMenu());
	menuBar->AddItem(BuildEffectsMenu());
	menuBar->AddItem(BuildGenerateMenu());
	menuBar->AddItem(BuildEngineMenu());
	menuBar->AddItem(BuildHelpMenu());

	return menuBar;
}


BMenu*
MenuBuilder::BuildFileMenu()
{
	return BuildMenu(kFileMenu);
}


BMenu*
MenuBuilder::BuildRecentMenu()
{
	return NULL;
}


BMenu*
MenuBuilder::BuildEditMenu()
{
	return BuildMenu(kEditMenu);
}


BMenu*
MenuBuilder::BuildTracksMenu()
{
	return BuildMenu(kTracksMenu);
}


BMenu*
MenuBuilder::BuildEffectsMenu()
{
	BMenu* menu = new BMenu(B_TRANSLATE("Effects"));
	EffectsManager* effectsManager = EffectsManager::Get();

	for (int i = 0; i < effectsManager->CountEffects(); i++) {
		FaberEffect* effect = effectsManager->GetEffect(i);
		if (effect->Flags() & FABER_FILTER) {
			BMenuItem* item = effect->BuildItem();
			menu->AddItem(item);
		}
	}

	return menu;
}


BMenu*
MenuBuilder::BuildGenerateMenu()
{
	BMenu* menu = new BMenu(B_TRANSLATE("Generate"));
	EffectsManager* effectsManager = EffectsManager::Get();

	for (int i = 0; i < effectsManager->CountEffects(); i++) {
		FaberEffect* effect = effectsManager->GetEffect(i);
		if (effect->Flags() & FABER_PRODUCER) {
			BMenuItem* item = effect->BuildItem();
			menu->AddItem(item);
		}
	}

	return menu;
}


BMenu*
MenuBuilder::BuildEngineMenu()
{
	return BuildMenu(kEngineMenu);
}


BMenu*
MenuBuilder::BuildProjectMenu()
{
	return BuildMenu(kProjectMenu);
}


BMenu*
MenuBuilder::BuildHelpMenu()
{
	return BuildMenu(kHelpMenu);
}


BPopUpMenu*
MenuBuilder::BuildTrackContextualMenu(BView* target)
{
	return BuildPopUpMenu(kTrackContextualMenu, target);
}
