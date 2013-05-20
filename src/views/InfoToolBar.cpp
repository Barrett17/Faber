/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include "InfoToolBar.h"

#include <LayoutBuilder.h>
#include <TextView.h>

InfoToolBar::InfoToolBar()
	:
	BView("InfoToolBar", B_FOLLOW_ALL_SIDES | B_FOLLOW_BOTTOM
		| B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_PULSE_NEEDED)
{
	fPointerTextView = new BTextView("view", B_WILL_DRAW);

	fPointerTextView->SetExplicitMaxSize(BSize(1500, 20));
	fPointerTextView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BLayoutBuilder::Group<>(this, B_HORIZONTAL)
	.Add(fPointerTextView)
	.End();
}


InfoToolBar::~InfoToolBar()
{
}


void
InfoToolBar::Pulse()
{
	int64 p = 0;
	if (Pool.sample_type){
		if (Pool.IsPlaying()){
			p = Pool.last_pointer;
		}else
			p = Pool.pointer;
	}

	if (p != pointer){
		pointer = p;
		fPointerTextView->SetText(BString() << p);
	}
}
