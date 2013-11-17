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

#include <GroupLayoutBuilder.h>

#include "Behavior.h"
#include "ColorScheme.h"
#include "Engine.h"
#include "FaberDefs.h"
#include "Interface.h"
#include "Keymap.h"


SettingsView::SettingsView()
	:
	BGroupView(B_HORIZONTAL, B_FOLLOW_LEFT)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	fTabView = new BTabView("");

	_AddTab(new EngineView(), B_TRANSLATE("Engine"));
	_AddTab(new BehaviorView(), B_TRANSLATE("Behavior"));
	_AddTab(new InterfaceView(), B_TRANSLATE("Interface"));
	_AddTab(new KeymapView(), B_TRANSLATE("Keymap"));
	_AddTab(new ColorSchemeView(), B_TRANSLATE("Color Scheme"));

	BGroupLayoutBuilder(this)
		.Add(fTabView)
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
