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

#include "FaberView.h"

#include <Application.h>
#include <LayoutBuilder.h>
#include <StringView.h>

#include "CommandBuilder.h"
#include "EffectsManager.h"
#include "FaberDefs.h"
#include "MenuBuilder.h"
#include "RecentItems.h"


class RecentsMenuBuilder : public CustomMenuCreator {
	BMenu* CreateCustomMenu(uint32 message)
	{
		if (message == FABER_RECENTS_MENU) {
			return BRecentFilesList::NewFileListMenu(
				B_TRANSLATE("Recents"B_UTF8_ELLIPSIS),
				NULL, NULL, be_app, 9, true,
				NULL, FABER_SIGNATURE);
		}

		return NULL;
	}
};


FaberView::FaberView()
	:
	BGroupView(B_VERTICAL, 0)
{
	fToolBar = new ToolBar();

	fTracksContainer = new TracksContainer();

	fInfoToolBar = new InfoToolBar();

	rgb_color backgroundColor = {120,120,120};

	fMenuBar = BuildMainMenuBar();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(fToolBar)
		.AddStrut(0.0f)
			.AddGroup(B_VERTICAL, 0)
				.Add(fTracksContainer)
			.End()
		.AddStrut(10.0f)
		.Add(fInfoToolBar)
		.AddStrut(0.5f)
	.End();

	UpdateMenu();
	fTracksContainer->SetViewColor(backgroundColor);
}


FaberView::~FaberView()
{
}


TracksContainer*
FaberView::Container() const
{
	return fTracksContainer;
}


status_t
FaberView::AddTrack(Track* track)
{
	return fTracksContainer->AddTrack(track);
}


BMenuBar*
FaberView::BuildMainMenuBar()
{
	BMenuBar* menuBar = new BMenuBar("MainMenuBar");

	RecentsMenuBuilder builder;
	fFileMenu = MenuBuilder::BuildMenu(kFileMenu, NULL, &builder);
	menuBar->AddItem(fFileMenu);

	fEditMenu = MenuBuilder::BuildMenu(kEditMenu);
	menuBar->AddItem(fEditMenu);

	fProjectMenu = MenuBuilder::BuildMenu(kProjectMenu);
	menuBar->AddItem(fProjectMenu);

	fTracksMenu = MenuBuilder::BuildMenu(kTracksMenu);
	menuBar->AddItem(fTracksMenu);

	fEffectsMenu = BuildEffectsMenu();
	menuBar->AddItem(fEffectsMenu);

	fGenerateMenu = BuildGenerateMenu();
	menuBar->AddItem(fGenerateMenu);

	fEngineMenu = MenuBuilder::BuildMenu(kEngineMenu);
	menuBar->AddItem(fEngineMenu);

	menuBar->AddItem(MenuBuilder::BuildMenu(kHelpMenu));

	return menuBar;
}


void
FaberView::UpdateMenu()
{
	bool enableEdit = fTracksContainer->CountTracks() > 0;

	fFileMenu->ItemAt(5)->SetEnabled(enableEdit);
	fFileMenu->ItemAt(6)->SetEnabled(enableEdit);
	fFileMenu->ItemAt(8)->SetEnabled(enableEdit);

	fEditMenu->SetEnabled(enableEdit);

	fProjectMenu->ItemAt(0)->SetEnabled(enableEdit);

	fTracksMenu->ItemAt(1)->SetEnabled(enableEdit);
	fTracksMenu->ItemAt(3)->SetEnabled(enableEdit);
	fTracksMenu->ItemAt(4)->SetEnabled(enableEdit);

	fEffectsMenu->SetEnabled(enableEdit);
	fGenerateMenu->SetEnabled(enableEdit);
		
	fEngineMenu->SetEnabled(enableEdit);
}


BMenu*
FaberView::BuildEffectsMenu()
{
	BMenu* menu = new BMenu(B_TRANSLATE("Effects"));

	for (int i = 0; i < EffectsManager::CountEffects(); i++) {
		//printf("called\n");
		FaberEffect* effect = EffectsManager::GetEffect(i);
		if (effect->Flags() & FABER_FILTER) {
			BMenuItem* item = effect->BuildItem();
			menu->AddItem(item);
		}
	}

	return menu;
}


BMenu*
FaberView::BuildGenerateMenu()
{
	BMenu* menu = new BMenu(B_TRANSLATE("Generate"));

	for (int i = 0; i < EffectsManager::CountEffects(); i++) {
		FaberEffect* effect = EffectsManager::GetEffect(i);
		if (effect->Flags() & FABER_PRODUCER) {
			BMenuItem* item = effect->BuildItem();
			menu->AddItem(item);
		}
	}

	return menu;
}
