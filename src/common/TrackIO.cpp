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

#include "TrackIO.h"

#include <Alert.h>
#include <Entry.h>
#include <MediaDefs.h>
#include <MediaFile.h>
#include <MediaTrack.h>
#include <String.h>

#include "AudioTrack.h"
#include "FaberDefs.h"
#include "FaberMath.h"
#include "Globals.h"
#include "WindowsManager.h"

#include <stdlib.h>
#include <string.h>

TrackIO* TrackIO::fInstance = NULL;


TrackIO*
TrackIO::Get()
{
	if (fInstance == NULL)
		fInstance = new TrackIO();

	return fInstance;	
}


Track*
TrackIO::Load(BMessage* message)
{
	AudioTrack* track = new AudioTrack();

	uint32			ref_num = 0;
	entry_ref		ref;
	BMediaTrack 	*audTrack(NULL);
	media_format	format;
	memset(&format, 0, sizeof(format));
	int32			channels = 1;

	if (message->FindRef("refs",ref_num, &ref) == B_OK) {
		BMediaFile inFile(&ref);

		if (inFile.InitCheck() == B_OK) {

			track->SetName(ref.name);

			// gather the necessary format information
			int32 tracks = inFile.CountTracks();
			for (int32 i = 0; i < tracks; i++) {
				BMediaTrack *inTrack = inFile.TrackAt(i);
				inTrack->EncodedFormat(&format);

				if (format.IsAudio()) {
					audTrack = inTrack;
					inTrack->DecodedFormat(&format);

					track->SetFormat(format);

					printf("format : %x\n", format.u.raw_audio.format);

					int64 size = audTrack->CountFrames()-1;

					WindowsManager::Get()->StartProgress(
						B_TRANSLATE("Loading file..."), (int32) size);
					
					//frame_size = (format.u.raw_audio.format & 0xf) * channels;
					
					float* area = (float*) malloc((size_t) (size * channels * 4 + 1024));
					track->SetArea(area, size);		

				} else {
					inFile.ReleaseAllTracks();
				}
			}

			int64 frameCount, framesRead;
			status_t err;
			media_header mh;
			int32 lastPercent, currPercent;
			float completePercent;
			BString status;
			char* buffer = (char*)malloc(format.u.raw_audio.buffer_size);		// temp memory

			float* mem = track->Area();

			// read audio from source and write to destination, if necessary
			if (mem) {
				frameCount = audTrack->CountFrames();
				int64 count =0;
				lastPercent = -1;
				for (int64 i = 0; i < frameCount; i += framesRead) {
					
					// clear buffer first
					memset( buffer, 0, format.u.raw_audio.buffer_size);
					if ((err = audTrack->ReadFrames(buffer, &framesRead, &mh)) != B_OK) {
						printf("Error reading audio frames: %s\n", strerror(err));
						break;
					}

					// now correct for crashes if bigger than file
					count += framesRead;			
					if (count > frameCount)
						framesRead -= (count - frameCount);

					switch(format.u.raw_audio.format)
					{
						// 0 == mid, -1.0 == bottom, 1.0 == top (the preferred format for non-game audio)
						case media_raw_audio_format::B_AUDIO_FLOAT:	
						{	
							float *tmp = (float*) buffer;
							float x;
							for (int32 count = 0; count<framesRead*channels; count++) {
								x = *tmp++;
								if (x<-1.0)
									x = -1.0;
								else if (x>1.0)	
									x = 1.0;
								*mem++ = x;
							}
						}
						break;
						
						// 0 == mid, 0x80000001 == bottom, 0x7fffffff == top (all >16-bit formats, left-adjusted)
						case media_raw_audio_format::B_AUDIO_INT:	
						{	
							int32 *tmp = (int32*)buffer;
							float x;
							for (int32 count = 0; count<framesRead*channels; count++) {
								x = ((float) *tmp++) / ((float) 0x80000000);
								if (x<-1.0)
									x = -1.0;
								else if (x>1.0)	
									x = 1.0;
								*mem++ = x;
							}
						}
						break;

						// 0 == mid, -32767 == bottom, +32767 == top
						case media_raw_audio_format::B_AUDIO_SHORT:
						{	
							int16 *tmp = (int16*)buffer;
							float x;
							for (int32 count = 0; count<framesRead*channels; count++) {
								x = ((float) *tmp++) / ((float) 32767.0);
								if (x<-1.0)
									x = -1.0;
								else if (x>1.0)	
									x = 1.0;
								*mem++ = x;
							}
						}
						break;

						// 128 == mid, 1 == bottom, 255 == top (discouraged but supported format)
						case media_raw_audio_format::B_AUDIO_UCHAR:	
						{	
							uint8 *tmp = (uint8*)buffer;
							float x;
							for (int32 count = 0; count<framesRead*channels; count++) {
								x = ((float) *tmp++) / ((float) 127.0) - ((float) 1.0);
								if (x<-1.0)
									x = -1.0;
								else if (x>1.0)	
									x = 1.0;
								*mem++ = x;
							}
						}
						break;

						// 0 == mid, -127 == bottom, +127 == top (not officially supported format)
						case media_raw_audio_format::B_AUDIO_CHAR:
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
						}
						break;

					}

					WindowsManager::Get()->ProgressUpdate((int32) framesRead);

					completePercent = ((float)i) / ((float)frameCount) * ((float) 100);
					currPercent = (int16)floor(completePercent);
					if (currPercent > lastPercent) 
						lastPercent = currPercent;
				}
				inFile.ReleaseAllTracks();
			} else {
		        (new BAlert(NULL,B_TRANSLATE("Not enough memory, no Undo possible!"),B_TRANSLATE("OK")))->Go();
			}

			track->InitPeakFile();
			
			// reset undo class
			//Hist.Reset();				

			//if (IsLaunching() && Prefs.play_when_loaded)
			//	fFaberWindow->PostMessage(TRANSPORT_PLAY);

			WindowsManager::Get()->HideProgress();
		} else {
			(new BAlert(NULL,B_TRANSLATE("This file is not supported!"),B_TRANSLATE("OK")))->Go();
			return NULL;
		}
	}

	return track;
}


Track*
TrackIO::Save(BMessage *message)
{
	// Grab the stuff we know is there .. or should be :P

	entry_ref dir_ref, file_ref;
	const char *name;
	BFile newFile;
	BDirectory dir;
	float t;
	bool saveMode;

	if (message->FindBool("SaveMode", &saveMode) == B_OK)
		fSaveMode = saveMode;

	if ((message->FindRef("directory", &dir_ref) == B_OK)
		&& (message->FindString("name", &name) == B_OK)) {
		dir.SetTo(&dir_ref);
		if (dir.InitCheck() != B_OK)
			return NULL;
			
		dir.CreateFile(name, &newFile);
		
		BEntry entry(&dir, name);
		status_t ret = entry.InitCheck();
		if (ret != B_OK) {
			BString error(B_TRANSLATE("Error:"));
			error << strerror(ret);

			(new BAlert(NULL, error.String(), B_TRANSLATE("OK")))->Go();

			return NULL;
		}
		entry.GetRef(&file_ref);

		media_codec_info *audioCodec;
		media_file_format *fileFormat;
		media_raw_audio_format *raf(NULL), *raf_in(NULL);
		media_format format;
		memset(&format, 0, sizeof(format));
		char *buffer(NULL);
		int32 frame_size(1);

		WindowsManager::GetSavePanel()->GetSelectedFormatInfo(
			&fileFormat, &audioCodec);

		if (audioCodec != NULL) {
			//format = Pool.m_format;

			//memcpy(&format, &Pool.m_format, sizeof(format));
			raf_in = &(format.u.raw_audio);
			format.type = B_MEDIA_RAW_AUDIO;

			if (raf_in->format == 1)	
				raf_in->format = 0x11;

			// create media file
			BMediaFile file(&file_ref, fileFormat, B_MEDIA_FILE_REPLACE_MODE);
			if (file.InitCheck() != B_OK) {
				(new BAlert(NULL, B_TRANSLATE("Can't overwrite file."), B_TRANSLATE("OK")))->Go();
				return NULL;
			}

			BMediaTrack *outTrack = file.CreateTrack(&format, audioCodec);

			if (outTrack) {
				file.CommitHeader();

				/*if (save_start == 0) {
					// save as
					char s[B_FILE_NAME_LENGTH +20];
					sprintf(s, "Faber - %s", file_ref.name);
					fFaberWindow->SetTitle(s);
				}*/

				raf = &(format.u.raw_audio);
				buffer = (char*)malloc(raf->buffer_size);
				int32 channels = raf->channel_count;
				frame_size = (raf->format & 0xf) * raf->channel_count;
				
				int32 buffer_step = raf->buffer_size / frame_size;

				float *mem /*= Pool.sample_memory + save_start*Pool.sample_type*/;	// src memory

				/*WindowsManager::Get()->StartProgress(
					B_TRANSLATE("Saving file..."), (int32) (save_end-save_start));

				for (int64 i=save_start; i<save_end; i+=buffer_step) {
					// fill up the buffer

					int32 block = MIN( (int32) (save_end-i) , buffer_step);
					switch(format.u.raw_audio.format)
					{

						case media_raw_audio_format::B_AUDIO_FLOAT:	// 0 == mid, -1.0 == bottom, 1.0 == top (the preferred format for non-game audio)
						{	
							float *tmp = (float*)buffer;
							for (int32 count = 0; count<block*channels; count++) {
								*tmp++ = *mem++;
							}
							break;
						}	
	
						case media_raw_audio_format::B_AUDIO_INT:	// 0 == mid, 0x80000001 == bottom, 0x7fffffff == top (all >16-bit formats, left-adjusted)
						{	
							int32 *tmp = (int32*) buffer;
							for (int32 count = 0; count<block*channels; count++) {
								t = *mem++;
								*tmp++ = (int32) ROUND(t * (float) 0x7fffffff);
							}
							break;
						}
	
						case media_raw_audio_format::B_AUDIO_SHORT:	// 0 == mid, -32767 == bottom, +32767 == top
						{	
							int16 *tmp = (int16*) buffer;
							for (int32 count = 0; count<block*channels; count++) {
								t = *mem++;
								*tmp++ = (int16) ROUND(t * 32767.0);
							}
							break;
						}	
	
						case media_raw_audio_format::B_AUDIO_UCHAR:	// 128 == mid, 1 == bottom, 255 == top (discouraged but supported format)
						{	
							uint8 *tmp = (uint8*)buffer;
							for (int32 count = 0; count<block*channels; count++) {
								t = *mem++;
								*tmp = (uint8) ROUND(t*127.0);
								tmp++;
								*tmp = *tmp ^ 0x80;
							}
							break;
						}
	
						case media_raw_audio_format::B_AUDIO_CHAR:		// 0 == mid, -127 == bottom, +127 == top (not officially supported format)
						{	
							int8 *tmp = (int8*)buffer;
							for (int32 count = 0; count<block*channels; count++) {
								t = *mem++;
								*tmp++ = (int8) ROUND(t * 127.0);		// xor 128 to invert sign bit
							}
							break;
						}
	
					}

					WindowsManager::Get()->ProgressUpdate(block);
					outTrack->WriteFrames(buffer, block);
				}*/

				//Pool.changed = false;

				outTrack->Flush();

				BMimeType result;
				BEntry ent(&dir,name);
				entry_ref fref;
				ent.GetRef(&fref);
				BMimeType::GuessMimeType(&fref,&result);
				BNodeInfo ninfo(&newFile); 
				ninfo.SetType(result.Type()); 

			} else {
				(new BAlert(NULL, B_TRANSLATE("The selected codec does not support your file settings."), B_TRANSLATE("OK")))->Go();
			}

			file.CloseFile();
			
			free(buffer);
			WindowsManager::Get()->HideProgress();
		}

	} else {
		(new BAlert(NULL, B_TRANSLATE("This project has changed. Do you want to save it now?"), B_TRANSLATE("OK")))->Go();
	}

	/*if (fSaveMode == 2)
		PostMessage(B_QUIT_REQUESTED);

	if (fSaveMode == 1)
		fFaberWindow->PostMessage(OPEN);*/

	fSaveMode = 0;
}
