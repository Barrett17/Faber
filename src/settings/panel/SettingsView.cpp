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

#include "SettingsView.h"

#include <Button.h>
#include <GroupLayoutBuilder.h>

#include "Engine.h"
#include "FaberDefs.h"
#include "Interface.h"
#include "Keymap.h"


SettingsView::SettingsView()
	:
	BGroupView(B_VERTICAL, 0)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	fTabView = new BTabView("");

	_AddTab(new EngineView(), B_TRANSLATE("Engine"));
	_AddTab(new InterfaceView(), B_TRANSLATE("Interface"));
	_AddTab(new KeymapView(), B_TRANSLATE("Keymap"));

	BGroupLayoutBuilder(this)
		.Add(fTabView)
		.AddGroup(B_HORIZONTAL)
			.Add(new BButton(B_TRANSLATE("Apply"), new BMessage(FABER_APPLY)))
			.Add(new BButton(B_TRANSLATE("Revert"), new BMessage(FABER_REVERT)))
		.End()
	.End();

}


SettingsView::~SettingsView()
{

}


void
SettingsView::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		default:
			BGroupView::MessageReceived(message);
	}
}


void
SettingsView::_AddTab(BView* view, const char* label)
{
	BTab* tab = new BTab();

	fTabView->AddTab(view, tab);
	tab->SetLabel(label);
}
