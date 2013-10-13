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


#include <GroupLayoutBuilder.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <String.h>
#include <Path.h>
#include <TranslationKit.h>
#include <TranslationUtils.h>
#include <LayoutBuilder.h>
#include <ControlLook.h>

#include "FaberDefs.h"
#include "PrefGeneral.h"
#include "SpinControl.h"
#include "WindowsManager.h"

#include <stdio.h>

#define BOOL_CHANGED				'shwG'
#define PEAK_LEVEL					'peak'
#define SPIN_CHANGED				'spnC'
#define SET_TIME					'setT'
#define SET_TEMP					'stTP'

/*******************************************************
*   Setup the main view. Add in all the niffty components
*   we have made and get things rolling
*******************************************************/
PrefGeneral::PrefGeneral()
	:
	BView("Prefs general", B_FOLLOW_ALL | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_NAVIGABLE)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BMenuField *menu;

	c_grid = new BCheckBox("grid", B_TRANSLATE("Show grid"), new BMessage(BOOL_CHANGED));
	if (Prefs.show_grid)
		c_grid->SetValue(B_CONTROL_ON);

	c_peak = new BCheckBox("peak", B_TRANSLATE("Show Peak lines"), new BMessage(BOOL_CHANGED));
	if (Prefs.show_peak)
		c_peak->SetValue(B_CONTROL_ON);

	s_peak = new SpinControl("Peaklevel", B_TRANSLATE("Peak level %"),
		new BMessage(PEAK_LEVEL), 1, 100, Prefs.peak*100, 2);

	//s_peak->SetDivider(r.Width()*.4);

	c_paste = new BCheckBox("paste", B_TRANSLATE("Select Pasted clip"), new BMessage(BOOL_CHANGED));
	if (Prefs.select_after_paste)
		c_paste->SetValue(B_CONTROL_ON);

	c_follow_playing = new BCheckBox("follow", B_TRANSLATE("Follow while playing"), new BMessage(BOOL_CHANGED));

	if (Prefs.follow_playing)
		c_follow_playing->SetValue(B_CONTROL_ON);

	c_play = new BCheckBox("play", B_TRANSLATE("Play when loaded"), new BMessage(BOOL_CHANGED));

	if (Prefs.play_when_loaded)
		c_play->SetValue(B_CONTROL_ON);

	c_double = new BCheckBox("double", B_TRANSLATE("Select All on double click"), new BMessage(BOOL_CHANGED));
	if (Prefs.select_all_on_double)
		c_double->SetValue(B_CONTROL_ON);

	c_drag_drop = new BCheckBox("dragndrop", B_TRANSLATE("Drag 'n Drop"), new BMessage(BOOL_CHANGED));
	if (Prefs.drag_drop)
		c_drag_drop->SetValue(B_CONTROL_ON);

	time = new BPopUpMenu(B_TRANSLATE("Time display"));
	menu = new BMenuField(NULL,B_TRANSLATE("Time display"),time);

	BMessage *m = new BMessage(SET_TIME);
	m->AddInt32("time",DISPLAY_SAMPLES);
	time->AddItem(menu_sample = new BMenuItem(B_TRANSLATE("Samples"), m));

	if (Prefs.display_time == DISPLAY_SAMPLES)
		menu_sample->SetMarked(true);

	m = new BMessage(SET_TIME);
	m->AddInt32("time",DISPLAY_TIME);
	time->AddItem(menu_time = new BMenuItem(B_TRANSLATE("Time"), m));

	if (Prefs.display_time == DISPLAY_TIME)
		menu_time->SetMarked(true);

	menu->SetDivider(be_plain_font->StringWidth(B_TRANSLATE("Time Display")) +10);

	temp_file = new BTextControl("temp_file", B_TRANSLATE("Temporary directory"),
		Prefs.temp_dir.String(), new BMessage(SET_TEMP));

	s_free = new SpinControl("s_free", B_TRANSLATE("Minimum diskspace to keep free (Mb)"),
		new BMessage(SPIN_CHANGED), 10, 10000, Prefs.keep_free, 10);

	const float spacing = be_control_look->DefaultItemSpacing();

	SetLayout(new BGroupLayout(B_HORIZONTAL, spacing));
	AddChild(BGroupLayoutBuilder(B_VERTICAL)
		.AddGroup(B_VERTICAL, spacing)
			.Add(c_grid, 0)
			.Add(c_peak, 1)
			.Add(s_peak, 2)
			.Add(c_paste, 3)
			.Add(c_follow_playing, 4)
			.Add(c_play, 5)
			.Add(c_double, 6)
			.Add(c_drag_drop, 7)
			.Add(menu, 8)
			.Add(temp_file, 9)
			.Add(s_free, 10)
			.AddGlue()
		.End()
		.SetInsets(spacing, spacing, spacing, spacing)
	);
}


/*******************************************************
*  
*******************************************************/
PrefGeneral::~PrefGeneral()
{
}

/*******************************************************
*  
*******************************************************/
void
PrefGeneral::AttachedToWindow()
{
	c_grid->SetTarget(this);
	c_paste->SetTarget(this);
	c_play->SetTarget(this);
	c_double->SetTarget(this);
	c_peak->SetTarget(this);
	c_follow_playing->SetTarget(this);
	s_peak->SetTarget(this);
	s_free->SetTarget(this);
	c_drag_drop->SetTarget(this);
	temp_file->SetTarget(this);
	time->SetTargetForItems(this);

	if (Prefs.display_time == DISPLAY_SAMPLES) {
		menu_sample->SetMarked(true);
		menu_time->SetMarked(false);
	} else {
		menu_time->SetMarked(true);
		menu_sample->SetMarked(false);
	}
}

/*******************************************************
*
*******************************************************/
void
PrefGeneral::MessageReceived(BMessage* msg)
{
	char *lname = NULL;
	int32 i;

	switch(msg->what){

	case BOOL_CHANGED:
		Prefs.show_grid = (c_grid->Value() == B_CONTROL_ON);
		Prefs.select_after_paste = (c_paste->Value() == B_CONTROL_ON);
		Prefs.select_all_on_double = (c_double->Value() == B_CONTROL_ON);
		Prefs.play_when_loaded = (c_play->Value() == B_CONTROL_ON);
		Prefs.show_peak = (c_peak->Value() == B_CONTROL_ON);
		Prefs.follow_playing = (c_follow_playing->Value() == B_CONTROL_ON);
		Prefs.drag_drop = (c_drag_drop->Value() == B_CONTROL_ON);
		/*Pool.update_draw_cache = true;	// update the draw cache
		Pool.sample_view_dirty = true;	// update the sample-view*/
		// WindowsManager::MainWindow()->RedrawWindow();
		break;
	
	case PEAK_LEVEL:
		Prefs.peak = s_peak->Value()/100.0;
		/*Pool.sample_view_dirty = true;	// update the sample-view
		Pool.update_draw_cache = true;	// update the draw cache*/
		// WindowsManager::MainWindow()->RedrawWindow();
		break;

	case SPIN_CHANGED:
		Prefs.keep_free = s_free->Value();
		break;
	
	case SET_TIME:
		msg->FindInt32("time", &i);
		Prefs.display_time = i;
		// WindowsManager::MainWindow()->RedrawWindow();
		break;
	
	case SET_TEMP:
		Prefs.temp_dir.SetTo(temp_file->Text());
		(new BAlert(NULL,B_TRANSLATE("TEMP_CHANGED"),
			B_TRANSLATE("OK")))->Go();
		break;

	default:
		BView::MessageReceived(msg);
		break;   
	}
}
