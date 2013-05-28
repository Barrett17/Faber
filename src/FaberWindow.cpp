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

#include "AboutBox.h"
#include "Faber.h"
#include "FilterDialogs.h"
#include "Filters.h"
#include "Globals.h"
#include "MyClipBoard.h"
#include "PeakFile.h"
#include "Shortcut.h"
#include "WindowsManager.h"

#include <stdio.h>
#include <stdlib.h>


cookie_record play_cookie;


class MyMenuBar : public BMenuBar {
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
	// init prefs
	Prefs.Init();
	ClipBoard.Init();				// clipboard init
	Hist.Init();					// Undo init

	ResizeTo( Prefs.frame.Width(), Prefs.frame.Height() );
	MoveTo( Prefs.frame.left, Prefs.frame.top );

	// Now init the keyBindings
	KeyBind.Init();
	
	// create the player and recorder nodes
	Pool.InitBufferPlayer( 44100 );	

	// This is the Pool .. it loads layers and tools and floaters
	// and all kinds of groovy stuff
	Pool.Init();

	// Set the last filter
	FiltersInit();

	char s[255];
	sprintf(s, "Faber - %s", B_TRANSLATE("Untitled"));
	SetTitle(s);

	// GUI

	fToolBar = new ToolBar();
	fToolBar->SetTool(Prefs.tool_mode);

	fTracksContainer = new TracksContainer();

	fInfoToolBar = new InfoToolBar();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(_BuildMenu())
		.AddSplit(B_VERTICAL, 10.0f)
		.AddGroup(B_HORIZONTAL)
			.Add(fToolBar)
		.End()
		.Add(fTracksContainer)
		.Add(fInfoToolBar)
	.End();

	//Pool.SetLoop((fToolBar->IsLoop()));
	SetSizeLimits(MIN_W ,MAX_W, MIN_H , MAX_H);
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

	be_roster->GetRecentDocuments(&msg,10,"audio");
	while(msg.FindRef("refs",i,&eref) == B_OK){
		e.SetTo(&eref);
		if(e.InitCheck() == B_OK){
			e.GetName(name);
			msgout = new BMessage(B_REFS_RECEIVED);//DO_OPEN);
			msgout->AddRef("refs",&eref);
			fRecentMenu->AddItem(menuItem = new BMenuItem(name,msgout));
			menuItem->SetTarget(be_app);
		}
		i++;
	}
}


bool
FaberWindow::QuitRequested()
{
	if (!IsChanged(1)){
		Prefs.frame = Frame();
		Pool.StopPlaying();
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

//	message->PrintToStream();

	switch (message->what)
	{

	case TRANSPORT_PAUSE:
		play_cookie.pause = !fToolBar->IsPause();
		fToolBar->SetPause(!fToolBar->IsPause());
		break;

	case TRANSPORT_TOGGLE:
		if (Pool.IsPlaying())
			PostMessage(TRANSPORT_STOP);
		else
			PostMessage(TRANSPORT_PLAY);
		break;
	
	case TRANSPORT_SET:
		Pool.pointer = Pool.last_pointer;
		Pool.sample_view_dirty = true;	// update the sample-view
		RedrawWindow();
		break;

	case TRANSPORT_PLAY:
		if (Pool.size == 0)
			break;

		if (Pool.sample_type == NONE) {
			fToolBar->SetPlay(false);
			break;
		}

		play_cookie.pause = false;

		Pool.SetLoop(fToolBar->IsLoop());
		// play until the end
		Pool.StartPlaying(Pool.pointer*Pool.sample_type, true);
		fToolBar->SetPlay(true);
		break;
/*
	// NOTE - I have removed it as it looked unuseful.
	case TRANSPORT_PLAYS:
		if (Pool.size == 0)	break;
		if (Pool.sample_type == NONE){
			transport_view->play_sel->SetValue(B_CONTROL_OFF);
			transport_view->play->SetValue(B_CONTROL_OFF);
			break;
		}
		transport_view->stop->SetValue(B_CONTROL_OFF);
		transport_view->play_sel->SetValue(B_CONTROL_OFF);
		transport_view->play->SetValue(B_CONTROL_ON);

//		Pool.SetLoop(  (transport_view->loop->Value() == B_CONTROL_ON) );
		Pool.StartPlaying(Pool.pointer*Pool.sample_type, false);
		break;
*/

	case TRANSPORT_STOP:
		play_cookie.mem = play_cookie.start_mem;
		Pool.StopPlaying();
		FindView("Sample view")->Pulse();
		fToolBar->SetStop(true);
		break;

	case TRANSPORT_LOOP:
		fToolBar->SetLoop(!fToolBar->IsLoop());
		Pool.SetLoop(fToolBar->IsLoop());
		break;

	case UNDO:
		ClipBoard.Undo();
		break;
		
	case B_SELECT_ALL:
		if (Pool.size == 0)
			break;
		SelectAll();
		break;

	case UNSELECT_ALL:
		if (Pool.size == 0)
			break;
		DeSelectAll();
		break;

	case B_COPY:
		ClipBoard.Copy();
		break;
	
	case COPY_SILENCE:
		ClipBoard.Cut(false);		// silence cut
		break;
	
	case B_CUT:
		ClipBoard.Cut(true);		// delete cut
		break;
	
	case B_PASTE:
		ClipBoard.Paste();
		break;

	case DROP_PASTE:
	{
		/*BPoint p;
		message->FindPoint("_drop_point_", &p);
		p = fTrackView->ConvertFromScreen(p);
		BRect r = fTrackView->Bounds();
		if (r.Contains(p)){
			Pool.selection = NONE;
			Pool.pointer = (int32)(Pool.l_pointer + p.x * (Pool.r_pointer - Pool.l_pointer)/Bounds().Width());
			ClipBoard.Paste();
		}*/
	}	break;
	
	case PASTE_MIXED:
		ClipBoard.PasteMix();		// ClipBoard handles redrawing and dialogs
		break;

	case B_MOUSE_WHEEL_CHANGED:
		message->FindFloat("be:wheel_delta_y", &y);
		if (y==-1)	PostMessage(ZOOM_IN);
		if (y==1)	PostMessage(ZOOM_OUT);
		break;

	case ZOOM_IN:
		if (Pool.size == 0)	break;
		x = Pool.r_pointer - Pool.l_pointer;
		
		//if (x < fTrackView->Bounds().Width()/64)	break;
		
		x /= 2;
		if (x < 1)	x = 1;

		Pool.l_pointer = Pool.l_pointer +x/2;				// window to selection
		if (Pool.l_pointer<0)	Pool.l_pointer = 0;
		Pool.r_pointer = Pool.l_pointer + x;
		if (Pool.r_pointer > Pool.size){
			Pool.r_pointer = Pool.size;
			Pool.l_pointer = Pool.r_pointer - x;
			if (Pool.l_pointer<0)
				Pool.l_pointer = 0;
		}
		
		UpdateMenu();
		RedrawWindow();
		break;
	case ZOOM_OUT:
		if (Pool.size == 0)	break;
		x = (Pool.r_pointer - Pool.l_pointer)+1;
		x *= 2;
		if (x > Pool.size)	x = Pool.size;

		Pool.l_pointer = Pool.l_pointer -x/4;				// window to selection
		if (Pool.l_pointer<0)	Pool.l_pointer = 0;
		Pool.r_pointer = Pool.l_pointer + x;
		if (Pool.r_pointer > Pool.size){
			Pool.r_pointer = Pool.size;
			Pool.l_pointer = Pool.r_pointer - x;
			if (Pool.l_pointer<0)
				Pool.l_pointer = 0;
		}
		UpdateMenu();
		
		RedrawWindow();
		break;
	case ZOOM_FULL:
		if (Pool.size == 0)	break;
		Pool.l_pointer = 0;
		Pool.r_pointer = Pool.size;
		UpdateMenu();
		
		RedrawWindow();
		break;
	case ZOOM_SELECTION:
		if (Pool.size == 0)	break;
		if (Pool.selection != NONE){
			Pool.l_pointer = Pool.pointer;
			Pool.r_pointer = Pool.r_sel_pointer;
		}
		UpdateMenu();
		
		RedrawWindow();
		break;

	// To re enable these we have to
	// create a way to identify the
	// track currently selected.
	// That looks like a good job for TracksContainer.
	case ZOOM_LEFT:
		/*if (Pool.size == 0 || Pool.selection==NONE)	break;
		x = fTrackView->Bounds().IntegerWidth()/6;
		Pool.l_pointer = Pool.pointer -x/2;	// window to selection
		if (Pool.l_pointer<0)	Pool.l_pointer = 0;
		Pool.r_pointer = Pool.l_pointer + x;

		UpdateMenu();
		
		RedrawWindow();*/
		break;
	case ZOOM_RIGHT:
		/*if (Pool.size == 0 || Pool.selection==NONE)	break;
		x = fTrackView->Bounds().IntegerWidth()/6;
		Pool.l_pointer = Pool.r_sel_pointer - x/2;	// window to selection
		if (Pool.l_pointer<0)	Pool.l_pointer = 0;
		Pool.r_pointer = Pool.l_pointer + x;
		if (Pool.r_pointer > Pool.size){
			Pool.r_pointer = Pool.size;
			Pool.l_pointer = Pool.r_pointer - x;
		}

		UpdateMenu();
		
		RedrawWindow();*/
		break;
		
	case EDIT_L:
		if (Pool.selection != NONE)
			Pool.selection = LEFT;
		UpdateMenu();
		fTracksContainer->Invalidate();
		break;	
	case EDIT_R:
		if (Pool.selection != NONE)
			Pool.selection = RIGHT;
		UpdateMenu();
		fTracksContainer->Invalidate();
		break;	
	case EDIT_B:
		if (Pool.selection != NONE)
			Pool.selection = BOTH;
		UpdateMenu();
		fTracksContainer->Invalidate();
		break;

	case TRANSPORT_REW:
		x = Pool.r_pointer - Pool.l_pointer;
		Pool.pointer -= x/40;
		if (Pool.pointer <0)	Pool.pointer = 0;
		if (Pool.pointer < Pool.l_pointer){
			Pool.l_pointer -= x/10;
			if (Pool.l_pointer <0)	Pool.l_pointer = 0;
		}
		Pool.r_pointer = Pool.l_pointer + x;
		
		RedrawWindow();
		break;

	case TRANSPORT_REW_ALL:
		x = Pool.r_pointer - Pool.l_pointer;
		Pool.pointer = 0;
		Pool.l_pointer = 0;
		Pool.r_pointer = x;
		
		RedrawWindow();
		break;

	case TRANSPORT_FWD:
		x = Pool.r_pointer - Pool.l_pointer;
		Pool.pointer += x/40;
		if (Pool.pointer >Pool.size)	Pool.pointer = Pool.size;
		if (Pool.pointer > Pool.r_pointer){
			Pool.r_pointer += x/10;
			if (Pool.r_pointer >Pool.size)	Pool.r_pointer = Pool.size;
		}
		Pool.l_pointer = Pool.r_pointer - x;
		
		RedrawWindow();
		break;

	case TRANSPORT_FWD_ALL:
		x = Pool.r_pointer - Pool.l_pointer;
		Pool.pointer = Pool.size;;
		Pool.r_pointer = Pool.pointer;
		Pool.l_pointer = Pool.pointer - x;
		
		RedrawWindow();
		break;
	
	case TRANSPORT_HOME:
		Pool.pointer = Pool.l_pointer;
		
		RedrawWindow();
		break;
	
	case TRANSPORT_END:
		Pool.pointer = Pool.r_pointer;
		
		RedrawWindow();
		break;
	
	case TRANSPORT_LEFT:
		x = Pool.r_pointer - Pool.l_pointer;
		Pool.l_pointer -= x/2;
		if (Pool.l_pointer<0)	Pool.l_pointer = 0;
			Pool.r_pointer = Pool.l_pointer + x;
		
		RedrawWindow();
		break;

	case TRANSPORT_RIGHT:
		x = Pool.r_pointer - Pool.l_pointer;
		Pool.l_pointer += x/2;
		if (Pool.l_pointer>(Pool.size-x))
			Pool.l_pointer = Pool.size-x;
		Pool.r_pointer = Pool.l_pointer + x;
		
		RedrawWindow();
		break;

	case ABOUT:
		WindowsManager::ShowAbout();
	break;
		
	case HELP:
	{
		BPath path;
		app_info ai;
		be_app->GetAppInfo(&ai);
		BEntry entry(&ai.ref);
		entry.GetPath(&path);
		path.GetParent(&path);
		path.Append("Help/help.html");
		char *help = new char[strlen(path.Path())+1];
		sprintf(help, path.Path());
		be_roster->Launch("text/html",1, &help);
		delete help;
	}
	break;

	case HOMEPAGE:
	{
		const char* homepage = FABER_HOMEPAGE;
		be_roster->Launch("text/html",1, const_cast<char**>(&homepage));
		break;
	}
		
	case PREFERENCES:
		WindowsManager::Get()->ShowSettings();
		break;

	case UPDATE:
		fTracksContainer->Pulse();
		fInfoToolBar->Pulse();
		break;
	
	case REDRAW:
		Pool.sample_view_dirty = true;	// update the sample-view
		
		// NOTE: Enabling it cause the TrackView
		// selection to have drawing problems.
		//Pool.ResetIndexView();
		RedrawWindow();
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

	case SPECTRUM:
		WindowsManager::ShowSpectrumWindow();
		break;
		
	case SAMPLE_SCOPE:
		WindowsManager::ShowSampleScopeWindow();
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

	case CLEAR:
		if (!Pool.PrepareFilter())
			break;

		ClipBoard.DoSilence();
		Pool.changed = true;
		UpdateMenu();
		WindowsManager::Get()->HideProgress();
		ResetIndexView();
		RedrawWindow();
		break;
		
	case RUN_FILTER: // run a filter with or without GUI
	{
		if (Pool.size == 0)
			break;

		const char *tag = NULL;
		if (message->FindInt32("filter", &mod) == B_OK) {
			RunFilter(mod);
		} else if (message->FindString("language_key", &tag) == B_OK) {
			RunFilter(tag);
		}
	}
	break;

	case EXE_FILTER: // apply filter, this is send by the filter or repeat function
		if (message->FindPointer("filter", (void**)&filter) == B_OK)
			ExecuteFilter(filter);
		break;
	
	case CANCEL_FILTER:	// apply filter, this is send by the filter or repeat function
		if (message->FindPointer("filter", (void**)&filter) == B_OK)
			CancelFilter(filter);
		break;
	
	case RUN_LAST_FILTER: // run a filter with or without GUI
		RunLastFilter();
		break;
	
	case TRIM:
		DoTrim();
		break;
	
	case ZERO_IN:
		ZeroLR();
		ZeroRL();
		RedrawWindow();
		break;
	case ZERO_OUT:
		ZeroLL();
		ZeroRR();
		RedrawWindow();
		break;
	case ZERO_LL:
		ZeroLL();
		RedrawWindow();
		break;
	case ZERO_LR:
		ZeroLR();
		RedrawWindow();
		break;
	case ZERO_RL:
		ZeroRL();
		RedrawWindow();
		break;
	case ZERO_RR:
		ZeroRR();
		RedrawWindow();
		break;
	
	case SET_TIME:
		message->FindInt32("time", &x);
		Prefs.display_time = x;
		RedrawWindow();
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

	case NEW:
	case PASTE_NEW:
	case OPEN:
	case SAVE_AUDIO:
	case SAVE_AS:
	case SAVE_SELECTION:
	case B_MIME_DATA:			// let the app parse drops
	case B_SIMPLE_DATA:
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

 	fToolBar->Invalidate();
	fTracksContainer->Invalidate();
	// Not needed for now
	//fInfoToolBar->Invalidate();

	Unlock();	
}


void
FaberWindow::UpdateToolBar()
{

}


void
FaberWindow::UpdateMenu()
{
	BMenuItem *menuItem = NULL;

	Lock();

	while (menu_transform->ItemAt(0)){
		menuItem = menu_transform->ItemAt(0);
		menu_transform->RemoveItem(menuItem);
		delete menuItem;
	}
	
	if (Prefs.repeat_message)
		menu_transform->AddItem(menuItem = new BMenuItem(B_TRANSLATE(Prefs.repeat_tag.String()), new BMessage(RUN_LAST_FILTER), KeyBind.GetKey("REPEAT_ACTION"), KeyBind.GetMod("REPEAT_ACTION")));

// transform menu

	BMessage *filter_msg;
	int32 filter = 0;
	char name[255];
	// TODO rework filters to have a decent API.
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
	}

	while (menu_analyze->ItemAt(0)) {
		menuItem = menu_analyze->ItemAt(0);
		menu_analyze->RemoveItem(menuItem);
		delete menuItem;
	}
	
	menu_analyze->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Spectrum Analyzer"),
		new BMessage(SPECTRUM), KeyBind.GetKey("SPECTRUM_ANALYZER"), KeyBind.GetMod("SPECTRUM_ANALYZER")));

	menu_analyze->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Sample Scope"),
		new BMessage(SAMPLE_SCOPE), KeyBind.GetKey("SAMPLE_SCOPE"), KeyBind.GetMod("SAMPLE_SCOPE")));

	// TODO remove them, in future the CommonPool will not exist anymore.
	int32 sample_type = Pool.sample_type;
	int32 selection = Pool.selection;

	menu_transform->SetEnabled(sample_type != NONE);	// transform menu
	menu_analyze->SetEnabled(sample_type != NONE);		// analyzers menu

//	menu_generate->SetEnabled(false);					// generation menu

	menu_zero->SetEnabled(sample_type != NONE);			// zero cross menu
	mn_trim->SetEnabled(selection != NONE);				// trim
	mn_save_sel->SetEnabled(selection != NONE);			// save selection
	fSaveMenu->SetEnabled(sample_type != NONE && Pool.changed);			// save
	fSaveAsMenu->SetEnabled(sample_type != NONE);		// save as
	mn_set_freq->SetEnabled(sample_type != NONE);		// set frequency
	mn_resample->SetEnabled(sample_type != NONE);		// resample
	mn_select_all->SetEnabled(sample_type != NONE);		// select all
	mn_unselect->SetEnabled(selection != NONE);			// DeSelect all
	mn_cut->SetEnabled(selection != NONE);				// cut
	mn_copy->SetEnabled(selection != NONE);				// copy
	mn_copy_silence->SetEnabled(selection != NONE);		// copy & Silence
	mn_clear->SetEnabled(selection != NONE);			// clear

	mn_undo->SetEnabled(Hist.HasUndo());				// need history class for this
	mn_paste->SetEnabled(ClipBoard.HasClip());
	mn_paste_new->SetEnabled(ClipBoard.HasClip());

	mn_paste_mix->SetEnabled(ClipBoard.HasClip());
	mn_redo->SetEnabled(Hist.HasRedo());				// need history class for this

	UpdateToolBar();
	Unlock();
}


//Check for and handle changed files
bool FaberWindow::IsChanged(int32 mode)
{
	if (Pool.changed) {
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


BMenuBar*
FaberWindow::_BuildMenu()
{
	BMenu* menu;
	BMenuItem* menuItem;

	fMainMenuBar = new MyMenuBar("MenuBar");

	menu = new BMenu(B_TRANSLATE("File"));
	fMainMenuBar->AddItem(menu);

	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("New"), new BMessage(NEW),
		KeyBind.GetKey("FILE_NEW"), KeyBind.GetMod("FILE_NEW")));

	fRecentMenu = new BMenu(B_TRANSLATE("Open..."));
	UpdateRecent();
	menu->AddItem(fRecentMenu);
	BMenuItem *openitem = menu->FindItem(B_TRANSLATE("Open..."));
	openitem->SetShortcut(KeyBind.GetKey("FILE_OPEN"),KeyBind.GetMod("FILE_OPEN"));
	openitem->SetMessage(new BMessage(OPEN));
//	openitem->SetShortcut('O', 0);

	/*
	//TODO implement those functionalities
	menu->AddItem(new BMenuItem(B_TRANSLATE("Insert..."), new BMessage(INSERT), KeyBind.GetKey("FILE_INSERT"), KeyBind.GetMod("FILE_INSERT")));

	menu->AddItem(new BMenuItem(B_TRANSLATE("Append..."), new BMessage(APPEND), KeyBind.GetKey("FILE_APPEND"), KeyBind.GetMod("FILE_APPEND")));

	menu->AddItem(new BMenuItem(B_TRANSLATE("Mix..."), new BMessage(OPEN_MIX), KeyBind.GetKey("FILE_MIX"), KeyBind.GetMod("FILE_MIX")));
	*/
	menu->AddSeparatorItem();

	menu->AddItem(fSaveMenu = new BMenuItem(B_TRANSLATE("Save"),
		new BMessage(SAVE), KeyBind.GetKey("FILE_SAVE"), KeyBind.GetMod("FILE_SAVE")));

	menu->AddItem(fSaveAsMenu = new BMenuItem(B_TRANSLATE("Save As..."),
		new BMessage(SAVE_AS), KeyBind.GetKey("FILE_SAVE_AS"), KeyBind.GetMod("FILE_SAVE_AS")));

	menu->AddItem(mn_save_sel = new BMenuItem(B_TRANSLATE("Save Selection..."),
		new BMessage(SAVE_SELECTION), KeyBind.GetKey("FILE_SAVE_SELECTION"),
		KeyBind.GetMod("FILE_SAVE_SELECTION")));

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(B_TRANSLATE("Preferences..."),
		new BMessage(PREFERENCES), KeyBind.GetKey("PREFERENCES"), KeyBind.GetMod("PREFERENCES")));

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(B_TRANSLATE("Quit"),
		new BMessage(B_QUIT_REQUESTED), KeyBind.GetKey("FILE_QUIT"), KeyBind.GetMod("FILE_QUIT")));

	fEditMenu = new BMenu(B_TRANSLATE("Edit"));
	fMainMenuBar->AddItem(fEditMenu);

	fEditMenu->AddItem(mn_undo = new BMenuItem(B_TRANSLATE("Undo"),
		new BMessage(UNDO), KeyBind.GetKey("UNDO"), KeyBind.GetMod("UNDO")));

	fEditMenu->AddItem(mn_redo = new BMenuItem(B_TRANSLATE("Redo"),
		new BMessage(REDO), KeyBind.GetKey("REDO"), KeyBind.GetMod("REDO")));

	fEditMenu->AddSeparatorItem();

	fEditMenu->AddItem(mn_copy = new BMenuItem(B_TRANSLATE("Copy"),
		new BMessage(B_COPY), KeyBind.GetKey("COPY"), KeyBind.GetMod("COPY")));

	fEditMenu->AddItem(mn_copy_silence = new BMenuItem(B_TRANSLATE("Copy & Silence"),
		new BMessage(COPY_SILENCE), KeyBind.GetKey("COPY_SILENCE"), KeyBind.GetMod("COPY_SILENCE")));

	fEditMenu->AddItem(mn_cut = new BMenuItem(B_TRANSLATE("Cut"), new BMessage(B_CUT), KeyBind.GetKey("CUT"), KeyBind.GetMod("CUT")));

	fEditMenu->AddItem(mn_paste = new BMenuItem(B_TRANSLATE("Paste"), new BMessage(B_PASTE), KeyBind.GetKey("PASTE"), KeyBind.GetMod("PASTE")));

	fEditMenu->AddItem(mn_paste_new = new BMenuItem(B_TRANSLATE("Paste as new"), new BMessage(PASTE_NEW), KeyBind.GetKey("PASTE_NEW"), KeyBind.GetMod("PASTE_NEW")));

	fEditMenu->AddItem(mn_paste_mix = new BMenuItem(B_TRANSLATE("Paste & mix"), new BMessage(PASTE_MIXED), KeyBind.GetKey("EDIT_PASTE_MIX"), KeyBind.GetMod("EDIT_PASTE_MIX")));

	fEditMenu->AddSeparatorItem();
	fEditMenu->AddItem(mn_select_all = new BMenuItem(B_TRANSLATE("Select All"), new BMessage(B_SELECT_ALL), KeyBind.GetKey("SELECT_ALL"), KeyBind.GetMod("SELECT_ALL")));

	fEditMenu->AddItem(mn_unselect = new BMenuItem(B_TRANSLATE("Unselect All"), new BMessage(UNSELECT_ALL), KeyBind.GetKey("UNSELECT_ALL"), KeyBind.GetMod("UNSELECT_ALL")));

	fEditMenu->AddSeparatorItem();
	menu_zero = new BMenu(B_TRANSLATE("Zero Crossings"));

	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Inwards"), new BMessage(ZERO_IN), KeyBind.GetKey("ZERO_IN"), KeyBind.GetMod("ZERO_IN")));
	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Outwards"), new BMessage(ZERO_OUT), KeyBind.GetKey("ZERO_OUT"), KeyBind.GetMod("ZERO_OUT")));
	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Left to Left"), new BMessage(ZERO_LL), KeyBind.GetKey("ZERO_LL"), KeyBind.GetMod("ZERO_LL")));
	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Left to Right"), new BMessage(ZERO_LR), KeyBind.GetKey("ZERO_LR"), KeyBind.GetMod("ZERO_LR")));
	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Right to Left"), new BMessage(ZERO_RL), KeyBind.GetKey("ZERO_RL"), KeyBind.GetMod("ZERO_RL")));
	menu_zero->AddItem(new BMenuItem(B_TRANSLATE("Right to Right"), new BMessage(ZERO_RR), KeyBind.GetKey("ZERO_RR"), KeyBind.GetMod("ZERO_RR")));
	fEditMenu->AddItem(menu_zero);
	
	fEditMenu->AddSeparatorItem();
	fEditMenu->AddItem(mn_clear = new BMenuItem(B_TRANSLATE("Clear"), new BMessage(CLEAR), KeyBind.GetKey("CLEAR"), KeyBind.GetMod("CLEAR")));

	fEditMenu->AddItem(mn_trim = new BMenuItem(B_TRANSLATE("Trim"), new BMessage(TRIM), KeyBind.GetKey("TRIM"), KeyBind.GetMod("TRIM")));

	fEditMenu->AddItem(mn_set_freq = new BMenuItem(B_TRANSLATE("Change frequency..."), new BMessage(SET_FREQUENCY), KeyBind.GetKey("SET_FREQ"), KeyBind.GetMod("SET_FREQ")));

	fEditMenu->AddItem(mn_resample = new BMenuItem(B_TRANSLATE("Resample"), new BMessage(RESAMPLE), KeyBind.GetKey("RESAMPLE"), KeyBind.GetMod("RESAMPLE")));

	menu_transform = new BMenu(B_TRANSLATE("Effects"));
	fMainMenuBar->AddItem(menu_transform);

	menu_analyze = new BMenu(B_TRANSLATE("Analyze"));

	fMainMenuBar->AddItem(menu_analyze);

	menu_generate = new BMenu(B_TRANSLATE("Generate"));

	fMainMenuBar->AddItem(menu_generate);


	menu = new BMenu(B_TRANSLATE("Help"));
	fMainMenuBar->AddItem(menu);

	menu->AddItem(new BMenuItem(B_TRANSLATE("Help"),
		new BMessage(HELP), KeyBind.GetKey("HELP"), KeyBind.GetMod("HELP")));

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(B_TRANSLATE("Homepage"),
		new BMessage(HOMEPAGE), KeyBind.GetKey("HOMEPAGE"), KeyBind.GetMod("HOMEPAGE")));

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(B_TRANSLATE("About"),
		new BMessage(ABOUT), KeyBind.GetKey("ABOUT"), KeyBind.GetMod("ABOUT")));

	SetKeyMenuBar(NULL);
	return fMainMenuBar;
}


// refills the PeakFile Cache
void
FaberWindow::ResetIndexView()
{
	if (Pool.sample_type == NONE)
		return;

	WindowsManager::Get()->StartProgress(B_TRANSLATE("Indexing..."), Pool.size);

	Peak.Init(Pool.size+1, (Pool.sample_type == MONO) );
	Peak.CreatePeaks(0, Pool.size+1, Pool.size+1);
			// update the draw cache

	WindowsManager::Get()->HideProgress();
}


//   Select All
void
FaberWindow::SelectAll()
{
	if (Pool.sample_type != NONE) {
		Pool.pointer = 0;
		Pool.r_sel_pointer = Pool.size;
		Pool.selection = BOTH;
		UpdateMenu();
		RedrawWindow();
	}
}


//   DeSelect All
void
FaberWindow::DeSelectAll()
{
	if (Pool.sample_type != NONE && Pool.selection != NONE) {
		Pool.selection = NONE;
		Pool.r_sel_pointer = 0;
		UpdateMenu();
		RedrawWindow();
	}
}
