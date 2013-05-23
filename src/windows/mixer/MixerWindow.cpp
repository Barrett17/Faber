/*
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#include <Application.h>
#include <GroupLayout.h>
#include <Window.h>
#include <View.h>
#include "MixerWindow.h"
#include "MixerView.h"



MixerWindow::MixerWindow() 
		:
		BWindow(BRect(150, 100, 540, 480), "Mixer", B_TITLED_WINDOW, B_NOT_ZOOMABLE)
{	
	SetLayout(new BGroupLayout(B_VERTICAL));

	fMixerView = new MixerView();
	GetLayout()->AddView(fMixerView);
}


void MixerWindow::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		default:
			BWindow::MessageReceived(message);
	}

}



bool MixerWindow::QuitRequested()
{
	return true;
}

