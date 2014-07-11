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

#include "WidgetFrame.h"

#include <GradientRadialFocus.h>
#include <LayoutBuilder.h>


WidgetFrame::WidgetFrame(BView* view)
	:
	BView("WidgetFrame", B_WILL_DRAW)
{
	if (view != NULL) {
		BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
			.AddStrut(0.0f)
			.Add(view)
		.End();
	}
	rgb_color backgroundColor = {120,120,120};
	SetViewColor(backgroundColor);
}


WidgetFrame::~WidgetFrame()
{
}


void
WidgetFrame::Draw(BRect rect)
{
	rect = Bounds();

	rgb_color background = ui_color(B_PANEL_BACKGROUND_COLOR);
	rgb_color shadowColor = {120,120,120};
	float lightTint = 1.6;

	rgb_color high = tint_color(background, lightTint);
	rgb_color black = {120,120,120};

	float x = 15;
	float y = 15;

	rect.left++;
	rect.top++;

	SetDrawingMode(B_OP_OVER);

	SetHighColor(shadowColor);

	SetDrawingMode(B_OP_ALPHA);
	BGradientRadialFocus frameGradient;
	frameGradient.AddColor(black, 0);
	frameGradient.AddColor(high, 255);
	frameGradient.AddColor(high, 0);

	FillRoundRect(rect, x, y, frameGradient);
}
