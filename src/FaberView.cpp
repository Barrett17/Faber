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

#include "EffectsManager.h"
#include "FaberDefs.h"
#include "MenuBuilder.h"


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
FaberView::BuildFileMenu()
{
	return MenuBuilder::BuildMenu(kFileMenu);
}


BMenu*
FaberView::BuildRecentMenu()
{
	return NULL;
}


BMenu*
FaberView::BuildEditMenu()
{
	return MenuBuilder::BuildMenu(kEditMenu);
}


BMenu*
FaberView::BuildTracksMenu()
{
	return MenuBuilder::BuildMenu(kTracksMenu);
}


BMenu*
FaberView::BuildEffectsMenu()
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
FaberView::BuildGenerateMenu()
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
FaberView::BuildEngineMenu()
{
	return MenuBuilder::MenuBuilder::BuildMenu(kEngineMenu);
}


BMenu*
FaberView::BuildProjectMenu()
{
	return MenuBuilder::MenuBuilder::BuildMenu(kProjectMenu);
}


BMenu*
FaberView::BuildHelpMenu()
{
	return MenuBuilder::MenuBuilder::BuildMenu(kHelpMenu);
}
