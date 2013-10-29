/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Casalinuovo Dario, b.vitruvio@gmail.com
 *		Davide Gessa, dak.linux@gmail.com
 */
#include "SettingsView.h"

#include <Button.h>
#include <GroupLayout.h>
#include <GroupView.h>
#include <View.h>
#include <Box.h>
#include <Catalog.h>
#include <CheckBox.h>
#include <Input.h>
#include <LayoutBuilder.h>
#include <SpaceLayoutItem.h>

#include "FaberDefs.h"
#include "General.h"
#include "Keymap.h"
#include "Colors.h"
#include "StringItem.h"


SettingsView::~SettingsView()
{

}


SettingsView::SettingsView()
	:
	BView("SettingsView", B_FOLLOW_ALL)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	// Main layout
	BGroupLayout* rootLayout = new BGroupLayout(B_VERTICAL);
	SetLayout(rootLayout);

	BRect r;
	BTab *tab;
	
	fTabView = new BTabView(r, " ");
	fTabView->SetViewColor(216,216,216,0);

	r = fTabView->Bounds();
	r.InsetBy(5,5);
	r.bottom -= fTabView->TabHeight();
	rootLayout->AddView(fTabView);

	// Interface
	BView *interfaceView = new PrefGeneral();
	
	tab = new BTab();
	fTabView->AddTab(interfaceView, tab);
	tab->SetLabel(B_TRANSLATE("Interface"));

	// Interface
	BView *colorView = new PrefColors();
	
	tab = new BTab();
	fTabView->AddTab(colorView, tab);
	tab->SetLabel(B_TRANSLATE("Color layout"));

	// Interface
	BView *hotkeyView = new PrefKeys();
	
	tab = new BTab();
	fTabView->AddTab(hotkeyView, tab);
	tab->SetLabel(B_TRANSLATE("Key bindings"));

	// Audio I/O
	BView *audioView = new BView(" ", B_WILL_DRAW,  0);
	
	tab = new BTab();
	fTabView->AddTab(audioView, tab);
	tab->SetLabel(B_TRANSLATE("Audio IO"));

	// Defaults and revert buttons
	BGroupView* buttonGroup = new BGroupView(B_HORIZONTAL);

	fDefaultsButton = new BButton(B_TRANSLATE("Defaults"),
		new BMessage(SET_FACTORY));						

	buttonGroup->GroupLayout()->AddView(fDefaultsButton);

	fRevertButton = new BButton(B_TRANSLATE("OK"),
		new BMessage(QUIT));							

	buttonGroup->GroupLayout()->AddView(fRevertButton);

	rootLayout->AddView(buttonGroup);
}


void SettingsView::AttachedToWindow()
{
	fDefaultsButton->SetTarget(this);
	fRevertButton->SetTarget(this);

	Window()->CenterOnScreen();
}


void SettingsView::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		default:
			BView::MessageReceived(message);
	}			
}
