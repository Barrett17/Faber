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
#include "SoundPlayer.h"

#include "WindowsManager.h"
#include "CommonPool.h"

//#include "MyClipBoard.h"
#include "FaberWindow.h"

#include <stdio.h>
#include <stdlib.h>

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
	player = NULL;
	
	m_playing = false;
	changed = false;

	last_pointer = 0;
	sample_view_dirty = true;	// update the sample-view
	update_draw_cache = true;	// update the draw cache
	update_peak = false;
	
	play_cookie.buffer = NULL;

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
	pointer = 0;
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
	if (IsPlaying())
		StopPlaying();

	if (play_cookie.buffer)
		delete [] play_cookie.buffer;

	if (sample_memory)
		free(sample_memory);

	if (player)
		delete player;
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


void CommonPool::StopPlaying()
{
	m_playing = false;

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
	WindowsManager::MainWinMessenger()->SendMessage(TRANSPORT_STOP);		// stop playing

	if (selection == NONE)
		 WindowsManager::MainWindow()->SelectAll();	// select all if noe is selected
	//ClipBoard.SaveUndo();			// save undo data

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
		if (format.channel_count == 1){	//cookie->mono){
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
		WindowsManager::MainWinMessenger()->SendMessage(TRANSPORT_STOP);

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
		WindowsManager::MainWinMessenger()->SendMessage(UPDATE);
	}
}
