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

/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "FaberWindow.h"

#include <Cursor.h>
#include <LayoutBuilder.h>
#include <Path.h>

#include "Faber.h"
#include "FilterDialogs.h"
#include "Filters.h"
#include "Globals.h"
#include "PeakFile.h"
#include "Shortcut.h"
#include "WindowsManager.h"

#include <stdio.h>
#include <stdlib.h>


class MyMenuBar : public BMenuBar
{
public:
  					MyMenuBar(const char *name);
	virtual void	MakeFocus(bool b);
};


MyMenuBar::MyMenuBar(const char *name)
	:
	BMenuBar(name)
{
}


void
MyMenuBar::MakeFocus(bool b)
{
	// This one does make sure the MenuBar does NOT get focus !!!
	// To avoid the key-navigation to stop working
}


FaberWindow::FaberWindow(BRect frame)
	:
	BWindow(frame, "Faber" , B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS),
	fMainMenuBar(NULL)
{
	char s[255];
	sprintf(s, "Faber - %s", B_TRANSLATE("Untitled"));
	SetTitle(s);

	// init prefs
	Prefs.Init();

	//ClipBoard.Init();
	//Hist.Init();

	ResizeTo( Prefs.frame.Width(), Prefs.frame.Height() );
	MoveTo( Prefs.frame.left, Prefs.frame.top );

	// Now init the keyBindings
	KeyBind.Init();

	fOutputGate = new AudioGate();
	fOutputGate->Init();
	fOutputGate->InitNode();

	// Set the last filter
	FiltersInit();

	fToolBar = new ToolBar();
	fToolBar->SetTool(Prefs.tool_mode);

	fTracksContainer = new TracksContainer();

	fInfoToolBar = new InfoToolBar();

	BScrollView* tracksScrollView = new BScrollView("scrollviewR",
		fTracksContainer, B_WILL_DRAW | B_FOLLOW_ALL, true, true);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.Add(_BuildMenu())
		.Add(fToolBar)
		.Add(tracksScrollView)
		.Add(fInfoToolBar)
	.End();

	//fOutputGate->SetLoop((fToolBar->IsLoop()));

	SetSizeLimits(WINDOW_MIN_SIZE_X, WINDOW_MIN_SIZE_Y,
		WINDOW_MAX_SIZE_X, WINDOW_MAX_SIZE_Y);

	SetPulseRate(50000);
}


FaberWindow::~FaberWindow()
{
}


void
FaberWindow::UpdateRecent()
{
	BMenuItem *menuItem;
	BMessage msg;
	BMessage *msgout = NULL;
	entry_ref eref;
	BEntry e;
	int32 i = 0;
	char name[B_FILE_NAME_LENGTH];
	
	while(fRecentMenu->ItemAt(0))
		fRecentMenu->RemoveItem(fRecentMenu->ItemAt(0));

	be_roster->GetRecentDocuments(&msg, 100, 0, FABER_MIMETYPE);
	for(int i = 0; msg.FindRef("refs", i,&eref) == B_OK; i++) {
		e.SetTo(&eref);
		if(e.InitCheck() == B_OK) {
			e.GetName(name);
			msgout = new BMessage(B_REFS_RECEIVED);
			msgout->AddRef("refs",&eref);
			fRecentMenu->AddItem(menuItem = new BMenuItem(name,msgout));
			menuItem->SetTarget(be_app);
		}
	}
}


bool
FaberWindow::QuitRequested()
{
	if (!IsChanged(1)) {
		Prefs.frame = Frame();
		fOutputGate->Stop();
		be_app->PostMessage(B_QUIT_REQUESTED);
		return true;
	} else {
		return false;
	}
}


void
FaberWindow::MessageReceived(BMessage *message)
{
	RealtimeFilter *filter = NULL;
	int32 x, key, mod, raw_key;
	uint32 msg;
	float y;

	switch (message->what)
	{
		case ABOUT:
			WindowsManager::ShowAbout();
			break;
	
		case HOMEPAGE:
		{
			const char* homepage = FABER_HOMEPAGE;
			be_roster->Launch("text/html", 1, const_cast<char**>(&homepage));
			break;
		}
			
		case PREFERENCES:
			WindowsManager::Get()->ShowSettings();
			break;

		case NEW:
		{
			app_info info;
			be_app->GetAppInfo(&info);
			be_roster->Launch(info.signature);
			break;
		}

		case PASTE_NEW:
		{
			app_info info;
			be_app->GetAppInfo(&info);
			be_roster->Launch(info.signature, new BMessage(B_PASTE));
			break;
		}

		case OPEN:
		{
			if (!IsChanged())
				WindowsManager::GetOpenPanel()->Show();
			break;
		}

		case SAVE_AUDIO:
		{
			break;
		}

		case SAVE_AS:
		case SAVE:
		{
			if (fTracksContainer->CountTracks() == 0)
				return;

			fSaveSelection = false;

			SavePanel* panel = WindowsManager::GetSavePanel();
			panel->Window()->SetTitle(B_TRANSLATE("Save soundfile..."));
			panel->Show();

			break;
		}

		case SAVE_SELECTION:
		{
			TrackView* current = fTracksContainer->CurrentTrack();

			if (fTracksContainer->CountTracks() == 0
				|| fTracksContainer->CurrentTrack() == NULL)
				return;		

			fSaveSelection = true;

			SavePanel* panel = WindowsManager::GetSavePanel();
			panel->Window()->SetTitle(B_TRANSLATE("Save selection..."));

			panel->Show();
			break;
		}

		case TRANSPORT_PAUSE:
		{
			fOutputGate->SetPause(!fToolBar->IsPause());
			fToolBar->SetPause(!fToolBar->IsPause());
			break;
		}

		case TRANSPORT_TOGGLE:
		{
			if (fOutputGate->IsStarted())
				PostMessage(TRANSPORT_STOP);
			else
				PostMessage(TRANSPORT_PLAY);

			break;
		}

		case TRANSPORT_SET:
			/*Pool.pointer = Pool.last_pointer;
			Pool.sample_view_dirty = true;	// update the sample-view
			RedrawWindow();*/
			break;

		case TRANSPORT_PLAY:
		{
			if (fTracksContainer->CountTracks() == 0)
				break;

			fOutputGate->SetPause(false);

			fOutputGate->SetLoop(fToolBar->IsLoop());

			// play until the end
			//fOutputGate.StartFrom(Pool.pointer*Pool.sample_type);
			fOutputGate->Start();
			fToolBar->SetPlay(true);
			break;
		}

		case TRANSPORT_STOP:
		{
			fOutputGate->Stop();
			fTracksContainer->Pulse();
			fToolBar->SetStop(true);
			break;
		}

		case TRANSPORT_LOOP:
			fToolBar->SetLoop(!fToolBar->IsLoop());
			fOutputGate->SetLoop(fToolBar->IsLoop());
			break;

		case TRANSPORT_REW:
		/*	x = Pool.r_pointer - Pool.l_pointer;
			Pool.pointer -= x/40;
			if (Pool.pointer <0)
				Pool.pointer = 0;
	
			if (Pool.pointer < Pool.l_pointer) {
				Pool.l_pointer -= x/10;
				if (Pool.l_pointer <0)
					Pool.l_pointer = 0;
			}
			Pool.r_pointer = Pool.l_pointer + x;
			RedrawWindow();*/
			break;
	
		case TRANSPORT_REW_ALL:
		/*	x = Pool.r_pointer - Pool.l_pointer;
			Pool.pointer = 0;
			Pool.l_pointer = 0;
			Pool.r_pointer = x;
			
			RedrawWindow();*/
			break;
	
		case TRANSPORT_FWD:
		/*	x = Pool.r_pointer - Pool.l_pointer;
			Pool.pointer += x/40;
			if (Pool.pointer >Pool.size)
				Pool.pointer = Pool.size;
	
			if (Pool.pointer > Pool.r_pointer) {
				Pool.r_pointer += x/10;
				if (Pool.r_pointer >Pool.size)
					Pool.r_pointer = Pool.size;
			}
			Pool.l_pointer = Pool.r_pointer - x;
			RedrawWindow();*/
			break;
	
		case TRANSPORT_FWD_ALL:
		/*	x = Pool.r_pointer - Pool.l_pointer;
			Pool.pointer = Pool.size;
			Pool.r_pointer = Pool.pointer;
			Pool.l_pointer = Pool.pointer - x;
			
			RedrawWindow();*/
			break;
		
		case TRANSPORT_HOME:
		/*	Pool.pointer = Pool.l_pointer;
			
			RedrawWindow();*/
			break;
		
		case TRANSPORT_END:
			/*Pool.pointer = Pool.r_pointer;
			
			RedrawWindow();*/
			break;
		
		case TRANSPORT_LEFT:
			/*x = Pool.r_pointer - Pool.l_pointer;
			Pool.l_pointer -= x/2;
			if (Pool.l_pointer<0)
				Pool.l_pointer = 0;
	
			Pool.r_pointer = Pool.l_pointer + x;
			
			RedrawWindow();*/
			break;
	
		case TRANSPORT_RIGHT:
		/*	x = Pool.r_pointer - Pool.l_pointer;
			Pool.l_pointer += x/2;
	
			if (Pool.l_pointer>(Pool.size-x))
				Pool.l_pointer = Pool.size-x;
	
			Pool.r_pointer = Pool.l_pointer + x;
			
			RedrawWindow();*/
			break;
	
		case B_SELECT_ALL:
		case UNSELECT_ALL:

		case ZOOM_IN:
		case ZOOM_OUT:
		case ZOOM_FULL:
		case ZOOM_SELECTION:
		case ZOOM_LEFT:
		case ZOOM_RIGHT:

		case EDIT_L:
		case EDIT_R:
		case EDIT_B:

		case B_COPY:
		case COPY_SILENCE:
		case B_CUT:		
		case B_PASTE:
		case DROP_PASTE:

		case UNDO:
			fTracksContainer->Looper()->PostMessage(msg);
			break;

		case B_MOUSE_WHEEL_CHANGED:
		{
			message->FindFloat("be:wheel_delta_y", &y);
			if (y==-1)
				PostMessage(ZOOM_IN);
	
			if (y==1)
				PostMessage(ZOOM_OUT);

			break;
		}

		case UPDATE:
			fTracksContainer->Pulse();
			fInfoToolBar->Pulse();
			break;

		case TOOL_SELECT:
			fToolBar->SetTool(SELECT_TOOL);
			Prefs.tool_mode = SELECT_TOOL;
			UpdateMenu();
			break;

		case TOOL_DRAW:
			fToolBar->SetTool(DRAW_TOOL);
			Prefs.tool_mode = DRAW_TOOL;
			UpdateMenu();
			break;

		case TOOL_PLAY:
			fToolBar->SetTool(PLAY_TOOL);
			Prefs.tool_mode = PLAY_TOOL;
			UpdateMenu();
			break;

		case SET_FREQUENCY:
			WindowsManager::ShowFrequencyWindow();
			break;
	
		case RESAMPLE:
			WindowsManager::ShowResampleWindow();
			break;
	
		case RESAMPLE_DO:
			DoResample();
			break;

		case SET_TIME:
			message->FindInt32("time", &x);
			Prefs.display_time = x;
			//RedrawWindow();
			break;
	
		case B_KEY_DOWN:
			message->FindInt32("key", &raw_key);
			message->FindInt32("modifiers", &mod);
			message->FindInt32("raw_char", &key);
	
			// now do some conversions for combinations
			if (key == B_FUNCTION_KEY)
				key = 12+raw_key;
			else if (key>='a' && key<='z')
				key -= ('a'-'A');
			
			msg = KeyBind.GetMessage(key, mod);
			if (msg){
				BMessage new_message(msg);
				new_message.AddString("language_key", KeyBind.GetID(key, mod));
				PostMessage(&new_message);
			}
			break;

		case B_SIMPLE_DATA:
		case B_MIME_DATA:
			be_app->PostMessage(message);
			break;

		default:
			BWindow::MessageReceived(message);
	}

	if (CurrentFocus())
		CurrentFocus()->MakeFocus(false);
}


void
FaberWindow::RedrawWindow()
{
	Lock();

	fTracksContainer->Invalidate();

	Unlock();	
}


void
FaberWindow::UpdateMenu()
{
	BMenuItem *menuItem = NULL;

	Lock();
	
	if (Prefs.repeat_message) {
		menu_transform->AddItem(
			menuItem = new BMenuItem(B_TRANSLATE(Prefs.repeat_tag.String()),
				new BMessage(RUN_LAST_FILTER), KeyBind.GetKey("REPEAT_ACTION"),
				KeyBind.GetMod("REPEAT_ACTION")));
	}

	BMessage *filter_msg;
	int32 filter = 0;
	char name[255];

/*
	// TODO rework filters to have a decent API.
	while (menu_transform->ItemAt(0)) {
		menuItem = menu_transform->ItemAt(0);
		menu_transform->RemoveItem(menuItem);
		delete menuItem;
	}


	while(__FilterList[filter].name != NULL) {
		if (strcmp(__FilterList[filter].name, "---") == 0) {
			menu_transform->AddSeparatorItem();
		} else {
			// can do some stuff to organise menu here
		 	filter_msg = new BMessage(RUN_FILTER);
			filter_msg->AddInt32("filter", filter);

			sprintf(name, B_TRANSLATE(__FilterList[filter].name));
			if ( __FilterList[filter].type & FILTER_GUI )
				strcat(name, "...");

			menu_transform->AddItem(menuItem = new BMenuItem(name, filter_msg,
				KeyBind.GetKey(__FilterList[filter].name), KeyBind.GetMod(__FilterList[filter].name)));
			menuItem->SetEnabled( __FilterList[filter].type & Pool.sample_type );
		}
		filter++;
	}*/

	bool enable = fTracksContainer->CountTracks() > 0;
	bool selection = fTracksContainer->IsSelected();

	menu_transform->SetEnabled(enable);

	menu_generate->SetEnabled(false);

	menu_zero->SetEnabled(enable);

	mn_trim->SetEnabled(selection);
	mn_save_sel->SetEnabled(selection);

	fEditMenu->SetEnabled(enable);

	fSaveMenu->SetEnabled(enable/* && fProjectManager->HasChanged()*/);
	menu_generate->SetEnabled(enable);

	fSaveAsMenu->SetEnabled(enable);
	mn_set_freq->SetEnabled(enable);
	mn_resample->SetEnabled(enable);
	mn_select_all->SetEnabled(enable);
	mn_unselect->SetEnabled(selection);
	mn_cut->SetEnabled(selection);
	mn_copy->SetEnabled(selection);
	mn_copy_silence->SetEnabled(selection);
	mn_clear->SetEnabled(selection);

/*	mn_undo->SetEnabled(Hist.HasUndo());
	mn_paste->SetEnabled(ClipBoard.HasClip());
	mn_paste_new->SetEnabled(ClipBoard.HasClip());
	mn_paste_mix->SetEnabled(ClipBoard.HasClip());
	mn_redo->SetEnabled(Hist.HasRedo());
*/

	Unlock();
}


//Check for and handle changed files
bool FaberWindow::IsChanged(int32 mode)
{
	if (fTracksContainer->HasChanged()) {
		int32 k = (new BAlert(NULL,B_TRANSLATE("This project has changed. Do you want to save it now?"),
			B_TRANSLATE("Save"),B_TRANSLATE("Discard"),B_TRANSLATE("Cancel")))->Go();

		switch(k) {

			case 0:
			{
				BMessage* msg = new BMessage(SAVE_AS);
				msg->AddBool("SaveMode", mode);
				be_app->PostMessage(msg);
				return true;
				break;
			}
	
			case 1:
				return false;
			break;

			default:
				return true;
		}
	}

	return false;
}


TracksContainer*
FaberWindow::Container() const
{
	return fTracksContainer;
}


BMenuBar*
FaberWindow::_BuildMenu()
{
	BMenu* menu;
	BMenuItem* menuItem;

	fMainMenuBar = new MyMenuBar("MenuBar");

	menu = new BMenu(B_TRANSLATE("File"));
	fMainMenuBar->AddItem(menu);

	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("New Project"),
		new BMessage(NEW), KeyBind.GetKey("FILE_NEW"),
		KeyBind.GetMod("FILE_NEW")));

	menu->AddItem(fSaveMenu = new BMenuItem(B_TRANSLATE("Save Project"),
		new BMessage(SAVE), KeyBind.GetKey("FILE_SAVE"),
		KeyBind.GetMod("FILE_SAVE")));

	fRecentMenu = new BMenu(B_TRANSLATE("Recent Files..."));

	UpdateRecent();

	menu->AddItem(fRecentMenu);

	BMenuItem *openitem = menu->FindItem(B_TRANSLATE("Recent Files..."));
	openitem->SetShortcut(KeyBind.GetKey("FILE_OPEN"),KeyBind.GetMod("FILE_OPEN"));
	openitem->SetMessage(new BMessage(OPEN));

	menu->AddSeparatorItem();

	menu->AddItem(fSaveAsMenu = new BMenuItem(B_TRANSLATE("Export..."),
		new BMessage(SAVE_AS), KeyBind.GetKey("FILE_SAVE_AS"),
		KeyBind.GetMod("FILE_SAVE_AS")));

	menu->AddItem(mn_save_sel = new BMenuItem(B_TRANSLATE("Export Selection..."),
		new BMessage(SAVE_SELECTION), KeyBind.GetKey("FILE_SAVE_SELECTION"),
		KeyBind.GetMod("FILE_SAVE_SELECTION")));

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(B_TRANSLATE("Preferences..."),
		new BMessage(PREFERENCES), KeyBind.GetKey("PREFERENCES"),
		KeyBind.GetMod("PREFERENCES")));

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(B_TRANSLATE("Quit"),
		new BMessage(B_QUIT_REQUESTED), KeyBind.GetKey("FILE_QUIT"),
		KeyBind.GetMod("FILE_QUIT")));

	fEditMenu = new BMenu(B_TRANSLATE("Edit"));
	fMainMenuBar->AddItem(fEditMenu);

	fEditMenu->AddItem(mn_undo = new BMenuItem(B_TRANSLATE("Undo"),
		new BMessage(UNDO), KeyBind.GetKey("UNDO"), KeyBind.GetMod("UNDO")));

	fEditMenu->AddItem(mn_redo = new BMenuItem(B_TRANSLATE("Redo"),
		new BMessage(REDO), KeyBind.GetKey("REDO"), KeyBind.GetMod("REDO")));

	fEditMenu->AddSeparatorItem();

	fEditMenu->AddItem(mn_copy = new BMenuItem(B_TRANSLATE("Copy"),
		new BMessage(B_COPY), KeyBind.GetKey("COPY"), KeyBind.GetMod("COPY")));

	fEditMenu->AddItem(
		mn_copy_silence = new BMenuItem(B_TRANSLATE("Copy & Silence"),
		new BMessage(COPY_SILENCE), KeyBind.GetKey("COPY_SILENCE"),
		KeyBind.GetMod("COPY_SILENCE")));

	fEditMenu->AddItem(mn_cut = new BMenuItem(B_TRANSLATE("Cut"),
		new BMessage(B_CUT), KeyBind.GetKey("CUT"),
		KeyBind.GetMod("CUT")));

	fEditMenu->AddItem(mn_paste = new BMenuItem(B_TRANSLATE("Paste"),
		new BMessage(B_PASTE), KeyBind.GetKey("PASTE"),
		KeyBind.GetMod("PASTE")));

	fEditMenu->AddItem(mn_paste_new = new BMenuItem(B_TRANSLATE("Paste as new"),
		new BMessage(PASTE_NEW), KeyBind.GetKey("PASTE_NEW"),
		KeyBind.GetMod("PASTE_NEW")));

	fEditMenu->AddSeparatorItem();
	fEditMenu->AddItem(mn_select_all = new BMenuItem(B_TRANSLATE("Select All"),
		new BMessage(B_SELECT_ALL), KeyBind.GetKey("SELECT_ALL"),
		KeyBind.GetMod("SELECT_ALL")));

	fEditMenu->AddItem(mn_unselect = new BMenuItem(B_TRANSLATE("Unselect All"),
		new BMessage(UNSELECT_ALL), KeyBind.GetKey("UNSELECT_ALL"),
		KeyBind.GetMod("UNSELECT_ALL")));

	fEditMenu->AddSeparatorItem();

	fEditMenu->AddItem(mn_clear = new BMenuItem(B_TRANSLATE("Clear"),
		new BMessage(CLEAR), KeyBind.GetKey("CLEAR"),
		KeyBind.GetMod("CLEAR")));

	fEditMenu->AddItem(mn_trim = new BMenuItem(B_TRANSLATE("Trim"),
		new BMessage(TRIM), KeyBind.GetKey("TRIM"),
		KeyBind.GetMod("TRIM")));

	fEditMenu->AddItem(mn_set_freq = new BMenuItem(B_TRANSLATE("Change frequency..."),
		new BMessage(SET_FREQUENCY), KeyBind.GetKey("SET_FREQ"),
		KeyBind.GetMod("SET_FREQ")));

	fEditMenu->AddItem(mn_resample = new BMenuItem(B_TRANSLATE("Resample project"),
		new BMessage(RESAMPLE), KeyBind.GetKey("RESAMPLE"), KeyBind.GetMod("RESAMPLE")));

	fTracksMenu = new BMenu(B_TRANSLATE("Tracks"));

	fMainMenuBar->AddItem(fTracksMenu);

	fTracksMenu->AddItem(new BMenuItem(B_TRANSLATE("New Track"),
		new BMessage(), KeyBind.GetKey(""), KeyBind.GetMod("")));

	menu_transform = new BMenu(B_TRANSLATE("Effects"));

	fMainMenuBar->AddItem(menu_transform);

	menu_zero = new BMenu(B_TRANSLATE("Zero Crossings"));

	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Inwards"), new BMessage(ZERO_IN),
		KeyBind.GetKey("ZERO_IN"), KeyBind.GetMod("ZERO_IN")));

	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Outwards"), new BMessage(ZERO_OUT),
		KeyBind.GetKey("ZERO_OUT"), KeyBind.GetMod("ZERO_OUT")));
	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Left to Left"), new BMessage(ZERO_LL),
		KeyBind.GetKey("ZERO_LL"), KeyBind.GetMod("ZERO_LL")));
	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Left to Right"), new BMessage(ZERO_LR),
		KeyBind.GetKey("ZERO_LR"), KeyBind.GetMod("ZERO_LR")));
	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Right to Left"), new BMessage(ZERO_RL),
		KeyBind.GetKey("ZERO_RL"), KeyBind.GetMod("ZERO_RL")));
	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Right to Right"), new BMessage(ZERO_RR),
		KeyBind.GetKey("ZERO_RR"), KeyBind.GetMod("ZERO_RR")));

	menu_transform->AddItem(menu_zero);

	menu_generate = new BMenu(B_TRANSLATE("Generate"));

	fMainMenuBar->AddItem(menu_generate);

	menu = new BMenu(B_TRANSLATE("Help"));
	fMainMenuBar->AddItem(menu);

	menu->AddItem(new BMenuItem(B_TRANSLATE("Homepage"),
		new BMessage(HOMEPAGE), KeyBind.GetKey("HOMEPAGE"), KeyBind.GetMod("HOMEPAGE")));

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(B_TRANSLATE("About"),
		new BMessage(ABOUT), KeyBind.GetKey("ABOUT"), KeyBind.GetMod("ABOUT")));

	SetKeyMenuBar(NULL);
	return fMainMenuBar;
}
