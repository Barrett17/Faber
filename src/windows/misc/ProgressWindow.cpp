/*
	Copyright (c) 2003, Xentronix
	Author: Frans van Nispen (frans@xentronix.com)
	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:

	Redistributions of source code must retain the above copyright notice, this list
	of conditions and the following disclaimer. Redistributions in binary form must
	reproduce the above copyright notice, this list of conditions and the following
	disclaimer in the documentation and/or other materials provided with the distribution.

	Neither the name of Xentronix nor the names of its contributors may be used
	to endorse or promote products derived from this software without specific prior
	written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
	SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ProgressWindow.h"

#include <View.h>

#include "FaberDefs.h"


ProgressWindow::ProgressWindow(BRect frame)
	:
	BWindow(frame, "Progress",
		B_MODAL_WINDOW_LOOK,
		B_MODAL_APP_WINDOW_FEEL, 
		B_NOT_RESIZABLE	| B_NOT_CLOSABLE
		| B_NOT_ZOOMABLE | B_NOT_MOVABLE)
{
	Looper()->SetName("Progress Window");

	BView *view = new BView(frame, NULL,
		B_FOLLOW_ALL, B_WILL_DRAW | B_NOT_CLOSABLE);

	frame.InsetBy(2,2);

	fBar = new BStatusBar(frame, NULL, NULL, NULL);

	rgb_color barColor = { 120, 120, 120 };
	fBar->SetBarColor(barColor);

	view->AddChild(fBar);
	view->SetViewColor(216,216,216);

	AddChild(view);

	Run();
}


ProgressWindow::~ProgressWindow()
{
}


void
ProgressWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {
		default:
			BWindow::MessageReceived(message);
	}
}


void
ProgressWindow::StartProgress(const char *label, int32 max)
{
	if (Lock()) {
		fBar->Reset();
		fBar->SetText(B_TRANSLATE(label));

		fBar->SetMaxValue(max);

		ResizeTo(300,40);
		MoveTo( Prefs.frame.Width()/2 -150 + Prefs.frame.left,
				Prefs.frame.Height()/2 -15 + Prefs.frame.top);

		if (IsHidden())
			Show();
		Unlock();
	}
}


void
ProgressWindow::SetProgress(int32 p)
{
	if (Lock()) {
		fBar->Update(p);
		Unlock();
	}
}


void
ProgressWindow::SetProgressName(const char *name)
{
	if (Lock()) {
		fBar->SetText(name);
		Unlock();
	}
}


void
ProgressWindow::HideProgress()
{
	if (Lock()) {
		if (!IsHidden())
			Hide();
		Unlock();
	}
}
