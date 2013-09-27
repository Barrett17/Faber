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
#include <ScrollView.h>


FaberView::FaberView()
	:
	BGroupView(B_VERTICAL, 0)
{
	fToolBar = new ToolBar();
	fToolBar->SetTool(Prefs.tool_mode);

	fTracksContainer = new TracksContainer();

	fInfoToolBar = new InfoToolBar();

	BScrollView* tracksScrollView = new BScrollView("scrollviewR",
		fTracksContainer, B_FOLLOW_ALL, true, true);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.Add(fToolBar)
		.Add(tracksScrollView)
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
