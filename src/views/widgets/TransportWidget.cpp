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

#include "TransportWidget.h"

#include "FaberDefs.h"
#include "CommandBuilder.h"
#include "WidgetFrame.h"

#include <LayoutBuilder.h>


TransportWidget::TransportWidget()
	:
	FaberWidget(B_HORIZONTAL)
{
	WidgetFrame* transportView = new WidgetFrame(NULL);

	fPlayButton = BuildButton(B_TRANSLATE("Play"),
		CommandBuilder(FABER_TRANSPORT_PLAY), kPlayIcon);

	//fPauseButton = _BuildButton(B_TRANSLATE("Pause"),
	//	new BMessage(FABER_TRANSPORT_PAUSE), kPauseIcon);

	fStopButton = BuildButton(B_TRANSLATE("Stop"),
		CommandBuilder(FABER_TRANSPORT_STOP), kStopIcon);

	fLoopButton = BuildButton(B_TRANSLATE("Loop"),
		CommandBuilder(FABER_TRANSPORT_LOOP), kLoopIcon);

	BLayoutBuilder::Group<>(transportView, B_HORIZONTAL, 0)
			.AddGlue(0.0f)
			.Add(fStopButton)
			.Add(fPlayButton)
			.Add(BuildButton(B_TRANSLATE("Record"),
				CommandBuilder(FABER_TRANSPORT_REC), kRecordIcon))
			.Add(BuildButton(B_TRANSLATE("Rewind All"),
				CommandBuilder(FABER_TRANSPORT_REW_ALL), kSeekBackwardAllIcon))
			.Add(BuildButton(B_TRANSLATE("Rewind"),
				CommandBuilder(FABER_TRANSPORT_REW), kSeekBackwardIcon))
			.Add(BuildButton(B_TRANSLATE("Forward"),
				CommandBuilder(FABER_TRANSPORT_FWD), kSeekForwardIcon))
			.Add(BuildButton(B_TRANSLATE("Forward All"),
				CommandBuilder(FABER_TRANSPORT_FWD_ALL), kSeekForwardAllIcon))
			.Add(fLoopButton)
			.AddGlue(0.0f)
		.End();
	transportView->SetExplicitSize(BSize(270, 40));

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
		.Add(transportView);
}


TransportWidget::~TransportWidget()
{
}


void
TransportWidget::SetPlay(bool play)
{
	/*if (play == true) {
		fStopButton->SetPressed(false);
		//fPlayButton->SetMessage(new BMessage(TRANSPORT_PAUSE));
		fPlayButton->SetIcon(kPauseIcon);
		fPlayButton->TrimIcon();
	}*/
}


bool
TransportWidget::IsPlay() const
{
	return fPlaying;
}


void
TransportWidget::SetStop(bool stop)
{
	/*if (stop == true) {
		fPlayButton->SetMessage(FABER_TRANSPORT_PLAY);
		fPlayButton->SetIcon(kPlayIcon);
		fPlayButton->TrimIcon();
		fPlayButton->Invalidate();
	}*/
}


bool
TransportWidget::IsStop() const
{
	return fStopButton->IsPressed();
}


void
TransportWidget::SetPause(bool pause)
{
	/*if (pause == true) {
		fStopButton->SetPressed(false);
		fPlayButton->SetIcon(kPlayIcon);
		fPlayButton->TrimIcon();
		fPlaying = false;
		return;
	}
	fPlaying = true;
	fPlayButton->SetIcon(kPauseIcon);
	fPlayButton->TrimIcon();*/
}


bool
TransportWidget::IsPause() const
{
	return !fPlaying;
}


void
TransportWidget::SetLoop(bool loop)
{
	fLoopButton->SetPressed(loop);
}


bool
TransportWidget::IsLoop() const
{
	return fLoopButton->IsPressed();
}

