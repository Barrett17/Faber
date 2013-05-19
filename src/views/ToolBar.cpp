/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include "ToolBar.h"

#include <LayoutBuilder.h>

#include "FaberResources.h"
#include "IconButton.h"


ToolBar::ToolBar()
	:
	BView("ToolBar", B_HORIZONTAL)
{
	BLayoutBuilder::Group<>(this, B_VERTICAL, 3)
		.AddGroup(B_HORIZONTAL, 0)
		 	.Add(_BuildButton(B_TRANSLATE("New document"), new BMessage(NEW), kNewDocumentIcon))
		 	.Add(_BuildButton(B_TRANSLATE("Open file"), new BMessage(OPEN), kOpenDocumentIcon))
			.Add(_BuildButton(B_TRANSLATE("Insert at current location"), new BMessage(INSERT), 0))
			.Add(_BuildButton(B_TRANSLATE("Save file"), new BMessage(SAVE), kDocumentSaveIcon))
			.Add(_BuildButton(B_TRANSLATE("Save the current selection"), new BMessage(SAVE_SELECTION), 0))
			.AddStrut(10.0f)
			.Add(_BuildButton(B_TRANSLATE("Edit left channel"), new BMessage(EDIT_L), 0))
			.Add(_BuildButton(B_TRANSLATE("Edit right channel"), new BMessage(EDIT_R), 0))
			.Add(_BuildButton(B_TRANSLATE("Edit both channels"), new BMessage(EDIT_B), 0))
			.AddStrut(10.0f)
			.Add(_BuildButton(B_TRANSLATE("Undo"), new BMessage(UNDO), 0))
			.Add(_BuildButton(B_TRANSLATE("Redo"), new BMessage(REDO), 0))
			.Add(_BuildButton(B_TRANSLATE("Cut"), new BMessage(B_CUT), kCutIcon))
			.Add(_BuildButton(B_TRANSLATE("Copy"), new BMessage(B_COPY), 0))
			.Add(_BuildButton(B_TRANSLATE("Paste"), new BMessage(B_PASTE), 0))
		 	.Add(_BuildButton(B_TRANSLATE("Paste and mix with current wave"), new BMessage(PASTE_MIXED), 0))
			/* Unimplemented, disabled, feel free to remove

			.Add(_BuildButton(B_TRANSLATE("Copy to wave-stack"), new BMessage(TO_STACK), 0))
			.Add(_BuildButton(B_TRANSLATE("SET_LOOP"), new BMessage(SET_LOOP), NULL));
			*/

			.AddStrut(10.0f)
			// TODO those should be B_TWO_STATE_BUTTON
			.Add(_BuildButton(B_TRANSLATE("Selection Tool"), new BMessage(TOOL_SELECT), 0))
			.Add(_BuildButton(B_TRANSLATE("Drawing Tool"), new BMessage(TOOL_DRAW), 0))
			.Add(_BuildButton(B_TRANSLATE("Playing Tool"), new BMessage(TOOL_PLAY), kPlayIcon))
			.Add(_BuildButton(B_TRANSLATE("Jogg Wheel Tool"), new BMessage(TOOL_JOGG), kPlayIcon))
			.AddGlue()
		.End()
		.AddGroup(B_HORIZONTAL, 0)
			.Add(_BuildButton(B_TRANSLATE("Stop"), new BMessage(TRANSPORT_STOP), 0))
			//stop->SetValue(B_CONTROL_ON);
			.Add(_BuildButton(B_TRANSLATE("Play"), new BMessage(TRANSPORT_PLAY), 0))
			.Add(_BuildButton(B_TRANSLATE("Plays"),new BMessage(TRANSPORT_PLAYS), 0))
			.Add(_BuildButton(B_TRANSLATE("Pause"), new BMessage(TRANSPORT_PAUSE), 0))
			.Add(_BuildButton(B_TRANSLATE("Record"), new BMessage(TRANSPORT_REC), 0))
			.Add(_BuildButton(B_TRANSLATE("Rewind All"), new BMessage(TRANSPORT_REW_ALL), 0))
			.Add(_BuildButton(B_TRANSLATE("Rewind"), new BMessage(TRANSPORT_REW), 0))
			.Add(_BuildButton(B_TRANSLATE("Forward"), new BMessage(TRANSPORT_FWD),0))
			.Add(_BuildButton(B_TRANSLATE("Forward All"), new BMessage(TRANSPORT_FWD_ALL), 0))
			.Add(_BuildButton(B_TRANSLATE("Loop"), new BMessage(TRANSPORT_LOOP), 0))
			.AddStrut(15.0f)
			.Add(_BuildButton(B_TRANSLATE("Zoom in"), new BMessage(ZOOM_IN), 0))
			.Add(_BuildButton(B_TRANSLATE("Zoom to selection"), new BMessage(ZOOM_SELECTION), 0))
			.Add(_BuildButton(B_TRANSLATE("Zoom to left pointer"), new BMessage(ZOOM_LEFT), 0))
			.Add(_BuildButton(B_TRANSLATE("Zoom to right pointer"), new BMessage(ZOOM_OUT), 0))
			.Add(_BuildButton(B_TRANSLATE("Zoom full wave"), new BMessage(ZOOM_FULL), 0))
			.Add(_BuildButton(B_TRANSLATE("Zoom to right pointer"), new BMessage(ZOOM_RIGHT), 0))
			.AddGlue()
			.AddSplit(B_VERTICAL, 50.0f)
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
