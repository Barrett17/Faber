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

#include "ZoomWidget.h"

#include "CommandBuilder.h"
#include "FaberDefs.h"
#include "WidgetFrame.h"

#include <LayoutBuilder.h>


ZoomWidget::ZoomWidget()
	:
	FaberWidget(B_HORIZONTAL)
{
	WidgetFrame* zoomView = new WidgetFrame();

	BLayoutBuilder::Group<>(zoomView, B_HORIZONTAL, 0.5f)
		.AddStrut(7.0f)
		.Add(BuildButton(B_TRANSLATE("Zoom in"), 
			CommandBuilder(FABER_ZOOM_IN), kZoomInIcon))

		.Add(BuildButton(B_TRANSLATE("Zoom out"),
			CommandBuilder(FABER_ZOOM_OUT), kZoomOutIcon))

		.Add(BuildButton(B_TRANSLATE("Zoom to selection"),
			CommandBuilder(FABER_ZOOM_SELECTION), kZoomToSelectionIcon))

		.Add(BuildButton(B_TRANSLATE("Zoom full wave"),
			CommandBuilder(FABER_ZOOM_FULL), kZoomFullWaveIcon))
		.AddStrut(7.0f)
	.End();

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
		.Add(zoomView);

	SetExplicitSize(BSize(140, 40));

}


ZoomWidget::~ZoomWidget()
{
}
