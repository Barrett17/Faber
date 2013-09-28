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

#include <LayoutBuilder.h>
#include <ScrollBar.h>
#include <ScrollView.h>


FaberView::FaberView()
	:
	BGroupView(B_VERTICAL, 0)
{
	fToolBar = new ToolBar();
	fToolBar->SetTool(Prefs.tool_mode);

	fTracksContainer = new TracksContainer();

	fInfoToolBar = new InfoToolBar();

	BScrollView* verticalScrollView = new BScrollView("scrollviewV",
		fTracksContainer, B_FOLLOW_NONE, false, true);
	verticalScrollView->ScrollBar(B_VERTICAL)->SetRange(0, 0);

	// This is needed to stop the vertical scrollview to go out of the window
	verticalScrollView->SetExplicitMinSize(BSize(B_SIZE_UNSET, 200));


	BScrollBar* horizontalScrollBar = new BScrollBar("scrollviewH",
		fTracksContainer,
		0, 0, B_HORIZONTAL);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.Add(fToolBar)
		.AddGroup(B_VERTICAL)
			.Add(verticalScrollView)
			.Add(horizontalScrollBar)
		.End()
		.AddGlue(0.0f)
		.Add(fInfoToolBar)
	.End();
}


FaberView::~FaberView()
{
}


void
FaberView::MessageReceived(BMessage* message)
{
	switch (message->what)
	{

		default:
			BGroupView::MessageReceived(message);
	}
}


TracksContainer*
FaberView::Container() const
{
	return fTracksContainer;
}


bool
FaberView::IsEmpty() const
{
	return fTracksContainer->CountTracks() == 0;
}


bool
FaberView::IsSelected()
{
	
}


bool
FaberView::HasChanged()
{
	return fTracksContainer->HasChanged();
}


/*
void
FaberView::Pulse()
{
	fInfoToolBar->Pulse();
}*/
