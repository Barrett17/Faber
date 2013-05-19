/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include "ToolBar.h"

#include <LayoutBuilder.h>
#include <TranslationUtils.h>

#include "FaberResources.h"
#include "IconButton.h"


ToolBar::ToolBar()
	:
	BView(BRect(0,10, 1000, 60), "ToolBar", B_HORIZONTAL, B_WILL_DRAW)
{
	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 1)
	 	.Add(_BuildButton(B_TRANSLATE("New document"), new BMessage(NEW), kNewDocumentIcon))
	 	.Add(_BuildButton(B_TRANSLATE("Open file"), new BMessage(OPEN), kOpenDocumentIcon))
		.Add(_BuildButton(B_TRANSLATE("Insert at current location"), new BMessage(INSERT), 0))
		.Add(_BuildButton(B_TRANSLATE("Save file"), new BMessage(SAVE), kDocumentSaveIcon))
		.Add(_BuildButton(B_TRANSLATE("Save the current selection"), new BMessage(SAVE_SELECTION), 0))
		.AddStrut(10.0f)
		.Add(_BuildButton(B_TRANSLATE("Edit left channel"), new BMessage(EDIT_L), 0))
		.Add(_BuildButton(B_TRANSLATE("Edit right channel"), new BMessage(EDIT_R), 0))
		.Add(_BuildButton(B_TRANSLATE("Edit both channels"), new BMessage(EDIT_B), 0))
		.Add(_BuildButton(B_TRANSLATE("Undo"), new BMessage(UNDO), 0))
		.Add(_BuildButton(B_TRANSLATE("Redo"), new BMessage(REDO), 0))
		.Add(_BuildButton(B_TRANSLATE("Cut"), new BMessage(B_CUT), kCutIcon))
		.Add(_BuildButton(B_TRANSLATE("Copy"), new BMessage(B_COPY), 0))
		.Add(_BuildButton(B_TRANSLATE("Paste"), new BMessage(B_PASTE), 0))
	 	.Add(_BuildButton(B_TRANSLATE("Paste and mix with current wave"), new BMessage(PASTE_MIXED), 0))
		.Add(_BuildButton(B_TRANSLATE("Copy to wave-stack"), new BMessage(TO_STACK), 0))
		//.Add(_BuildButton(B_TRANSLATE("SET_LOOP"), new BMessage(SET_LOOP), NULL));
		.AddStrut(15.0f)
		// TODO those should be B_TWO_STATE_BUTTON
		.Add(_BuildButton(B_TRANSLATE("Selection Tool"), new BMessage(TOOL_SELECT), 0))
		.Add(_BuildButton(B_TRANSLATE("Drawing Tool"), new BMessage(TOOL_DRAW), 0))
		.Add(_BuildButton(B_TRANSLATE("Playing Tool"), new BMessage(TOOL_PLAY), 0))
		.Add(_BuildButton(B_TRANSLATE("Jogg Wheel Tool"), new BMessage(TOOL_JOGG), 0))
		.End();
}


ToolBar::~ToolBar()
{

}


void
ToolBar::Update()
{
}


IconButton*
ToolBar::_BuildButton(const char* tip, BMessage* message, int32 resourceID)
{
	IconButton* button = new IconButton(NULL, 0, NULL, message, this);
	// Well those could go into the constructor, but no reason for now.
	button->SetToolTip(tip);
	button->SetIcon(resourceID);
	button->TrimIcon();
	return button;
}
