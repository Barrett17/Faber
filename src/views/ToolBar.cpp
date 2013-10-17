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

#include "ToolBar.h"

#include <LayoutBuilder.h>
#include <StringView.h>

#include "DurationView.h"
#include "FaberDefs.h"
#include "IconButton.h"
#include "WidgetFrame.h"


ToolBar::ToolBar()
	:
	BView("ToolBar", B_HORIZONTAL)
{
	fOutputPeakView = new PeakView("OutputPeakView", true, false);
	fOutputPeakView->SetExplicitMaxSize(BSize(150, 20));

	fPlayButton = _BuildButton(B_TRANSLATE("Play"),
		new BMessage(FABER_TRANSPORT_PLAY), kPlayIcon);
	//fPauseButton = _BuildButton(B_TRANSLATE("Pause"),
	//	new BMessage(FABER_TRANSPORT_PAUSE), kPauseIcon);
	fStopButton = _BuildButton(B_TRANSLATE("Stop"),
		new BMessage(FABER_TRANSPORT_STOP), kStopIcon);
	fLoopButton = _BuildButton(B_TRANSLATE("Loop"),
		new BMessage(FABER_TRANSPORT_LOOP), kLoopIcon);

	fToolButtons[0] = _BuildButton(B_TRANSLATE("Selection Tool"),
		new BMessage(FABER_SELECTION_TOOL), kSelectionToolIcon);

	fToolButtons[1] = _BuildButton(B_TRANSLATE("Drawing Tool"),
		new BMessage(FABER_DRAW_TOOL), kDrawingToolIcon);

	fToolButtons[2] = _BuildButton(B_TRANSLATE("Playing Tool"),
		new BMessage(FABER_PLAY_TOOL), kPlayingToolIcon);

	WidgetFrame* transportView = new WidgetFrame(NULL);
	BLayoutBuilder::Group<>(transportView, B_HORIZONTAL, 0)
			.AddGlue(0.0f)
			.Add(fStopButton)
			.Add(fPlayButton)
			.Add(_BuildButton(B_TRANSLATE("Record"),
				new BMessage(FABER_TRANSPORT_REC), kRecordIcon))
			.Add(_BuildButton(B_TRANSLATE("Rewind All"),
				new BMessage(FABER_TRANSPORT_REW_ALL), kSeekBackwardAllIcon))
			.Add(_BuildButton(B_TRANSLATE("Rewind"),
				new BMessage(FABER_TRANSPORT_REW), kSeekBackwardIcon))
			.Add(_BuildButton(B_TRANSLATE("Forward"),
				new BMessage(FABER_TRANSPORT_FWD), kSeekForwardIcon))
			.Add(_BuildButton(B_TRANSLATE("Forward All"),
				new BMessage(FABER_TRANSPORT_FWD_ALL), kSeekForwardAllIcon))
			.Add(fLoopButton)
			.AddGlue(0.0f)
		.End();
	transportView->SetExplicitSize(BSize(270, 40));

	WidgetFrame* toolsView = new WidgetFrame(NULL);
	BLayoutBuilder::Group<>(toolsView, B_HORIZONTAL, 0)
			.AddStrut(5.0f)
			.Add(fToolButtons[0])
			.Add(fToolButtons[1])
			.Add(fToolButtons[2])
			.AddStrut(5.0f)
		.End();
	toolsView->SetExplicitSize(BSize(100, 40));

	DurationView* durationView = new DurationView();

	fOutputPeakView->SetExplicitSize(BSize(150, 13));

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
			.AddGlue()
			.Add(transportView)
			.AddStrut(15.0f)

			.AddGroup(B_VERTICAL, 0.5f)
				.Add(fOutputPeakView)
				.Add(durationView)
			.End()

			.AddGlue()

			.Add(toolsView)
			.AddStrut(2.0f)
		.End();

}


ToolBar::~ToolBar()
{

}


void
ToolBar::Update()
{
}


void
ToolBar::SetPlay(bool play)
{
	if (play == true) {
		fStopButton->SetPressed(false);
		//fPlayButton->SetMessage(new BMessage(TRANSPORT_PAUSE));
		fPlayButton->SetIcon(kPauseIcon);
		fPlayButton->TrimIcon();
	}
}


bool
ToolBar::IsPlay() const
{
	return fPlaying;
}


void
ToolBar::SetStop(bool stop)
{
	if (stop == true) {
		fPlayButton->SetMessage(new BMessage(FABER_TRANSPORT_PLAY));
		fPlayButton->SetIcon(kPlayIcon);
		fPlayButton->TrimIcon();
		fPlayButton->Invalidate();
	}
}


bool
ToolBar::IsStop() const
{
	return fStopButton->IsPressed();
}


void
ToolBar::SetPause(bool pause)
{
	if (pause == true) {
		fStopButton->SetPressed(false);
		fPlayButton->SetIcon(kPlayIcon);
		fPlayButton->TrimIcon();
		fPlaying = false;
		return;
	}
	fPlaying = true;
	fPlayButton->SetIcon(kPauseIcon);
	fPlayButton->TrimIcon();
}


bool
ToolBar::IsPause() const
{
	return !fPlaying;
}


void
ToolBar::SetLoop(bool loop)
{
	fLoopButton->SetPressed(loop);
}


bool
ToolBar::IsLoop() const
{
	return fLoopButton->IsPressed();
}


void
ToolBar::SetTool(const uint32 tool)
{
	for (int i = 0; i < 3; i++) {
		if (i == tool) {
			fToolButtons[i]->SetPressed(true);			
			fToolButtons[i]->SetEnabled(false);
			continue;
		}
		fToolButtons[i]->SetPressed(false);			
		fToolButtons[i]->SetEnabled(true);
	}
}


IconButton*
ToolBar::_BuildButton(const char* tip, BMessage* message, int32 resourceID)
{
	IconButton* button = new IconButton(NULL, 0, NULL, message, this, true);
	// Well those could go into the constructor, but no reason for now.
	button->SetToolTip(tip);
	button->SetIcon(resourceID);
	button->TrimIcon();
	return button;
}
