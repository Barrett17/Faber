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

#ifndef _POOL_H
#define _POOL_H

#include <Entry.h>
#include <SoundPlayer.h>

#include "FaberWindow.h"

class FaberWindow;
class BMenu;

#define PLAY_HOOKS	64		// number of realtime effects possible

#define UPDATE	'updt'

typedef struct cookie_record
{
	float *mem, *end_mem, *start_mem;
	float *buffer;
	float left, right;
	float frequency, add;
	bool pause;
	bool mono;
	bool loop;
	bool end;
	int count;
} cookie_record; 


class CommonPool
{
 public:
	CommonPool();
	~CommonPool();
	void Init();

	float system_frequency;

	void ResetIndexView();			// Create the IndexZoomView data
	bool update_peak;

	
	void SelectAll();
	void DeSelectAll();

	// TODO Remove, it's not a good place for it.
	void RedrawWindow();
	
	void SaveUndo();			// save Undo data
	void Undo();				// undo

	BSoundPlayer	*player;
	
	BCursor *mouseArrow, *mousePencil, *mouseLeftRight, *mouseMove, *mouseArrowMove, *mouseArrowLeft, *mouseArrowRight;
	
	media_format	m_format;
	bool	save_selection;		// to decide whether to save selection or full

	float	*sample_memory;					// memory 8Mb fixed for demo version

	int32	tool_mode;		// draw or select

	int64	pointer;		// the sample pointer
	int64	last_pointer;
	int64	play_pointer;	// used during playback
	int64	size;			// sample size (in samples)
	int64	l_pointer;		// left start of edit window
	int64	r_pointer;		// right end of edit window
	int64	r_sel_pointer;	// right selection pointer
	int32	selection;		// selection mode: NONE, LEFT, RIGHT, BOTH
	int32	sample_type;	// MONO, STEREO
	int32	sample_bits;	// 8, 16
	float	frequency;
	int32	play_mode;		// to determine the play mode (or record)
	
	void	InitBufferPlayer(float freq);
	void	StartPlaying(int64 p, bool end);	// play back the sound, convert data and
	void	StopPlaying();				// sync VU meters
	bool	IsPlaying();
	bool	SetLoop(bool);				// returns old state
	int32	SetPlayHook(void (*hook)(float*, size_t, void *), int32 min=0, void *cookie=NULL);	// set a hook for a filter
	void	RemovePlayHook(void (*hook)(float*, size_t, void *), int32 min=-1);			// set a hook for a filter
	void	(*BufferHook[PLAY_HOOKS])(float*, size_t, void *cookie);
	void	*BufferCookie[PLAY_HOOKS];
	
	bool	PrepareFilter();		// to save undo-data, stop playing, check selection
	
	void	UpdateMenu();			// activate/deactivate menu items
	bool	changed;
	int32	save_mode;				// 1-do load after save, 2-do quit after save
	bool	IsChanged(int32 mode=1); // checks to see if a file is changed after load

	BView	*m_SampleView;			// pointer to the sample-view to update the pointer

	FaberWindow* mainWindow;

	BMenu		*menu_edit, *menu_transform, *menu_zero, *menu_analyze, *menu_generate;
	BMenuItem	*mn_save, *mn_save_as, *mn_save_sel, *mn_undo_enable, *mn_undo, *mn_cut, *mn_copy;
	BMenuItem	*mn_paste, *mn_select_all, *mn_trim, *mn_set_freq, *mn_resample;
	BMenuItem	*mn_clear, *mn_unselect, *mn_copy_silence, *mn_paste_new;

	BMenuItem	*mn_paste_mix, *mn_copy_to_stack, *mn_redo;

	bool	sample_view_dirty, update_draw_cache;	// these are uses to recalc the sampleview cache and as dirty bit
	bool	update_index;

private:

	bool m_playing;			// is there any playback ?
};

extern CommonPool Pool; // Included so you don't have too 
#endif
