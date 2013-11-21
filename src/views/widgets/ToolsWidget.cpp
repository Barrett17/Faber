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

#include "ToolsWidget.h"

#include "FaberDefs.h"
#include "WidgetFrame.h"

#include <LayoutBuilder.h>


ToolsWidget::ToolsWidget()
	:
	FaberWidget(B_HORIZONTAL)
{
	fToolButtons[0] = BuildButton(B_TRANSLATE("Selection Tool"),
		MessageBuilder(FABER_SELECTION_TOOL), kSelectionToolIcon);

	fToolButtons[1] = BuildButton(B_TRANSLATE("Drawing Tool"),
		MessageBuilder(FABER_DRAW_TOOL), kDrawingToolIcon);

	fToolButtons[2] = BuildButton(B_TRANSLATE("Playing Tool"),
		MessageBuilder(FABER_PLAY_TOOL), kPlayingToolIcon);

	WidgetFrame* toolsView = new WidgetFrame(NULL);
	BLayoutBuilder::Group<>(toolsView, B_HORIZONTAL, 0)
			.AddStrut(5.0f)
			.Add(fToolButtons[0])
			.Add(fToolButtons[1])
			.Add(fToolButtons[2])
			.AddStrut(5.0f)
		.End();

	toolsView->SetExplicitSize(BSize(100, 40));

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
		.Add(toolsView);
}


ToolsWidget::~ToolsWidget()
{
}


void
ToolsWidget::SetTool(const uint32 tool)
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
