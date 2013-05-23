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

#ifndef FABER_WINDOW
#define FABER_WINDOW

#include <Window.h>

#include "ToolBar.h"
#include "TrackAudio.h"
#include "InfoToolBar.h"
#include "TimeBarView.h"

// TODO remove them
// for some reason if you
// remove them there's a compilation error
class BMenu;
class BMenuBar;
class ToolBar;
class TimeBarView;
class TrackAudio;
class InfoToolBar;


class FaberWindow : public BWindow
{
public:
							FaberWindow(BRect frame);
	virtual					~FaberWindow();
	virtual bool 			QuitRequested();
	virtual void 			MessageReceived(BMessage *message);
	
			void 			UpdateRecent();
			void			RedrawWindow();
			void			UpdateToolBar();
			// activate/deactivate menu items
			void			UpdateMenu();

			// checks to see if a file is changed after load
			bool			IsChanged(int32 mode=1); 
private:
			BMenuBar*		_BuildMenu();

			BMenuBar*		fMainMenuBar;
			BMenu*			fRecentMenu;

			ToolBar*		fToolBar;
			TrackAudio*		fTrackView;
			InfoToolBar*	fInfoToolBar;
			TimeBarView*	fTimeBar;

	friend class SampleView;


	// TODO polish them
	BMenu		*fEditMenu, *menu_transform, *menu_zero, *menu_analyze, *menu_generate;
	BMenuItem	*fSaveMenu, *fSaveAsMenu, *mn_save_sel, *mn_undo, *mn_cut, *mn_copy;
	BMenuItem	*mn_paste, *mn_select_all, *mn_trim, *mn_set_freq, *mn_resample;
	BMenuItem	*mn_clear, *mn_unselect, *mn_copy_silence, *mn_paste_new;

	BMenuItem	*mn_paste_mix, *mn_copy_to_stack, *mn_redo;
};

#endif
