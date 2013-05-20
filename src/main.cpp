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
#include "main.h"

#include <AppKit.h>
#include <InterfaceKit.h>
#include <MediaKit.h>
#include <StorageKit.h>
#include <View.h>

#include "Globals.h"
#include "FaberWindow.h"
#include "OpenPanel.h"
#include "SavePanel.h"
#include "PeakFile.h"
#include "VMSystem.h"

#include <stdio.h>
#include <stdlib.h>

extern cookie_record play_cookie;


int main()
{
	FaberApp	*myApp;
	myApp = new FaberApp;

	myApp->Run();

	delete (myApp);
	return (0);
}


FaberApp::FaberApp():BApplication(FABER_MIMETYPE)
{
	BRect rect(50, 50, WINDOW_DEFAULT_SIZE_X, WINDOW_DEFAULT_SIZE_Y);
	fFaberWindow = new FaberWindow(rect);
	fOpenPanel = new OpenPanel(this);
	fSavePanel = new SavePanel(this);
	Pool.UpdateMenu();
	fFaberWindow->Show();
}


bool
FaberApp::QuitRequested()
{
	if (fFaberWindow) {
		if (fFaberWindow->Lock() && fFaberWindow->QuitRequested()) {
			fFaberWindow->Quit();

			if (fOpenPanel)
				delete fOpenPanel;

			if (fSavePanel)
				delete fSavePanel;

			return true;
		}
	}
	return true;
}


void
FaberApp::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
	case SAVE_AUDIO:
		if (Pool.save_selection && Pool.selection != NONE)
		{
			save_start = Pool.pointer;		// save selection
			save_end = Pool.r_sel_pointer;
		}
		else
		{
			save_start = 0;				// save the whole memory
			save_end = Pool.size;
		}
		Save(message);
		break;
		
	case B_SIMPLE_DATA:
	case B_MIME_DATA:
		if (Pool.size == 0) // drop on empty is load
			RefsReceived(message);
		else
		{
			app_info info;
			GetAppInfo(&info);
			be_roster->Launch(info.signature, message);
		}
		break;

	case UPDATE_MENU:
		Pool.UpdateMenu();
		break;

	case DROP_PASTE:
	case B_PASTE:
		fFaberWindow->PostMessage(message);
		break;

	default:
		BApplication::MessageReceived(message);
		break;	
	}
}


void
FaberApp::RefsReceived(BMessage* message)
{
//	be_app->Lock();

	uint32			ref_num;
	entry_ref		ref;
	BMediaTrack 	*audTrack(NULL);
	media_format	format;
	memset(&format, 0, sizeof(format));
	//media_raw_audio_format *raf(NULL);
	//short			audioFrameSize(1);
	//char			*audioData(NULL);
	//int32			frame_size;
	int32			channels = 1;
	
	Pool.sample_type = NONE;		// for frame moving / resize
	bool temp_pause = play_cookie.pause;;

	ref_num=0;
	if (message->FindRef("refs",ref_num, &ref) == B_OK)
	{
		BMediaFile		inFile(&ref);
		if (inFile.InitCheck() == B_OK)
		{
			char s[B_FILE_NAME_LENGTH +20];
			sprintf(s, "Faber - %s", ref.name);
			fFaberWindow->SetTitle(s);

			Pool.sample_view_dirty = true;	// update the sample-view
			Pool.update_index = true;
			fFaberWindow->RedrawWindow();
			play_cookie.pause = true;

			// gather the necessary format information
			int32 tracks = inFile.CountTracks();
			for (int32 i = 0; i < tracks; i++) {
				BMediaTrack *inTrack = inFile.TrackAt(i);
				inTrack->EncodedFormat(&format);

				if (format.IsAudio()) 
				{
					audTrack = inTrack;
					inTrack->DecodedFormat(&format);

//					Pool.m_format = format;
					memcpy(&Pool.m_format, &format, sizeof(Pool.m_format));

					Pool.sample_bits = (format.u.raw_audio.format & 0xf)*8;
					Pool.selection = NONE;
					Pool.frequency = format.u.raw_audio.frame_rate;

//					printf("format : %x\n", format.u.raw_audio.format);

					Pool.size = audTrack->CountFrames()-1;
					channels = format.u.raw_audio.channel_count;

					Pool.StartProgress(B_TRANSLATE("Loading file..."), (int32) Pool.size);
					
					//frame_size = (format.u.raw_audio.format & 0xf) * channels;

#ifndef __VM_SYSTEM	//RAM
					if (Pool.sample_memory)						// create buffer for sample memory, add an extra frame to be able to do
						free(Pool.sample_memory);				//  32bit to 16 bit conversions
					
					Pool.sample_memory = (float*) malloc((size_t) (Pool.size * channels * 4 + 1024));
#endif					
				}
				else
				{
					inFile.ReleaseAllTracks();
				}
			}

			int64 frameCount, framesRead;
			status_t err;
			media_header mh;
			int32 lastPercent, currPercent;
			float completePercent;
			BString status;
			char *buffer = (char*)malloc(format.u.raw_audio.buffer_size);		// temp memory
#ifndef __VM_SYSTEM	//RAM
			float *mem = Pool.sample_memory;									// dest memory
			// read audio from source and write to destination, if necessary
			if (mem) 
			{
#else
			VM.Reset();

			float *convert_buffer = (float*)malloc(format.u.raw_audio.buffer_size*4);		// make sure there can be floats in it
			// read audio from source and write to destination, if necessary
			if (convert_buffer) 
			{
				float *mem = NULL;
#endif			
				frameCount = audTrack->CountFrames();
				int64 count =0;
				lastPercent = -1;
				for (int64 i = 0; i < frameCount; i += framesRead) 
				{
					#ifdef __VM_SYSTEM	//RAM
					mem = convert_buffer;
					#endif
					
					// clear buffer first
					memset( buffer, 0, format.u.raw_audio.buffer_size);
					if ((err = audTrack->ReadFrames(buffer, &framesRead, &mh)) != B_OK) 
					{
						printf("Error reading audio frames: %s\n", strerror(err));
						break;
					}

					count += framesRead;			// now correct for crashes if bigger than file
					if (count > frameCount)
						framesRead -= (count - frameCount);
		
					switch(format.u.raw_audio.format)
					{
					case 0x24:	// 0 == mid, -1.0 == bottom, 1.0 == top (the preferred format for non-game audio)
					{	
						float *tmp = (float*) buffer;
						float x;
						for (int32 count = 0; count<framesRead*channels; count++)
						{
							x = *tmp++;
							if (x<-1.0)
								x = -1.0;
							else if (x>1.0)	
								x = 1.0;
							*mem++ = x;
						}
					}	break;
					case 0x4:	// 0 == mid, 0x80000001 == bottom, 0x7fffffff == top (all >16-bit formats, left-adjusted)
					{	
						int32 *tmp = (int32*)buffer;
						float x;
						for (int32 count = 0; count<framesRead*channels; count++)
						{
							x = ((float) *tmp++) / ((float) 0x80000000);
							if (x<-1.0)
								x = -1.0;
							else if (x>1.0)	
								x = 1.0;
							*mem++ = x;
						}
					}	break;
					case 0x2:	// 0 == mid, -32767 == bottom, +32767 == top
					{	
						int16 *tmp = (int16*)buffer;
						float x;
						for (int32 count = 0; count<framesRead*channels; count++)
						{
							x = ((float) *tmp++) / ((float) 32767.0);
							if (x<-1.0)
								x = -1.0;
							else if (x>1.0)	
								x = 1.0;
							*mem++ = x;
						}
					}	break;
					case 0x11:	// 128 == mid, 1 == bottom, 255 == top (discouraged but supported format)
					{	
						uint8 *tmp = (uint8*)buffer;
						float x;
						for (int32 count = 0; count<framesRead*channels; count++)
						{
							x = ((float) *tmp++) / ((float) 127.0) - ((float) 1.0);
							if (x<-1.0)
								x = -1.0;
							else if (x>1.0)	
								x = 1.0;
							*mem++ = x;
						}
					}	break;
					case 0x1:		// 0 == mid, -127 == bottom, +127 == top (not officially supported format)
					{	
						int8 *tmp = (int8*)buffer;
						float x;
						for (int32 count = 0; count<framesRead*channels; count++)
						{
							x = ((float) *tmp++) / ((float) 127.0);		// xor 128 to invert sign bit
							if (x<-1.0)
								x = -1.0;
							else if (x>1.0)	
								x = 1.0;
							*mem++ = x;
						}
					}	break;
					}
					
					#ifdef __VM_SYSTEM	//RAM
					VM.WriteBlock( convert_buffer, framesRead*channels );
					#endif

					Pool.ProgressUpdate( (int32) framesRead );

					completePercent = ((float)i) / ((float)frameCount) * ((float) 100);
					currPercent = (int16)floor(completePercent);
					if (currPercent > lastPercent) 
						lastPercent = currPercent;
				}
				inFile.ReleaseAllTracks();
				#ifdef __VM_SYSTEM	//RAM
				free(convert_buffer);
				#endif
			}
			else
			{
				Pool.play_mode = NONE;
				Pool.pointer = 0;
				Pool.play_pointer = 0;
				Pool.l_pointer = 0;
				Pool.r_pointer = 0;
				Pool.r_sel_pointer = 0;
				Pool.size = 0;
				Pool.selection = NONE;
				Pool.sample_type = NONE;
				Pool.sample_bits = 16;
				Pool.frequency = 41400.0;

		        (new BAlert(NULL,B_TRANSLATE("Not enough memory, no Undo possible!"),B_TRANSLATE("OK")))->Go();
			}
			
			if (channels == 1)
				Pool.sample_type = MONO;
			else
				Pool.sample_type = STEREO;

			Pool.r_pointer = Pool.size;
			Pool.pointer = 0;
			Pool.r_sel_pointer = Pool.pointer;
			Pool.l_pointer = 0;

#ifndef __VM_SYSTEM	//RAM
			play_cookie.mem = Pool.sample_memory;
			play_cookie.start_mem = Pool.sample_memory;
			play_cookie.end_mem = Pool.sample_memory + Pool.size*Pool.sample_type;
			play_cookie.frequency = Pool.frequency;
			play_cookie.add = 0;
#else			
			play_cookie.mem = 0;
			play_cookie.start_mem = 0;
//			play_cookie.end_mem = Pool.size*Pool.sample_type;
			play_cookie.frequency = Pool.frequency;
			play_cookie.add = 0;
#endif
			Pool.changed = false;
			Pool.HideProgress();

			// create the PeakFile
			Pool.ResetIndexView();
			Hist.Reset();				// reset undo class

			if (IsLaunching() && Prefs.play_when_loaded)
				Pool.mainWindow->PostMessage(TRANSPORT_PLAY);
			
		}
		else
		{
			(new BAlert(NULL,B_TRANSLATE("This file is not supported!"),B_TRANSLATE("OK")))->Go();
		}
	}
	
	Pool.sample_view_dirty = true;	// update the sample-view
	Pool.update_draw_cache = true;	// update the draw cache
	Pool.update_index = true;		// update the draw cache
	Pool.update_peak = true;
	fFaberWindow->RedrawWindow();
	Pool.InitBufferPlayer( Pool.frequency );

	play_cookie.pause = temp_pause;
	Pool.UpdateMenu();
	fFaberWindow->UpdateRecent();
//	be_app->Unlock();
}

//------------------------------------------------------------------ Save

void
FaberApp::Save(BMessage *message){
	// Grab the stuff we know is there .. or should be :P

	entry_ref dir_ref, file_ref;
	const char *name;
	BFile newFile;
	BDirectory dir;
	float t;
	
	if ((message->FindRef("directory", &dir_ref) == B_OK)
		&& (message->FindString("name", &name) == B_OK))
	{
		dir.SetTo(&dir_ref);
		if (dir.InitCheck() != B_OK)
			return;
			
		dir.CreateFile(name, &newFile);
		
		BEntry entry(&dir, name);
		if (entry.InitCheck() != B_OK) 
		{
			(new BAlert(NULL, B_TRANSLATE("Can't overwrite file."), B_TRANSLATE("OK")))->Go();
			return;
		}
		entry.GetRef(&file_ref);

		media_codec_info *audioCodec;
		media_file_format *fileFormat;
		media_raw_audio_format *raf(NULL), *raf_in(NULL);
		media_format format;
		memset(&format, 0, sizeof(format));
		char *buffer(NULL);
		int32 frame_size(1);

		fSavePanel->GetSelectedFormatInfo(&fileFormat, &audioCodec);

		if (audioCodec != NULL)
		{
			//format = Pool.m_format;
			memcpy(&format, &Pool.m_format, sizeof(format));
			raf_in = &(format.u.raw_audio);
			format.type = B_MEDIA_RAW_AUDIO;

			if (raf_in->format == 1)	
				raf_in->format = 0x11;
			
			// create media file
			BMediaFile file(&file_ref, fileFormat, B_MEDIA_FILE_REPLACE_MODE);
			if (file.InitCheck() != B_OK)
			{
				(new BAlert(NULL, B_TRANSLATE("Can't overwrite file."), B_TRANSLATE("OK")))->Go();
				return;
			}
			
			BMediaTrack *outTrack = file.CreateTrack(&format, audioCodec);

			if (outTrack){
				file.CommitHeader();

				if (save_start == 0){			// save as
					char s[B_FILE_NAME_LENGTH +20];
					sprintf(s, "Faber - %s", file_ref.name);
					fFaberWindow->SetTitle(s);
				}

				raf = &(format.u.raw_audio);
				buffer = (char*)malloc(raf->buffer_size);
				int32 channels = raf->channel_count;
				frame_size = (raf->format & 0xf) * raf->channel_count;
				
				int32 buffer_step = raf->buffer_size / frame_size;
#ifndef __VM_SYSTEM	//RAM
				float *mem = Pool.sample_memory + save_start*Pool.sample_type;	// src memory
#else
				float *convert_buffer = (float*)malloc(buffer_step*channels*4);		// make sure there can be floats in it
				// read audio from source and write to destination, if necessary
				if (convert_buffer) {
					VM.ReadBlockAt(save_start, convert_buffer, buffer_step*channels );
					float *mem = convert_buffer;
#endif			

				Pool.StartProgress(B_TRANSLATE("Saving file..."), (int32) (save_end-save_start));
				
				for (int64 i=save_start; i<save_end; i+=buffer_step)
				{
					// fill up the buffer

					int32 block = MIN( (int32) (save_end-i) , buffer_step);
					switch(format.u.raw_audio.format){
					case 0x24:	// 0 == mid, -1.0 == bottom, 1.0 == top (the preferred format for non-game audio)
					{	
						float *tmp = (float*)buffer;
						for (int32 count = 0; count<block*channels; count++){
							*tmp++ = *mem++;
						}
					}	break;
					case 0x4:	// 0 == mid, 0x80000001 == bottom, 0x7fffffff == top (all >16-bit formats, left-adjusted)
					{	
						int32 *tmp = (int32*) buffer;
						for (int32 count = 0; count<block*channels; count++)
						{
							t = *mem++;
							*tmp++ = (int32) ROUND(t * (float) 0x7fffffff);
						}
					}	break;
					case 0x2:	// 0 == mid, -32767 == bottom, +32767 == top
					{	
						int16 *tmp = (int16*) buffer;
						for (int32 count = 0; count<block*channels; count++)
						{
							t = *mem++;
							*tmp++ = (int16) ROUND(t * 32767.0);
						}
					}	break;
					case 0x11:	// 128 == mid, 1 == bottom, 255 == top (discouraged but supported format)
					{	
						uint8 *tmp = (uint8*)buffer;
						for (int32 count = 0; count<block*channels; count++)
						{
							t = *mem++;
							*tmp = (uint8) ROUND(t*127.0);
							tmp++;
							*tmp = *tmp ^ 0x80;
						}
					}	break;
					case 0x1:		// 0 == mid, -127 == bottom, +127 == top (not officially supported format)
					{	
						int8 *tmp = (int8*)buffer;
						for (int32 count = 0; count<block*channels; count++)
						{
							t = *mem++;
							*tmp++ = (int8) ROUND(t * 127.0);		// xor 128 to invert sign bit
						}
					}	break;
					}

					Pool.ProgressUpdate( block );
					outTrack->WriteFrames(buffer, block);
					#ifdef __VM_SYSTEM	//RAM
					VM.ReadBlock(convert_buffer, block*channels );
					mem = convert_buffer;
					#endif
				}

				#ifdef __VM_SYSTEM	//RAM
				free(convert_buffer);
				}
				#endif
				
				Pool.changed = false;

				outTrack->Flush();

				BMimeType result;
				BEntry ent(&dir,name);
				entry_ref fref;
				ent.GetRef(&fref);
				BMimeType::GuessMimeType(&fref,&result);
				BNodeInfo ninfo(&newFile); 
				ninfo.SetType(result.Type()); 

			}
			else
			{
				(new BAlert(NULL, B_TRANSLATE("The selected codec does not support your file settings."), B_TRANSLATE("OK")))->Go();
			}

			file.CloseFile();
			
			free(buffer);
			Pool.HideProgress();
		}
	}
	else
	{
		(new BAlert(NULL, B_TRANSLATE("This project has changed. Do you want to save it now?"), B_TRANSLATE("OK")))->Go();
	}

	if (Pool.save_mode == 2)
		PostMessage(B_QUIT_REQUESTED);
	if (Pool.save_mode == 1)
		fFaberWindow->PostMessage(OPEN);

	Pool.save_mode = 0;
}
