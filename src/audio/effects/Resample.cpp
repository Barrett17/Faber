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

#include "SimpleEffects.h"

#include <Path.h>

#include "FaberDefs.h"

#include <stdlib.h>

#define BUFFER_SIZE		128*1024
#define BLOCK_SIZE		BUFFER_SIZE*4

void DoResample()
{
	//if (Pool.sample_type == NONE)	return;

	float *buffer = (float*)malloc(BUFFER_SIZE*4+4);
	if (!buffer)	return;	// error
	
	//Pool.player->Stop();
	WindowsManager::MainWinMessenger()->SendMessage(FABER_TRANSPORT_STOP); // stop playing

	//Hist.Save(H_FULL, 0, Pool.size);	// full undo

	app_info info;
	be_app->GetAppInfo(&info);
	BString str("/tmp/BeAETmp");
	str << info.thread;						// make the file unique
	BPath path;
	path.SetTo(str.String());
   
	BFile *file = new BFile(path.Path(), B_READ_WRITE|B_CREATE_FILE|B_ERASE_FILE);//|B_FAIL_IF_EXISTS
	if(file->InitCheck() != B_OK){
		// errror
		return;
	}
	BEntry e(path.Path());

	/*float left = 0.0, right = 0.0, add = 0.0;
	float fraq = Pool.frequency/Prefs.filter_resample_freq;		// the convert frequency
	float *mem = Pool.sample_memory;
	float *end = Pool.sample_memory + Pool.size*Pool.sample_type;
	
	int32 count = 1000;
	int32 bit_convert = (1 << Prefs.filter_resample_bits)-1;

	Pool.size = (int64)ceil(Pool.size * Prefs.filter_resample_freq / Pool.frequency);

	int32 buffer_pointer = 0;

	WindowsManager::Get()->StartProgress(B_TRANSLATE("Resampling..."), Pool.size);
	if (Pool.sample_type == MONO && Prefs.filter_resample_mono){		// mono to mono
		while(mem<= end){
			left = *mem;
			// sample conversion
			left = floor(left * bit_convert)/bit_convert;
			
			buffer[buffer_pointer++] = left;
			if (buffer_pointer == BUFFER_SIZE){
				file->Write((void*)buffer, BUFFER_SIZE*4);
				buffer_pointer = 0;
			}
	
			add += fraq;
			while (add >= 1.0){
				++mem;
				--add;
			}
			if (count-- <0){count = 1000;	WindowsManager::Get()->ProgressUpdate( 500 );	}
		}
		if (buffer_pointer){
			file->Write((void*)buffer, buffer_pointer*4);
		}
	}else if (Pool.sample_type == MONO && !Prefs.filter_resample_mono){			// mono to stereo
		float l_mul = bit_convert*Prefs.filter_resample_sl/100;
		float r_mul = bit_convert*Prefs.filter_resample_sr/100;
		while(mem<= end){
			left = *mem;
			right = *mem;
			
			// sample conversion
			left = floor(left * l_mul)/bit_convert;
			right = floor(right * r_mul)/bit_convert;
			
			buffer[buffer_pointer++] = MIN(left,1);
			buffer[buffer_pointer++] = MIN(right,1);
			if (buffer_pointer == BUFFER_SIZE){
				file->Write((void*)buffer, BUFFER_SIZE*4);
				buffer_pointer = 0;
			}
	
			add += fraq;
			while (add >= 1.0){
				++mem;
				--add;
			}
			if (count-- <0){count = 1000;	WindowsManager::Get()->ProgressUpdate( 500 );	}
		}
		if (buffer_pointer){
			file->Write((void*)buffer, buffer_pointer*4);
		}
	}else if (Pool.sample_type == STEREO && !Prefs.filter_resample_mono){		// stereo to stereo
		while(mem<= end){
			// sample conversion
			left = floor(mem[0] * bit_convert)/bit_convert;
			right = floor(mem[1] * bit_convert)/bit_convert;
			
			buffer[buffer_pointer++] = left;
			buffer[buffer_pointer++] = right;
			if (buffer_pointer == BUFFER_SIZE){
				file->Write((void*)buffer, BUFFER_SIZE*4);
				buffer_pointer = 0;
			}

			add += fraq;
			while (add >= 1.0){
				mem+=2;
				--add;
			}
			if (count-- <0){count = 1000;	WindowsManager::Get()->ProgressUpdate( 500 );	}
		}
		if (buffer_pointer){
			file->Write((void*)buffer, buffer_pointer*4);
		}
	}else if (Pool.sample_type == STEREO && Prefs.filter_resample_mono){		// stereo to mono
		float l_mul = bit_convert*Prefs.filter_resample_ml/100;
		float r_mul = bit_convert*Prefs.filter_resample_mr/100;
		while(mem<= end){
			// sample conversion
			left = floor(mem[0] * l_mul)/bit_convert + floor(mem[1] * r_mul)/bit_convert;
			
			buffer[buffer_pointer++] = MIN(left,1);
			if (buffer_pointer == BUFFER_SIZE){
				file->Write((void*)buffer, BUFFER_SIZE*4);
				buffer_pointer = 0;
			}

			add += fraq;
			while (add >= 1.0){
				mem+=2;
				--add;
			}
			if (count-- <0){count = 1000;	WindowsManager::Get()->ProgressUpdate( 500 );	}
		}
		if (buffer_pointer){
			file->Write((void*)buffer, buffer_pointer*4);
		}
	}

// now reset memory and load the file
	Pool.frequency = Prefs.filter_resample_freq;
	if (Prefs.filter_resample_mono)
		Pool.sample_type = MONO;
	else
		Pool.sample_type = STEREO;

	Pool.sample_bits = Prefs.filter_resample_bits;
	
	Pool.m_format.u.raw_audio.frame_rate = Prefs.filter_resample_freq;
	Pool.m_format.u.raw_audio.channel_count = Pool.sample_type;
	if (Pool.sample_bits <= 8)							Pool.m_format.u.raw_audio.format = 0x11;	// 8bits
	if (Pool.sample_bits <= 16 && Pool.sample_bits >8)	Pool.m_format.u.raw_audio.format = 0x2;		// 16bits
	if (Pool.sample_bits <= 32 && Pool.sample_bits >16)	Pool.m_format.u.raw_audio.format = 0x24;	// 32bits (float)


	// resize mem to frequncy
	Pool.pointer = 0;
	Pool.l_pointer = 0;
	Pool.r_pointer = Pool.size;
	Pool.r_sel_pointer = Pool.size;
	Pool.selection = BOTH;

	free(Pool.sample_memory);
	Pool.sample_memory = (float*)malloc(Pool.size*Pool.sample_type*4 +1024);

	file->Seek(0, SEEK_SET);		// reset file
	end = Pool.sample_memory + Pool.size*Pool.sample_type;
	
	uint32 size = (Pool.size+1)*4*Pool.sample_type;

	char *p = (char*)(Pool.sample_memory);
	while(size>=BLOCK_SIZE){
		file->Read((void*)p, BLOCK_SIZE);
		p+=BLOCK_SIZE;
		size-=BLOCK_SIZE;
		WindowsManager::Get()->ProgressUpdate( BLOCK_SIZE/(4*Pool.sample_type) );
	}
	if (size)
		file->Read((void*)p, size);

	WindowsManager::Get()->HideProgress();

	if(file)	delete file;
	e.Remove();				// remove file

	free(buffer);

	// init the BSoundPlayer to the new frequency
	Pool.InitBufferPlayer( Pool.frequency );

	Pool.changed = true;
	Peak.Init( Pool.size+1, (Pool.sample_type == MONO) );	// Init peakfile
	WindowsManager::MainWindow()->ResetIndexView();
	WindowsManager::MainWindow()->UpdateMenu();
	WindowsManager::MainWindow()->RedrawWindow();*/
}
