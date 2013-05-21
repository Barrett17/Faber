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
#include "FaberWindow.h"

#include <Application.h>
#include <Cursor.h>
#include <InterfaceKit.h>
#include <LayoutBuilder.h>
#include <MediaKit.h>
#include <StorageKit.h>

#include "AboutBox.h"
#include "main.h"
#include "Globals.h"
#include "Filters.h"


#include "FreqWindow.h"
#include "MyClipBoard.h"
#include "Shortcut.h"
#include "FilterDialogs.h"
#include "Analyzers.h"

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
	BWindow(frame, "Faber" ,B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS),
	fMainMenuBar(NULL)
{
	// create global access
	Pool.mainWindow = this;			// handle to this window

	// set MIME types
	//Pool.InstallMimeType();

	// init prefs
	Prefs.Init();
	ClipBoard.Init();				// clipboard init
	Hist.Init();					// Undo init

	Pool.mouseArrow = new BCursor(IMouse_Arrow);
	Pool.mouseArrowLeft = new BCursor(IMouse_ArrowLeft);
	Pool.mouseArrowRight = new BCursor(IMouse_ArrowRight);
	Pool.mousePencil = new BCursor(IMouse_Pencil);
	Pool.mouseMove = new BCursor(IMouse_Move);
	Pool.mouseArrowMove = new BCursor(IMouse_MoveArrow);
	Pool.mouseLeftRight = new BCursor(IMouse_LeftRight);

	ResizeTo( Prefs.frame.Width(), Prefs.frame.Height() );
	MoveTo( Prefs.frame.left, Prefs.frame.top );

	// Now init the keyBindings
	KeyBind.Init();
	
	Pool.progress = new ProgressWindow(BRect(0,0,300,30));
	Pool.PrefWin = new SettingsWindow();
	
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
	fTrackView = new TrackAudio();
	Pool.m_SampleView = fTrackView;	// for the player
	fInfoToolBar = new InfoToolBar();
	fTimeBar = new TimeBarView();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(_BuildMenu())
		.AddSplit(B_VERTICAL, 10.0f)
		.AddGroup(B_HORIZONTAL)
			.Add(fToolBar)
			//.AddGlue()
		.End()
		.AddGroup(B_VERTICAL, 0)	
			.Add(fTimeBar)
			.Add(fTrackView)
		.End()
		.Add(fInfoToolBar)
	.End();

	//Pool.SetLoop((transport_view->loop->Value() == B_CONTROL_ON));
	SetSizeLimits(MIN_W ,MAX_W, MIN_H , MAX_H);
	SetPulseRate(50000);
}


BMenuBar*
FaberWindow::_BuildMenu()
{
	BMenu		*menu;
	BMenuItem	*menuItem;

	fMainMenuBar = new MyMenuBar("MenuBar");
	
	menu = new BMenu(B_TRANSLATE("File"));
	fMainMenuBar->AddItem(menu);
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("New"), new BMessage(NEW), KeyBind.GetKey("FILE_NEW"), KeyBind.GetMod("FILE_NEW")));

	recent_menu = new BMenu(B_TRANSLATE("Open..."));
	UpdateRecent();
	menu->AddItem(recent_menu);
	BMenuItem *openitem = menu->FindItem(B_TRANSLATE("Open..."));
	openitem->SetShortcut(KeyBind.GetKey("FILE_OPEN"),KeyBind.GetMod("FILE_OPEN"));
	openitem->SetMessage(new BMessage(OPEN));
//	openitem->SetShortcut('O', 0);

	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Insert..."), new BMessage(INSERT), KeyBind.GetKey("FILE_INSERT"), KeyBind.GetMod("FILE_INSERT")));
	menuItem->SetEnabled(false);
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Append..."), new BMessage(APPEND), KeyBind.GetKey("FILE_APPEND"), KeyBind.GetMod("FILE_APPEND")));
	menuItem->SetEnabled(false);

	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Mix..."), new BMessage(OPEN_MIX), KeyBind.GetKey("FILE_MIX"), KeyBind.GetMod("FILE_MIX")));
	menuItem->SetEnabled(false);

	menu->AddSeparatorItem();
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Save"), new BMessage(SAVE), KeyBind.GetKey("FILE_SAVE"), KeyBind.GetMod("FILE_SAVE")));
	Pool.mn_save = menuItem;
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Save As..."), new BMessage(SAVE_AS), KeyBind.GetKey("FILE_SAVE_AS"), KeyBind.GetMod("FILE_SAVE_AS")));
	Pool.mn_save_as = menuItem;
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Save Selection..."), new BMessage(SAVE_SELECTION), KeyBind.GetKey("FILE_SAVE_SELECTION"), KeyBind.GetMod("FILE_SAVE_SELECTION")));
	Pool.mn_save_sel = menuItem;
	menu->AddSeparatorItem();
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Preferences..."), new BMessage(PREFERENCES), KeyBind.GetKey("PREFERENCES"), KeyBind.GetMod("PREFERENCES")));
	menu->AddSeparatorItem();
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Quit"), new BMessage(B_QUIT_REQUESTED), KeyBind.GetKey("FILE_QUIT"), KeyBind.GetMod("FILE_QUIT")));

	menu = new BMenu(B_TRANSLATE("Edit"));
	Pool.menu_edit = menu;
	fMainMenuBar->AddItem(menu);
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Undo"), new BMessage(UNDO), KeyBind.GetKey("UNDO"), KeyBind.GetMod("UNDO")));
	Pool.mn_undo = menuItem;

	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Redo"), new BMessage(REDO), KeyBind.GetKey("REDO"), KeyBind.GetMod("REDO")));
	Pool.mn_redo = menuItem;

	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Enable Undo"), new BMessage(UNDO_ENABLE), KeyBind.GetKey("UNDO_ENABLE"), KeyBind.GetMod("UNDO_ENABLE")));
	Pool.mn_undo_enable = menuItem;
	menuItem->SetMarked(Prefs.save_undo);
	menu->AddSeparatorItem();
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Copy"), new BMessage(B_COPY), KeyBind.GetKey("COPY"), KeyBind.GetMod("COPY")));
	Pool.mn_copy = menuItem;
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Copy & Silence"), new BMessage(COPY_SILENCE), KeyBind.GetKey("COPY_SILENCE"), KeyBind.GetMod("COPY_SILENCE")));
	Pool.mn_copy_silence = menuItem;
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Cut"), new BMessage(B_CUT), KeyBind.GetKey("CUT"), KeyBind.GetMod("CUT")));
	Pool.mn_cut = menuItem;
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Paste"), new BMessage(B_PASTE), KeyBind.GetKey("PASTE"), KeyBind.GetMod("PASTE")));
	Pool.mn_paste = menuItem;
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Paste as new"), new BMessage(PASTE_NEW), KeyBind.GetKey("PASTE_NEW"), KeyBind.GetMod("PASTE_NEW")));
	Pool.mn_paste_new = menuItem;
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Paste & mix"), new BMessage(PASTE_MIXED), KeyBind.GetKey("EDIT_PASTE_MIX"), KeyBind.GetMod("EDIT_PASTE_MIX")));
	menuItem->SetEnabled(false);
	
	Pool.mn_paste_mix = menuItem;
	
	/*menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Copy to stack"), new BMessage(TO_STACK), KeyBind.GetKey("COPY_TO_STACK"), KeyBind.GetMod("COPY_TO_STACK")));
	Pool.mn_copy_to_stack = menuItem;*/

	menu->AddSeparatorItem();
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Select All"), new BMessage(B_SELECT_ALL), KeyBind.GetKey("SELECT_ALL"), KeyBind.GetMod("SELECT_ALL")));
	Pool.mn_select_all = menuItem;
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Unselect All"), new BMessage(UNSELECT_ALL), KeyBind.GetKey("UNSELECT_ALL"), KeyBind.GetMod("UNSELECT_ALL")));
	Pool.mn_unselect = menuItem;
	menu->AddSeparatorItem();
	BMenu *sub = new BMenu(B_TRANSLATE("Zero Crossings"));
	Pool.menu_zero = sub;
	sub->AddItem(new BMenuItem(B_TRANSLATE("Inwards"), new BMessage(ZERO_IN), KeyBind.GetKey("ZERO_IN"), KeyBind.GetMod("ZERO_IN")));
	sub->AddItem(new BMenuItem(B_TRANSLATE("Outwards"), new BMessage(ZERO_OUT), KeyBind.GetKey("ZERO_OUT"), KeyBind.GetMod("ZERO_OUT")));
	sub->AddItem(new BMenuItem(B_TRANSLATE("Left to Left"), new BMessage(ZERO_LL), KeyBind.GetKey("ZERO_LL"), KeyBind.GetMod("ZERO_LL")));
	sub->AddItem(new BMenuItem(B_TRANSLATE("Left to Right"), new BMessage(ZERO_LR), KeyBind.GetKey("ZERO_LR"), KeyBind.GetMod("ZERO_LR")));
	sub->AddItem(new BMenuItem(B_TRANSLATE("Right to Left"), new BMessage(ZERO_RL), KeyBind.GetKey("ZERO_RL"), KeyBind.GetMod("ZERO_RL")));
	sub->AddItem(new BMenuItem(B_TRANSLATE("Right to Right"), new BMessage(ZERO_RR), KeyBind.GetKey("ZERO_RR"), KeyBind.GetMod("ZERO_RR")));
	menu->AddItem(sub);
	
	menu->AddSeparatorItem();
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Clear"), new BMessage(CLEAR), KeyBind.GetKey("CLEAR"), KeyBind.GetMod("CLEAR")));
	Pool.mn_clear = menuItem;
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Trim"), new BMessage(TRIM), KeyBind.GetKey("TRIM"), KeyBind.GetMod("TRIM")));
	Pool.mn_trim = menuItem;
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Change frequency..."), new BMessage(SET_FREQUENCY), KeyBind.GetKey("SET_FREQ"), KeyBind.GetMod("SET_FREQ")));
	Pool.mn_set_freq = menuItem;
	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Resample"), new BMessage(RESAMPLE), KeyBind.GetKey("RESAMPLE"), KeyBind.GetMod("RESAMPLE")));
	Pool.mn_resample = menuItem;

	menu = new BMenu(B_TRANSLATE("Transform"));
	Pool.menu_transform = menu;
	fMainMenuBar->AddItem(menu);

	menu = new BMenu(B_TRANSLATE("Analyze"));
	Pool.menu_analyze = menu;
	fMainMenuBar->AddItem(menu);

	menu = new BMenu(B_TRANSLATE("Generate"));
	menu->SetEnabled(false);
	Pool.menu_generate = menu;
	fMainMenuBar->AddItem(menu);

	menu = new BMenu(B_TRANSLATE("Help"));
	fMainMenuBar->AddItem(menu);

	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Help"),
		new BMessage(HELP), KeyBind.GetKey("HELP"), KeyBind.GetMod("HELP")));

	menu->AddSeparatorItem();

	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Homepage"),
		new BMessage(HOMEPAGE), KeyBind.GetKey("HOMEPAGE"), KeyBind.GetMod("HOMEPAGE")));

	menu->AddSeparatorItem();

	menu->AddItem(menuItem = new BMenuItem(B_TRANSLATE("About"),
		new BMessage(ABOUT), KeyBind.GetKey("ABOUT"), KeyBind.GetMod("ABOUT")));

	SetKeyMenuBar(NULL);
	return fMainMenuBar;
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
	
	while(recent_menu->ItemAt(0))
		recent_menu->RemoveItem(recent_menu->ItemAt(0));

	be_roster->GetRecentDocuments(&msg,10,"audio");
	while(msg.FindRef("refs",i,&eref) == B_OK){
		e.SetTo(&eref);
		if(e.InitCheck() == B_OK){
			e.GetName(name);
			msgout = new BMessage(B_REFS_RECEIVED);//DO_OPEN);
			msgout->AddRef("refs",&eref);
			recent_menu->AddItem(menuItem = new BMenuItem(name,msgout));
			menuItem->SetTarget(be_app);
		}
		i++;
	}
}


bool
FaberWindow::QuitRequested()
{
	if (!Pool.IsChanged(1)){
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

	case OPEN:
		if (!Pool.IsChanged())
			((FaberApp*)be_app)->fOpenPanel->Show();
		break;
	
	case SAVE:			// need to add default setting in the save-panel for this
	case SAVE_AS:
		if (Pool.sample_type == NONE)	return;
		Pool.save_selection = false;
		((FaberApp*)be_app)->fSavePanel->Window()->SetTitle(B_TRANSLATE("Save soundfile..."));
		((FaberApp*)be_app)->fSavePanel->Show();
		break;
	
	case SAVE_SELECTION:
		if (Pool.selection == NONE || Pool.sample_type == NONE)	return;
		((FaberApp*)be_app)->fSavePanel->Window()->SetTitle(B_TRANSLATE("Save selection..."));
		Pool.save_selection = true;
		((FaberApp*)be_app)->fSavePanel->Show();
		break;
	
	case UNDO:
		Pool.Undo();
		break;
		
	case B_SELECT_ALL:
		if (Pool.size == 0)	break;
		Pool.SelectAll();
		break;

	case UNSELECT_ALL:
		if (Pool.size == 0)	break;
		Pool.DeSelectAll();
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
		BPoint p;
		message->FindPoint("_drop_point_", &p);
		p = fTrackView->ConvertFromScreen(p);
		BRect r = fTrackView->Bounds();
		if (r.Contains(p)){
			Pool.selection = NONE;
			Pool.pointer = (int32)(Pool.l_pointer + p.x * (Pool.r_pointer - Pool.l_pointer)/Bounds().Width());
			ClipBoard.Paste();
		}
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
		
		if (x < Pool.m_SampleView->Bounds().Width()/64)	break;
		
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
		Pool.update_index = true;
		Pool.UpdateMenu();
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
		Pool.UpdateMenu();
		Pool.update_index = true;
		RedrawWindow();
		break;
	case ZOOM_FULL:
		if (Pool.size == 0)	break;
		Pool.l_pointer = 0;
		Pool.r_pointer = Pool.size;
		Pool.UpdateMenu();
		Pool.update_index = true;
		RedrawWindow();
		break;
	case ZOOM_SELECTION:
		if (Pool.size == 0)	break;
		if (Pool.selection != NONE){
			Pool.l_pointer = Pool.pointer;
			Pool.r_pointer = Pool.r_sel_pointer;
		}
		Pool.UpdateMenu();
		Pool.update_index = true;
		RedrawWindow();
		break;
	case ZOOM_LEFT:
		if (Pool.size == 0 || Pool.selection==NONE)	break;
		x = fTrackView->Bounds().IntegerWidth()/6;
		Pool.l_pointer = Pool.pointer -x/2;	// window to selection
		if (Pool.l_pointer<0)	Pool.l_pointer = 0;
		Pool.r_pointer = Pool.l_pointer + x;

		Pool.UpdateMenu();
		Pool.update_index = true;
		RedrawWindow();
		break;
	case ZOOM_RIGHT:
		if (Pool.size == 0 || Pool.selection==NONE)	break;
		x = fTrackView->Bounds().IntegerWidth()/6;
		Pool.l_pointer = Pool.r_sel_pointer - x/2;	// window to selection
		if (Pool.l_pointer<0)	Pool.l_pointer = 0;
		Pool.r_pointer = Pool.l_pointer + x;
		if (Pool.r_pointer > Pool.size){
			Pool.r_pointer = Pool.size;
			Pool.l_pointer = Pool.r_pointer - x;
		}

		Pool.UpdateMenu();
		Pool.update_index = true;
		RedrawWindow();
		break;
		
	case EDIT_L:
		if (Pool.selection != NONE)
			Pool.selection = LEFT;
		Pool.UpdateMenu();
		fTrackView->Draw(fTrackView->Bounds());
		break;	
	case EDIT_R:
		if (Pool.selection != NONE)
			Pool.selection = RIGHT;
		Pool.UpdateMenu();
		fTrackView->Draw(fTrackView->Bounds());
		break;	
	case EDIT_B:
		if (Pool.selection != NONE)
			Pool.selection = BOTH;
		Pool.UpdateMenu();
		fTrackView->Draw(fTrackView->Bounds());
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
		Pool.update_index = true;
		RedrawWindow();
		break;

	case TRANSPORT_REW_ALL:
		x = Pool.r_pointer - Pool.l_pointer;
		Pool.pointer = 0;
		Pool.l_pointer = 0;
		Pool.r_pointer = x;
		Pool.update_index = true;
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
		Pool.update_index = true;
		RedrawWindow();
		break;

	case TRANSPORT_FWD_ALL:
		x = Pool.r_pointer - Pool.l_pointer;
		Pool.pointer = Pool.size;;
		Pool.r_pointer = Pool.pointer;
		Pool.l_pointer = Pool.pointer - x;
		Pool.update_index = true;
		RedrawWindow();
		break;
	
	case TRANSPORT_HOME:
		Pool.pointer = Pool.l_pointer;
		Pool.update_index = true;
		RedrawWindow();
		break;
	
	case TRANSPORT_END:
		Pool.pointer = Pool.r_pointer;
		Pool.update_index = true;
		RedrawWindow();
		break;
	
	case TRANSPORT_LEFT:
		x = Pool.r_pointer - Pool.l_pointer;
		Pool.l_pointer -= x/2;
		if (Pool.l_pointer<0)	Pool.l_pointer = 0;
		Pool.r_pointer = Pool.l_pointer + x;
		Pool.update_index = true;
		RedrawWindow();
		break;

	case TRANSPORT_RIGHT:
		x = Pool.r_pointer - Pool.l_pointer;
		Pool.l_pointer += x/2;
		if (Pool.l_pointer>(Pool.size-x))	Pool.l_pointer = Pool.size-x;
		Pool.r_pointer = Pool.l_pointer + x;
		Pool.update_index = true;
		RedrawWindow();
		break;
	
	case UNDO_ENABLE:
		Prefs.save_undo = !Prefs.save_undo;
		Pool.UpdateMenu();
		break;
	
	case ABOUT:
	{
		BPoint p;
		BRect r = Frame();

		p.x = (r.left+r.right)/2;
		p.y = (r.top+r.bottom)/2;

		AboutBox* box = new AboutBox(p);
	}
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
	
	case NEW:
	{	app_info info;
		be_app->GetAppInfo(&info);
		be_roster->Launch(info.signature);
	}	break;
		
	case PASTE_NEW:
	{	app_info info;
		be_app->GetAppInfo(&info);
		be_roster->Launch(info.signature, new BMessage(B_PASTE));
	}	break;
		
	case HOMEPAGE:
	{
		const char* homepage = FABER_HOMEPAGE;
		be_roster->Launch("text/html",1, const_cast<char**>(&homepage));
		break;
	}
		
	case PREFERENCES:
		if (Pool.PrefWin != NULL && Pool.PrefWin->LockLooper()) {
			if (Pool.PrefWin->IsHidden())
				Pool.PrefWin->Show();
			Pool.PrefWin->Activate();
			Pool.PrefWin->UnlockLooper();
		} else {
			//Create a new Prefs windos?!
		}
		break;

	case UPDATE:
		FindView("Sample view")->Pulse();
		FindView("InfoToolBar")->Pulse();
		break;
	
	case REDRAW:
		Pool.sample_view_dirty = true;	// update the sample-view
		Pool.update_index = true;
		// NOTE: Enabling it cause the TrackView
		// selection to have drawing problems.
		//Pool.ResetIndexView();
		RedrawWindow();
		break;
	
	case TOOL_SELECT:
		Pool.tool_mode = SELECT_TOOL;
		Pool.UpdateMenu();
		break;

	case TOOL_DRAW:
		Pool.tool_mode = DRAW_TOOL;
		Pool.UpdateMenu();
		break;

	case TOOL_PLAY:
		Pool.tool_mode = PLAY_TOOL;
		Pool.UpdateMenu();
		break;

	case TOOL_JOGG:
		Pool.tool_mode = SCRUB_TOOL;
		Pool.UpdateMenu();
		break;

	case SPECTRUM:
		(new SpectrumWindow());
		break;
		
	case SAMPLE_SCOPE:
		(new SampleScopeWindow());
		break;
		
	case SET_FREQUENCY:
		(new FreqWindow(BPoint((Frame().left+Frame().right)/2,
			(Frame().top+Frame().bottom)/2)));
		break;

	case RESAMPLE_DO:
		DoResample();
		break;

	case RESAMPLE:
		(new ResampleWindow(BPoint( (Frame().left+Frame().right)/2,
			(Frame().top+Frame().bottom)/2)));
		break;

	case CLEAR:
		if (!Pool.PrepareFilter())	break;
		ClipBoard.DoSilence();
		Pool.changed = true;
		Pool.UpdateMenu();
		Pool.HideProgress();
		Pool.ResetIndexView();
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
//		message->PrintToStream();
		message->FindInt32("key", &raw_key);
		message->FindInt32("modifiers", &mod);
		message->FindInt32("raw_char", &key);

		// now do some conversions for combinations
		if (key == B_FUNCTION_KEY){
			key = 12+raw_key;
		}else if (key>='a' && key<='z')
			key -= ('a'-'A');
		
		msg = KeyBind.GetMessage(key, mod);
		if (msg){
			BMessage new_message(msg);
			new_message.AddString("language_key", KeyBind.GetID(key, mod));
			PostMessage(&new_message);
		}
		break;

	case B_MIME_DATA:			// let the app parse drops
	case B_SIMPLE_DATA:
		be_app->PostMessage(message);
//		message->PrintToStream();
		break;				

	default:
//		message->PrintToStream();
		BWindow::MessageReceived(message);
	}
	
	if (CurrentFocus())
		CurrentFocus()->MakeFocus(false);
}


void
FaberWindow::RedrawWindow()
{
	Lock();
	//for (int32 i = 0; i < CountChildren(); i++)
	//	ChildAt(i)->Invalidate();

 	fToolBar->Invalidate();
	fTrackView->Invalidate();
	fTimeBar->Invalidate();

	Unlock();	
}


void
FaberWindow::UpdateToolBar()
{

}
