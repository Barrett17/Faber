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

#include <LayoutBuilder.h>

#include "Faber.h"
#include "FaberDefs.h"
#include "Globals.h"
#include "Shortcut.h"
#include "WindowsManager.h"

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

	ResizeTo(Prefs.frame.Width(), Prefs.frame.Height());
	MoveTo(Prefs.frame.left, Prefs.frame.top);

	// Init the keyBindings
	KeyBind.Init();

	fOutputGate = new AudioGate();
	fOutputGate->Init();
	fOutputGate->InitNode();

	fFaberView = new FaberView();

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.Add(_BuildMenu())
		.Add(fFaberView)
	.End();

	//fOutputGate->SetLoop((fToolBar->IsLoop()));

	SetSizeLimits(WINDOW_MIN_SIZE_X, WINDOW_MIN_SIZE_Y,
		WINDOW_MAX_SIZE_X, WINDOW_MAX_SIZE_Y);

	SetPulseRate(50000);

	UpdateMenu();
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

	switch (message->what)
	{
		case FABER_ABOUT:
			WindowsManager::ShowAbout();
			break;
	
		case FABER_OPEN_HOMEPAGE:
		{
			const char* homepage = FABER_HOMEPAGE;
			be_roster->Launch("text/html", 1, const_cast<char**>(&homepage));
			break;
		}
			
		case FABER_PREFERENCES:
			WindowsManager::Get()->ShowSettings();
			break;

		case FABER_NEW_PROJECT:
		{
			app_info info;
			be_app->GetAppInfo(&info);
			be_roster->Launch(info.signature);
			break;
		}

		case FABER_PASTE_NEW:
		{
			app_info info;
			be_app->GetAppInfo(&info);
			be_roster->Launch(info.signature, new BMessage(B_PASTE));
			break;
		}

		case FABER_FILE_OPEN:
		{
			if (!IsChanged())
				WindowsManager::GetOpenPanel()->Show();
			break;
		}

		case FABER_SAVE_PROJECT_AUDIO:
		{
			break;
		}

		case FABER_EXPORT_PROJECT:
		case FABER_SAVE_PROJECT:
		{
			if (fFaberView->IsEmpty())
				return;

			fSaveSelection = false;

			SavePanel* panel = WindowsManager::GetSavePanel();
			panel->Window()->SetTitle(B_TRANSLATE("Save project..."));
			panel->Show();

			break;
		}

		case FABER_SAVE_SELECTION:
		{
			TrackView* current = fFaberView->Container()->CurrentTrack();

			if (fFaberView->IsEmpty() || current == NULL)
				return;		

			fSaveSelection = true;

			SavePanel* panel = WindowsManager::GetSavePanel();
			panel->Window()->SetTitle(B_TRANSLATE("Save selection..."));

			panel->Show();
			break;
		}
#if 0
		case TRANSPORT_PAUSE:
		{
			fOutputGate->SetPause(!fToolBar->IsPause());
			fToolBar->SetPause(!fToolBar->IsPause());
			break;
		}

		case TRANSPORT_PLAY:
		{
			if (fFaberView->IsEmpty())
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

#endif
		case B_SELECT_ALL:
		case UNSELECT_ALL:

		case B_COPY:
		case COPY_SILENCE:
		case B_CUT:		
		case B_PASTE:
		case FABER_DROP_PASTE:

		case UNDO:
			fFaberView->Looper()->PostMessage(message);
			break;

		case SET_FREQUENCY:
			WindowsManager::ShowFrequencyWindow();
			break;
	
		case RESAMPLE:
			WindowsManager::ShowResampleWindow();
			break;
	
		/*case RESAMPLE_DO:
			DoResample();
			break;*/
	
		case B_KEY_DOWN:
			int32 key, mod, raw_key, msg;

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

	fFaberView->Invalidate();

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

	bool enable = !fFaberView->IsEmpty();
	bool selection = fFaberView->IsSelected();

	menu_transform->SetEnabled(enable);

	menu_generate->SetEnabled(false);

	menu_zero->SetEnabled(enable);

	mn_trim->SetEnabled(selection);
	mn_save_sel->SetEnabled(selection);

	fEditMenu->SetEnabled(enable);

	fSaveMenu->SetEnabled(enable/* && fProjectManager->HasChanged()*/);
	menu_generate->SetEnabled(enable);

	fSaveAsMenu->SetEnabled(enable);
	mn_save_sel->SetEnabled(enable);

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

	// Set targets for menus

	fTracksMenu->SetTargetForItems(fFaberView->Container());

	Unlock();
}


//Check for and handle changed files
bool FaberWindow::IsChanged(int32 mode)
{
	if (fFaberView->HasChanged()) {
		int32 k = (new BAlert(NULL,B_TRANSLATE("This project has changed. Do you want to save it now?"),
			B_TRANSLATE("Save"),B_TRANSLATE("Discard"),B_TRANSLATE("Cancel")))->Go();

		switch(k) {

			case 0:
			{
				BMessage* msg = new BMessage(FABER_EXPORT_PROJECT);
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


FaberView*
FaberWindow::MainView() const
{
	return fFaberView;
}


BMenuBar*
FaberWindow::_BuildMenu()
{
	BMenu* menu;
	BMenuItem* menuItem;

	fMainMenuBar = new BMenuBar("MenuBar");

	menu = new BMenu(B_TRANSLATE("File"));
	fMainMenuBar->AddItem(menu);

	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("New Empty Project"),
		new BMessage(FABER_NEW_PROJECT), KeyBind.GetKey("FABER_NEW_PROJECT"),
		KeyBind.GetMod("FABER_NEW_PROJECT")));

	menu->AddItem(fSaveMenu = new BMenuItem(B_TRANSLATE("Save Project"),
		new BMessage(FABER_SAVE_PROJECT), KeyBind.GetKey("FABER_SAVE_PROJECT"),
		KeyBind.GetMod("FABER_SAVE_PROJECT")));

	fRecentMenu = new BMenu(B_TRANSLATE("Open Files..."));
	UpdateRecent();
	menu->AddItem(fRecentMenu);

	BMenuItem *openitem = menu->FindItem(B_TRANSLATE("Open Files..."));

	openitem->SetShortcut(KeyBind.GetKey("FABER_FILE_OPEN"),
		KeyBind.GetMod("FABER_FILE_OPEN"));

	openitem->SetMessage(new BMessage(FABER_FILE_OPEN));

	menu->AddSeparatorItem();

	menu->AddItem(fSaveAsMenu = new BMenuItem(B_TRANSLATE("Export..."),
		new BMessage(FABER_EXPORT_PROJECT), KeyBind.GetKey("FABER_EXPORT_PROJECT"),
		KeyBind.GetMod("FABER_EXPORT_PROJECT")));

	menu->AddItem(mn_save_sel = new BMenuItem(B_TRANSLATE("Export Selection..."),
		new BMessage(FABER_SAVE_SELECTION), KeyBind.GetKey("FABER_SAVE_SELECTION"),
		KeyBind.GetMod("FABER_SAVE_SELECTION")));

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(B_TRANSLATE("Preferences..."),
		new BMessage(FABER_PREFERENCES), KeyBind.GetKey("FABER_PREFERENCES"),
		KeyBind.GetMod("FABER_PREFERENCES")));

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
		new BMessage(FABER_PASTE_NEW), KeyBind.GetKey("FABER_PASTE_NEW"),
		KeyBind.GetMod("FABER_PASTE_NEW")));

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
		new BMessage(FABER_NEW_EMPTY_TRACK), KeyBind.GetKey(""), KeyBind.GetMod("")));

	menu_transform = new BMenu(B_TRANSLATE("Effects"));

	fMainMenuBar->AddItem(menu_transform);

	menu_zero = new BMenu(B_TRANSLATE("Zero Crossings"));

	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Inwards"), new BMessage(FABER_ZERO_IN),
		KeyBind.GetKey("FABER_ZERO_IN"), KeyBind.GetMod("FABER_ZERO_IN")));

	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Outwards"), new BMessage(FABER_ZERO_OUT),
		KeyBind.GetKey("FABER_ZERO_OUT"), KeyBind.GetMod("FABER_ZERO_OUT")));
	
	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Left to Left"), new BMessage(FABER_ZERO_LL),
		KeyBind.GetKey("FABER_ZERO_LL"), KeyBind.GetMod("FABER_ZERO_LL")));
	
	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Left to Right"), new BMessage(FABER_ZERO_LR),
		KeyBind.GetKey("FABER_ZERO_LR"), KeyBind.GetMod("FABER_ZERO_LR")));
	
	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Right to Left"), new BMessage(FABER_ZERO_RL),
		KeyBind.GetKey("FABER_ZERO_RL"), KeyBind.GetMod("FABER_ZERO_RL")));
	
	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Right to Right"), new BMessage(FABER_ZERO_RR),
		KeyBind.GetKey("FABER_ZERO_RR"), KeyBind.GetMod("FABER_ZERO_RR")));

	menu_transform->AddItem(menu_zero);

	menu_generate = new BMenu(B_TRANSLATE("Generate"));

	fMainMenuBar->AddItem(menu_generate);

	menu = new BMenu(B_TRANSLATE("Help"));
	fMainMenuBar->AddItem(menu);

	menu->AddItem(new BMenuItem(B_TRANSLATE("Homepage"),
		new BMessage(FABER_OPEN_HOMEPAGE),
			KeyBind.GetKey("FABER_OPEN_HOMEPAGE"),
			KeyBind.GetMod("FABER_OPEN_HOMEPAGE")));

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(B_TRANSLATE("About"),
		new BMessage(FABER_ABOUT), KeyBind.GetKey("FABER_ABOUT"), KeyBind.GetMod("FABER_ABOUT")));

	SetKeyMenuBar(NULL);
	return fMainMenuBar;
}
