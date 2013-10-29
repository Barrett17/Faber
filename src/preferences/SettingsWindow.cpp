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

#include "SettingsWindow.h"

#include <Alert.h>
#include <Application.h>
#include <GroupLayout.h>
#include <Window.h>
#include <View.h>

#include "FaberDefs.h"
#include "SettingsView.h"
#include "Shortcut.h"
#include "WindowsManager.h"

#define COLOR_SELECT		'colS'


SettingsWindow::SettingsWindow() 
	:
	BWindow(BRect(100,100,600,550),
		B_TRANSLATE("Preferences"), B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{	
	SetLayout(new BGroupLayout(B_VERTICAL));

	fSettingsView = new SettingsView();
	GetLayout()->AddView(fSettingsView);
	Run();
}


void SettingsWindow::MessageReceived(BMessage* message)
{
	BView *tmpV;
	int32 k;

	switch (message->what){
	case QUIT:
		Hide();
		break;

	case SET_FACTORY:
		k = (new BAlert(NULL,B_TRANSLATE("Restore the factory settings overwrites all user settings!"),B_TRANSLATE("Apply"),B_TRANSLATE("Cancel")))->Go();
		if (k==0){
			Prefs.FactorySettings();
			KeyBind.InstallDefaults();
			tmpV = FindView("Prefs color");
			if (tmpV != NULL){
				PostMessage(COLOR_SELECT, tmpV);
			}
			tmpV = FindView("Prefs keys");
			if (tmpV != NULL){
				tmpV->LockLooper();
				tmpV->Pulse();
				tmpV->UnlockLooper();
			}

			/*Pool.sample_view_dirty = true;	// update the sample-view
			Pool.update_draw_cache = true;	// update the draw cache*/
					// update the index cache
			//WindowsManager::MainWindow()->RedrawWindow();
		}
		break;

	default:
		BWindow::MessageReceived(message);
	}
}


bool SettingsWindow::QuitRequested()
{
   Hide();
   return false;
}
