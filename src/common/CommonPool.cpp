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
#include <Application.h>
#include <Directory.h>
#include <FindDirectory.h>
#include <Message.h>
#include <NodeMonitor.h>
#include <Path.h>
#include <Roster.h>
#include <SoundPlayer.h>
#include <Cursor.h>

//#include "Globals.h"
#include "CommonPool.h"
#include "ProgressWindow.h"
#include "Settings.h"
#include "MyClipBoard.h"
#include "FaberWindow.h"
#include "Shortcut.h"
#include "PeakFile.h"
#include "VMSystem.h"
#include "Filters.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // header with crypt in it :)

// this is the list with filters for the basic version
extern filter_info __FilterList[];

extern void BufferPlayer(void *theCookie, void *buffer, size_t size, const media_raw_audio_format &format);

// our Global def
CommonPool Pool;
extern cookie_record play_cookie;

extern const char *APP_SIGNATURE;

/*******************************************************
*   The Common pool is a group of all plugins/addons/functions
*******************************************************/
CommonPool::CommonPool(){
	// To be save set them to nil 
	progress = NULL;
	player = NULL;
	PrefWin = NULL;
	
	m_playing = false;
	play_pointer = 0;
	changed = false;
	save_mode = 0;
	last_pointer = 0;
	sample_view_dirty = true;	// update the sample-view
	update_draw_cache = true;	// update the draw cache
	update_peak = false;
	update_index = true;
	play_cookie.buffer = NULL;

	tool_mode = SELECT_TOOL;

	for (int i=0; i<8; i++)
		BufferHook[i] = NULL;
}

/*******************************************************
*   
*******************************************************/
void CommonPool::Init(){
//   thread_id tid = spawn_thread(_LoadFilters_, I_FILTER_LOADER, B_NORMAL_PRIORITY, (void *)this);
//	if(tid < 0){
//		debugger(THREAD_FAIL_MSG);
//	}else{
//		resume_thread(tid);
//	}
	play_mode = NONE;
	pointer = 0;
	play_pointer = 0;
	l_pointer = 0;
	r_pointer = 0;
	r_sel_pointer = 0;
	size = 0;
	selection = NONE;
	sample_type = NONE;
	sample_bits = 16;
	frequency = 41400.0;

	play_cookie.buffer = new float[16384];
}

/*******************************************************
*   
*******************************************************/
CommonPool::~CommonPool(){
	if (progress && progress->Lock())  {
		progress->Quit();
		progress->Unlock();
	}
	if (IsPlaying())
		StopPlaying();

	if (play_cookie.buffer)
		delete [] play_cookie.buffer;

#ifndef __VM_SYSTEM				// RAM
	if (sample_memory)
		free(sample_memory);
#endif

	if (PrefWin && PrefWin->Lock()) {
		PrefWin->Quit();
		PrefWin->Unlock();
	}
//	if (tt)	delete tt;
}

// refills the PeakFile Cache
void CommonPool::ResetIndexView()
{
	if (sample_type == NONE) return;

	StartProgress(B_TRANSLATE("Indexing..."), size);

	Peak.Init(size+1, (Pool.sample_type == MONO) );
	Peak.CreatePeaks(0, size+1, size+1);
	Pool.update_index = true;		// update the draw cache

	HideProgress();
}

/*******************************************************
*   SoundPlayer
*******************************************************/
void CommonPool::InitBufferPlayer(float f)
{
	media_raw_audio_format format;
	memset(&format, 0, sizeof(format));
	format.frame_rate = f;									// see if you can set frequency
	format.format = media_raw_audio_format::B_AUDIO_FLOAT;	// 32 bit float
	format.channel_count = 2;								// stereo
	format.buffer_size = Prefs.buffer_size;					// media buffer
	format.byte_order = 2;
	
	play_cookie.pause = false;

	if (player){
		player->Stop();
		delete player;
	}
	player = new BSoundPlayer(&format, "Faber", BufferPlayer, NULL, &play_cookie);

	format = Pool.player->Format();
	system_frequency = format.frame_rate;
}

#ifndef __VM_SYSTEM				// RAM
	void CommonPool::StartPlaying(int64 p, bool end)
	{
		m_playing = true;
		if (p){
			play_cookie.mem = sample_memory +p*4;
		}else{
			play_cookie.mem = sample_memory;
		}
		play_cookie.mono = (sample_type == MONO);
		play_cookie.start_mem = play_cookie.mem;

		if (end | (selection == NONE))
			play_cookie.end_mem = sample_memory + size*sample_type;
		else
			play_cookie.end_mem = sample_memory + r_sel_pointer*sample_type;

		play_cookie.end = end;
		play_cookie.frequency = frequency;
		play_cookie.add = 0;
		last_pointer = 0;

		player->Start();
		player->SetHasData(true);
	}
#else	// VM
	void CommonPool::StartPlaying(int64 p, bool end)
	{
		m_playing = true;
		if (p){
			play_cookie.mem = p;
		}else{
			play_cookie.mem = 0;
		}
		play_cookie.mono = (sample_type == MONO);
		play_cookie.start_mem = play_cookie.mem;

		if (end | (selection == NONE))
			play_cookie.end_mem = size*sample_type;
		else
			play_cookie.end_mem = r_sel_pointer*sample_type;

		play_cookie.end = end;
		play_cookie.frequency = frequency;
		play_cookie.add = 0;
		last_pointer = 0;

		// set play cache
		VM.SetPlayPointer(play_cookie.mem);

		player->Start();
		player->SetHasData(true);
	}
#endif

void CommonPool::StopPlaying()
{
	m_playing = false;

	#ifdef __VM_SYSTEM
	VM.StopCache();
	#endif

	player->Stop();
}

bool CommonPool::IsPlaying()
{
	return m_playing;
}

bool CommonPool::SetLoop(bool l)
{
	bool ret = play_cookie.loop;
	play_cookie.loop = l;
	return ret;
}

bool CommonPool::PrepareFilter()
{
	if (sample_type == NONE)
		return false;

	player->Stop();
	mainWindow->PostMessage(TRANSPORT_STOP);		// stop playing

	if (selection == NONE)		SelectAll();	// select all if noe is selected
	if (Prefs.save_undo)		SaveUndo();			// save undo data

	return true;
}

int32 CommonPool::SetPlayHook(void (*in)(float*, size_t, void*), int32 index, void *cookie)
{
	for (int i=index; i<PLAY_HOOKS; i++){
		if (BufferHook[i] == NULL){
			BufferHook[i] = in;
			BufferCookie[i] = cookie;
			return i;	// return installed handler
		}
	}
	return -1;		// error
}

void CommonPool::RemovePlayHook(void (*in)(float*, size_t, void *), int32 index)
{
	if (index==-1){
		for (int i=0; i<PLAY_HOOKS; i++){
			if (BufferHook[i] == in){
				BufferHook[i] = NULL;
				BufferCookie[i] = NULL;
				return;
			}
		}
	}else{
		if (BufferHook[index] == in){
			BufferHook[index] = NULL;
			BufferCookie[index] = NULL;
			return;
		}
	}
}

/*******************************************************
*   Select All
*******************************************************/
void CommonPool::SelectAll()
{
	if (sample_type != NONE){
		pointer = 0;
		r_sel_pointer = size;
		selection = BOTH;
		UpdateMenu();
		RedrawWindow();
	}
}

/*******************************************************
*   DeSelect All
*******************************************************/
void CommonPool::DeSelectAll()
{
	if (sample_type != NONE && Pool.selection != NONE){
		selection = NONE;
		r_sel_pointer = 0;
		UpdateMenu();
		RedrawWindow();
	}
}

/*******************************************************
*   Redraw Main Window
*******************************************************/
void CommonPool::RedrawWindow()
{
	sample_view_dirty = true;	// update the sample-view
	mainWindow->RedrawWindow();
}

/*******************************************************
*   Update the menus
*******************************************************/
void CommonPool::UpdateMenu()
{
	BMenuItem *menuItem = NULL;
	mainWindow->Lock();

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
	while(__FilterList[filter].name != NULL)
	{
		if (strcmp(__FilterList[filter].name, "---") == 0)
		{
			menu_transform->AddSeparatorItem();
		}
		else
		{
			// can do some stuff to organise menu here
		 	filter_msg = new BMessage(RUN_FILTER);
			filter_msg->AddInt32("filter", filter);
			sprintf(name, B_TRANSLATE(__FilterList[filter].name));
			if ( __FilterList[filter].type & FILTER_GUI )
				strcat(name, "...");
			menu_transform->AddItem(menuItem = new BMenuItem(name, filter_msg, KeyBind.GetKey(__FilterList[filter].name), KeyBind.GetMod(__FilterList[filter].name)));
			menuItem->SetEnabled( __FilterList[filter].type & Pool.sample_type );
		}
		filter++;
	}

	while (menu_analyze->ItemAt(0)){
		menuItem = menu_analyze->ItemAt(0);
		menu_analyze->RemoveItem(menuItem);
		delete menuItem;
	}
	
	menu_analyze->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Spectrum Analyzer"), new BMessage(SPECTRUM), KeyBind.GetKey("SPECTRUM_ANALYZER"), KeyBind.GetMod("SPECTRUM_ANALYZER")));
	menu_analyze->AddItem(menuItem = new BMenuItem(B_TRANSLATE("Sample Scope"), new BMessage(SAMPLE_SCOPE), KeyBind.GetKey("SAMPLE_SCOPE"), KeyBind.GetMod("SAMPLE_SCOPE")));

	menu_transform->SetEnabled(sample_type != NONE);	// transform menu
	menu_analyze->SetEnabled(sample_type != NONE);		// analyzers menu

//	menu_generate->SetEnabled(false);					// generation menu

	menu_zero->SetEnabled(sample_type != NONE);			// zero cross menu
	mn_trim->SetEnabled(selection != NONE);				// trim
	mn_save_sel->SetEnabled(selection != NONE);			// save selection
	mn_save->SetEnabled(sample_type != NONE && Pool.changed);			// save
	mn_save_as->SetEnabled(sample_type != NONE);		// save as
	mn_set_freq->SetEnabled(sample_type != NONE);		// set frequency
	mn_resample->SetEnabled(sample_type != NONE);		// resample
	mn_select_all->SetEnabled(sample_type != NONE);		// select all
	mn_unselect->SetEnabled(selection != NONE);			// DeSelect all
	mn_cut->SetEnabled(selection != NONE);				// cut
	mn_copy->SetEnabled(selection != NONE);				// copy
	mn_copy_silence->SetEnabled(selection != NONE);		// copy & Silence
	mn_clear->SetEnabled(selection != NONE);			// clear
	mn_undo_enable->SetMarked(Prefs.save_undo);			// undo enabled
	mn_undo->SetEnabled(Hist.HasUndo());				// need history class for this
	mn_paste->SetEnabled(ClipBoard.HasClip());
	mn_paste_new->SetEnabled(ClipBoard.HasClip());

	mn_paste_mix->SetEnabled(ClipBoard.HasClip());
	mn_redo->SetEnabled(Hist.HasRedo());				// need history class for this
	//mn_copy_to_stack->SetEnabled(selection != NONE);	// copy to stack

	((FaberWindow*)mainWindow)->UpdateToolBar();
	mainWindow->Unlock();
}

/*******************************************************
*   Check for and handle changed files
*******************************************************/
bool CommonPool::IsChanged(int32 mode)
{
	if (changed){
		int32 k = (new BAlert(NULL,B_TRANSLATE("This project has changed. Do you want to save it now?"),
			B_TRANSLATE("Save"),B_TRANSLATE("Discard"),B_TRANSLATE("Cancel")))->Go();
		switch(k){
		case 0:
			save_selection = false;
			save_mode = mode;
			mainWindow->PostMessage(SAVE_AS);
			return true;
			break;
		case 1:
			return false;
			break;
		default:
			return true;
		}
	} else {
		return false;
	}
}

/*******************************************************
*   Handle UNDO
*******************************************************/
void CommonPool::SaveUndo()
{
	if (selection==NONE || !Prefs.save_undo)	return;

	Hist.Save(H_REPLACE, pointer, r_sel_pointer);
	UpdateMenu();
}

void CommonPool::Undo()
{
	Hist.Restore();
	Pool.ResetIndexView();
	UpdateMenu();
	RedrawWindow();
}

/*******************************************************
*   Progress Window
*******************************************************/
void CommonPool::StartProgress(const char *label, int32 max)
{
	if(progress->Lock()){
		progress->StartProgress(label, max);
		progress->Unlock();
	}
}

void CommonPool::ProgressUpdate(int32 delta)
{
	if(progress->Lock()){
		progress->SetProgress(delta);
		progress->Unlock();
	}
}

void CommonPool::SetProgressName(const char *name)
{
	if(progress->Lock()){
		progress->SetTitle(name);
		progress->Unlock();
	}
}

void CommonPool::HideProgress()
{
	if(progress->Lock()){
		if (!progress->IsHidden())
			progress->Hide();
		progress->Unlock();
	}
}

/*******************************************************
*   Dont forget to update the IDF_MIME_VER if any changes
*   are made to the structure or the Mime installation
*******************************************************/
status_t CommonPool::InstallMimeType(bool force){
/*	//Set Mime type for Inferno Document File
	BMimeType mime(DOCUMENT_MIME);
	bool inst = true;
	// if we need to force update then delete the old 
	// mime type out of the database
	if(force){
		mime.Delete();
	}
   
   // if its installed already then we should check if its 
   // the current version.  This is not really needed but 
   // it makes life easyer down the road if we ever want 
   // to change the type up or anything.
   if(mime.IsInstalled()){
      // if its not the current vir we should delete the thingy
      BMessage info;
      inst = false;
      if(mime.GetAttrInfo(&info) == B_NO_ERROR){
         // We tag each Mime setup with a ver number
         // so we can keep it uptoday if it ever changes
         // and we need to reinstall it
         // this is porly writen code as it does not realy
         // handle the cases correctly .. but thats ok for now
         int32 ver = -1;
         if(info.FindInt32("idf_mime_ver",&ver) != B_OK){ ver = -1; }
         if(ver < IDF_MIME_VER){
            inst = true;
         } 
      }
   }
   
   // We should now install it .. becase of a force or a First time run
   // or a wrong Version.
   if(inst){
      // Mime is already set to the IDF signature
      mime.Install();
      
      // install a pritty Icon that goes iwth our file
      BBitmap large_icon(BRect(0, 0, B_LARGE_ICON - 1, B_LARGE_ICON - 1), B_COLOR_8_BIT);
      BBitmap mini_icon(BRect(0, 0, B_MINI_ICON - 1, B_MINI_ICON - 1), B_COLOR_8_BIT);
      large_icon.SetBits(LargeInfernoIcon, large_icon.BitsLength(), 0, B_COLOR_8_BIT);
      mini_icon.SetBits(SmallInfernoIcon, mini_icon.BitsLength(), 0, B_COLOR_8_BIT);
      mime.SetIcon(&large_icon, B_LARGE_ICON);
      mime.SetIcon(&mini_icon, B_MINI_ICON);
      
      // Set up the Name and Description feilds just to be nice
      mime.SetShortDescription(IDF_MIME_NAME);
      mime.SetLongDescription(IDF_MIME_DESC);
      
      // This is VERRY important to do as it the reson we are installing the mime type
      mime.SetPreferredApp(APP_SIGNATURE);
      
      // Build the extentions list.  This is good if we are writeing to 
      // a NON-BFS disk. Mime types will not work there... but file
      // extentions DO!
      BMessage extmsg;
      extmsg.AddString("extensions","IDF");
      extmsg.AddString("extensions","idf");
      extmsg.AddString("extensions","Inferno");
      mime.SetFileExtensions(&extmsg);
      
      // add relevant Inferno fields to meta-mime type      
      // This is wher all the niffty Attributs get added.
      // This is not needed except if we dont then Tracker
      // will not pick up on them and the user will be upset
      BMessage msg;
            
      // first off add our current ver number
      msg.AddInt32("idf_mime_ver",IDF_MIME_VER);
      
      // Name
      msg.AddString("attr:public_name", "Image Name");
      msg.AddString("attr:name", NAME_ATTR);
      msg.AddInt32("attr:type", B_STRING_TYPE);
      msg.AddBool("attr:viewable", true);
      msg.AddBool("attr:editable", true);
      msg.AddInt32("attr:width", 120);
      msg.AddInt32("attr:alignment", B_ALIGN_LEFT);
      msg.AddBool("attr:extra", false);
      
      // Description
      msg.AddString("attr:public_name", "Description");
      msg.AddString("attr:name", DESC_ATTR);
      msg.AddInt32("attr:type", B_STRING_TYPE);
      msg.AddBool("attr:viewable", true);
      msg.AddBool("attr:editable", true);
      msg.AddInt32("attr:width", 120);
      msg.AddInt32("attr:alignment", B_ALIGN_LEFT);
      msg.AddBool("attr:extra", false);
      
      // Width
      msg.AddString("attr:public_name", "Width");
      msg.AddString("attr:name", WIDTH_ATTR);
      msg.AddInt32("attr:type", B_INT32_TYPE);
      msg.AddBool("attr:viewable", true);
      msg.AddBool("attr:editable", false);
      msg.AddInt32("attr:width", 120);
      msg.AddInt32("attr:alignment", B_ALIGN_LEFT);
      msg.AddBool("attr:extra", false);
      
      // Height
      msg.AddString("attr:public_name", "Height");
      msg.AddString("attr:name", HEIGHT_ATTR);
      msg.AddInt32("attr:type", B_INT32_TYPE);
      msg.AddBool("attr:viewable", true);
      msg.AddBool("attr:editable", false);
      msg.AddInt32("attr:width", 120);
      msg.AddInt32("attr:alignment", B_ALIGN_LEFT);
      msg.AddBool("attr:extra", false);
      
      // Now actaully add all of that stuff to the mime type .. 
      // and return ...
      return mime.SetAttrInfo(&msg);
   }
*/
	return B_OK;
}

#ifdef __VM_SYSTEM

void 
BufferPlayer(void *theCookie, void *buffer, size_t size, const media_raw_audio_format &format)
{
	// We're going to be cheap and only work for floating-point audio 
	if (format.format != media_raw_audio_format::B_AUDIO_FLOAT) { 
		return; 
	}
	
	bool stop_needed = false;
	size_t i; 
	float *buf = (float *) buffer; 
	size_t float_size = size/4; 
	cookie_record *cookie = (cookie_record *) theCookie;
	float left = 0.0, right = 0.0;
	float fraq = fabs(Pool.frequency/Pool.system_frequency);
	
	if ((Pool.selection == NONE) | cookie->end){
		cookie->end_mem = Pool.size*Pool.sample_type;
	}else{
		cookie->end_mem = Pool.r_sel_pointer*Pool.sample_type;
	}
	
	int32 mem_size = float_size * (int32)ceil(fraq);
	float *mem = cookie->buffer;
	VM.ReadCache( mem, mem_size );
//	VM.ReadBlockAt( cookie->mem, mem, mem_size );

	// Now fill the buffer with sound! 

	if (cookie->pause){
		for (i=0; i<float_size; i++) { 
			buf[i] = 0.0;
		}
	}else{
		if (Pool.sample_type == MONO){	//cookie->mono){
			for (i=0; i<float_size; i+=2){
				if (cookie->mem >= cookie->end_mem){
					if (cookie->loop){
						cookie->mem = Pool.pointer*Pool.sample_type;
						VM.SetPlayPointer( cookie->mem );
						VM.ReadCache( mem, mem_size );
					}else{
						buf[i] = 0.0;
						buf[i+1] = 0.0;
						stop_needed = true;
					}
				}
				if (!stop_needed){
					buf[i] = *mem;
					buf[i+1] = *mem;

					cookie->add += fraq;
					if (Pool.frequency>=0){
						while (cookie->add >= 1.0){
							cookie->mem++;
							mem++;
							--cookie->add;
						}
					}else{
						while (cookie->add >= 1.0){
							cookie->mem--;
							mem--;
							--cookie->add;
						}
						if (cookie->mem < 0)
							cookie->mem += cookie->end_mem;
					}
				}
			}
		}else{
			for (i=0; i<float_size; i+=2) { 
				if (cookie->mem >= cookie->end_mem){
					if (cookie->loop){
						cookie->mem = Pool.pointer*Pool.sample_type;
						VM.SetPlayPointer( cookie->mem );
						VM.ReadCache( mem, mem_size );
					}else{
						buf[i] = 0.0f;
						buf[i+1] = 0.0f;
						stop_needed = true;
					}
				}
				if (!stop_needed){
					switch(Pool.selection){
					case NONE:
					case BOTH:
						buf[i] = mem[0];
						buf[i+1] = mem[1];
						break;
					case LEFT:
						buf[i] = mem[0];
						buf[i+1] = mem[0];
						break;
					case RIGHT:
						buf[i] = mem[1];
						buf[i+1] = mem[1];
						break;
					}

					cookie->add += fraq;
					if (Pool.frequency>=0){
						while (cookie->add >= 1.0){
							cookie->mem+= 2;
							mem+=2;
							--cookie->add;
						}
					}else{
						while (cookie->add >= 1.0){
							cookie->mem-= 2;
							mem-=2;
							--cookie->add;
						}
						if (cookie->mem < 0)
							cookie->mem += cookie->end_mem;
					}
				}
			}
		}
	}
	
	Pool.last_pointer = cookie->mem;	// set the last played location
	if (Pool.sample_type == STEREO)
		Pool.last_pointer >>= 1;

	if (stop_needed)
		Pool.mainWindow->PostMessage(TRANSPORT_STOP);

	for (int i=0; i<PLAY_HOOKS; i++){
		if (Pool.BufferHook[i]){
			(Pool.BufferHook[i])(buf, float_size, Pool.BufferCookie[i]);
		}
	}
	

	for (i=0; i<float_size; i+=2) { 
		left = MAX(buf[i], left);
		right = MAX(buf[i+1],right);
	}
	cookie->left = left;
	cookie->right = right;

	// update the visuals
	cookie->count -= size;
	if (cookie->count <0){
		cookie->count = (int)Pool.system_frequency/3;						// 24 times a second
			Pool.mainWindow->PostMessage(UPDATE);
	}
}

#else
void BufferPlayer(void *theCookie, void *buffer, size_t size, const media_raw_audio_format &format)
{
	// We're going to be cheap and only work for floating-point audio 
	if (format.format != media_raw_audio_format::B_AUDIO_FLOAT) { 
		return; 
	}
	
// assumes 44.1Khz stereo floats

	bool stop_needed = false;
	size_t i; 
	float *buf = (float *) buffer; 
	size_t float_size = size/4; 
	cookie_record *cookie = (cookie_record *) theCookie;
	float left = 0.0, right = 0.0;
	double fraq = fabs(Pool.frequency/Pool.system_frequency);
	
	if ((Pool.selection == NONE) | cookie->end){
		cookie->end_mem = Pool.sample_memory + Pool.size*Pool.sample_type;
	}else{
		cookie->end_mem = Pool.sample_memory + Pool.r_sel_pointer*Pool.sample_type;
	}
	
	// Now fill the buffer with sound! 

	if (cookie->pause){
		for (i=0; i<float_size; i++) { 
			buf[i] = 0.0;
		}
	}else{
		if (Pool.sample_type == MONO){	//cookie->mono){
			for (i=0; i<float_size; i+=2){
				if (cookie->mem >= cookie->end_mem){
					if (cookie->loop){
						cookie->mem = Pool.sample_memory + Pool.pointer*Pool.sample_type;
					}else{
						buf[i] = 0.0;
						buf[i+1] = 0.0;
						stop_needed = true;
					}
				}
				if (!stop_needed){
					buf[i] = *cookie->mem;
					buf[i+1] = *cookie->mem;

					cookie->add += fraq;
					if (Pool.frequency>=0){
						while (cookie->add >= 1.0){
							cookie->mem++;
							--cookie->add;
						}
					}else{
						while (cookie->add >= 1.0){
							cookie->mem--;
							--cookie->add;
						}
						if (cookie->mem < Pool.sample_memory)
							cookie->mem += (cookie->end_mem - Pool.sample_memory);
					}
				}
			}
		}else{
			for (i=0; i<float_size; i+=2) { 
				if (cookie->mem >= cookie->end_mem){
					if (cookie->loop){
						cookie->mem = Pool.sample_memory + Pool.pointer*Pool.sample_type;
					}else{
						buf[i] = 0.0;
						buf[i+1] = 0.0;
						stop_needed = true;
					}
				}
				if (!stop_needed){
					switch(Pool.selection){
					case NONE:
					case BOTH:
						buf[i] = cookie->mem[0];
						buf[i+1] = cookie->mem[1];
						break;
					case LEFT:
						buf[i] = cookie->mem[0];
						buf[i+1] = cookie->mem[0];
						break;
					case RIGHT:
						buf[i] = cookie->mem[1];
						buf[i+1] = cookie->mem[1];
						break;
					}

					cookie->add += fraq;
					if (Pool.frequency>=0){
						while (cookie->add >= 1.0){
							cookie->mem+= 2;
							--cookie->add;
						}
					}else{
						while (cookie->add >= 1.0){
							cookie->mem-= 2;
							--cookie->add;
						}
						if (cookie->mem < Pool.sample_memory)
							cookie->mem += (cookie->end_mem - Pool.sample_memory);
					}
					
				}
			}
		}
	}

	Pool.last_pointer = (cookie->mem - Pool.sample_memory);	// set the last played location
	if (Pool.sample_type == STEREO)
		Pool.last_pointer >>= 1;

	if (stop_needed)
		Pool.mainWindow->PostMessage(TRANSPORT_STOP);

	for (int i=0; i<PLAY_HOOKS; i++){
		if (Pool.BufferHook[i]){
			(Pool.BufferHook[i])(buf, float_size, Pool.BufferCookie[i]);
		}
	}
	

	for (i=0; i<float_size; i+=2) { 
		left = MAX(buf[i], left);
		right = MAX(buf[i+1],right);
	}
	cookie->left = left;
	cookie->right = right;

	// update the visuals
	cookie->count -= size;
	if (cookie->count <0){
		cookie->count = (int)Pool.system_frequency/3;						// 24 times a second
			Pool.mainWindow->PostMessage(UPDATE);
	}
}
#endif
