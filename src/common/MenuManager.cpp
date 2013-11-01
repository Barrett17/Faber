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

#include "EffectsManager.h"
#include "FaberEffect.h"
#include "FaberDefs.h"
#include "Shortcut.h"

MenuManager* MenuManager::fInstance = NULL;


MenuManager*
MenuManager::Get()
{
	if (fInstance == NULL)
		fInstance = new MenuManager();

	return fInstance;	
}


BMenu*
MenuManager::BuildFileMenu()
{

}


BMenu*
MenuManager::BuildEditMenu()
{

}


BMenu*
MenuManager::BuildTracksMenu()
{

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

	trackMenu->AddItem(new BSeparatorItem());

	trackMenu->AddItem(new BMenuItem(B_TRANSLATE("Insert..."),
		new BMessage(FABER_INSERT), KeyBind.GetKey("FILE_INSERT"),
		KeyBind.GetMod("FILE_INSERT")));

	trackMenu->AddItem(new BMenuItem(B_TRANSLATE("Append..."),
		new BMessage(FABER_APPEND), KeyBind.GetKey("FILE_APPEND"),
		KeyBind.GetMod("FILE_APPEND")));

	return trackMenu;
}
