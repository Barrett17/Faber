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
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.AddGroup(B_HORIZONTAL, 0)
		 	.Add(_BuildButton(B_TRANSLATE("New document"), new BMessage(NEW), kNewDocumentIcon))
		 	.Add(_BuildButton(B_TRANSLATE("Open file"), new BMessage(OPEN), kOpenDocumentIcon))
			.Add(_BuildButton(B_TRANSLATE("Insert at current location"), new BMessage(INSERT), kInsertAtLocationIcon))
			.Add(_BuildButton(B_TRANSLATE("Save file"), new BMessage(SAVE), kDocumentSaveIcon))
			.Add(_BuildButton(B_TRANSLATE("Save the current selection"), new BMessage(SAVE_SELECTION), kSaveSelectionIcon))
			.AddStrut(10.0f)
			.Add(_BuildButton(B_TRANSLATE("Edit left channel"), new BMessage(EDIT_L), kEditLeftChannelIcon))
			.Add(_BuildButton(B_TRANSLATE("Edit right channel"), new BMessage(EDIT_R), 0))
			.Add(_BuildButton(B_TRANSLATE("Edit both channels"), new BMessage(EDIT_B), 0))
			.AddStrut(10.0f)
			.Add(_BuildButton(B_TRANSLATE("Undo"), new BMessage(UNDO), kUndoIcon))
			.Add(_BuildButton(B_TRANSLATE("Redo"), new BMessage(REDO), kRedoIcon))
			.Add(_BuildButton(B_TRANSLATE("Cut"), new BMessage(B_CUT), kCutIcon))
			.Add(_BuildButton(B_TRANSLATE("Copy"), new BMessage(B_COPY), kCopyIcon))
			.Add(_BuildButton(B_TRANSLATE("Paste"), new BMessage(B_PASTE), kPasteIcon))
		 	.Add(_BuildButton(B_TRANSLATE("Paste and mix with current wave"), new BMessage(PASTE_MIXED), kPasteIcon))
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
			.Add(_BuildButton(B_TRANSLATE("Stop"), new BMessage(TRANSPORT_STOP), kStopIcon))
			//stop->SetValue(B_CONTROL_ON);
			.Add(_BuildButton(B_TRANSLATE("Play"), new BMessage(TRANSPORT_PLAY), kPlayIcon))
			.Add(_BuildButton(B_TRANSLATE("Plays"),new BMessage(TRANSPORT_PLAYS), kPlayIcon))
			.Add(_BuildButton(B_TRANSLATE("Pause"), new BMessage(TRANSPORT_PAUSE), kPauseIcon))
			.Add(_BuildButton(B_TRANSLATE("Record"), new BMessage(TRANSPORT_REC), kRecordIcon))
			.Add(_BuildButton(B_TRANSLATE("Rewind All"), new BMessage(TRANSPORT_REW_ALL), kSeekBackwardAllIcon))
			.Add(_BuildButton(B_TRANSLATE("Rewind"), new BMessage(TRANSPORT_REW), kSeekBackwardIcon))
			.Add(_BuildButton(B_TRANSLATE("Forward"), new BMessage(TRANSPORT_FWD), kSeekForwardIcon))
			.Add(_BuildButton(B_TRANSLATE("Forward All"), new BMessage(TRANSPORT_FWD_ALL), kSeekForwardAllIcon))
			.Add(_BuildButton(B_TRANSLATE("Loop"), new BMessage(TRANSPORT_LOOP), kLoopIcon))
			.AddStrut(15.0f)
			.Add(_BuildButton(B_TRANSLATE("Zoom in"), new BMessage(ZOOM_IN), kZoomInIcon))
			.Add(_BuildButton(B_TRANSLATE("Zoom out"), new BMessage(ZOOM_OUT), kZoomOutIcon))
			.Add(_BuildButton(B_TRANSLATE("Zoom to selection"), new BMessage(ZOOM_SELECTION), kZoomToSelectionIcon))
			.Add(_BuildButton(B_TRANSLATE("Zoom full wave"), new BMessage(ZOOM_FULL), 0))
			.Add(_BuildButton(B_TRANSLATE("Zoom to left pointer"), new BMessage(ZOOM_LEFT), 0))
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
