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
#include "Filters.h"

#include <Path.h>
#include <File.h>

#include "FaberDefs.h"

#include "SimpleEffects.h"

#include "Globals.h"
#include "PeakFile.h"
#include "ResampleWindow.h"
#include "WindowsManager.h"

// the filters with GUI
#include "RoomFilter.h"
#include "ReverbFilter.h"
#include "DelayFilter.h"
#include "BassBoostFilter.h"
#include "AmplifierFilter.h"
#include "CompressorFilter.h"
#include "NormalizeFilter.h"

#include <stdio.h>

#define FILTER_BLOCK	2048

int32 __Last_Filter = 0;
int32 __Last_FilterTmp = 0;
int32 __FilterCount = 0;		// nr of open filters

filter_info __FilterList[] = { 	
	{"Silence", FILTER_BOTH, 1},
	{"---", FILTER_BOTH | FILTER_REALTIME | FILTER_GUI, 1},
	{"Fade In", FILTER_BOTH, 1},
	{"Fade Out", FILTER_BOTH, 1},
	{"Normalize", FILTER_BOTH | FILTER_GUI, 2},
	{"---", FILTER_BOTH | FILTER_REALTIME | FILTER_GUI, 1},
	{"Bass Boost", FILTER_BOTH | FILTER_REALTIME | FILTER_GUI, 1},
	{"Compressor", FILTER_BOTH | FILTER_REALTIME | FILTER_GUI, 1},
	{"Amplifier", FILTER_BOTH | FILTER_REALTIME | FILTER_GUI, 1},
	{"---", FILTER_BOTH | FILTER_REALTIME | FILTER_GUI, 1},
	{"Delay", FILTER_BOTH | FILTER_REALTIME | FILTER_GUI, 1},
	{"Reverb", FILTER_BOTH | FILTER_REALTIME | FILTER_GUI, 1},
	{"Room", FILTER_BOTH | FILTER_REALTIME | FILTER_GUI, 1},
	{"---", FILTER_BOTH | FILTER_REALTIME | FILTER_GUI, 1},
	{"Invert", FILTER_BOTH, 1},
	{"Swap", FILTER_STEREO, 1},
{NULL,0, 0} };


// reinstate the last filter using the tag from the prefs
void FiltersInit()
{
	int32 filter = 0;
	while(__FilterList[filter].name != NULL)
	{
		if (strcmp(__FilterList[filter].name, Prefs.repeat_tag.String()) == 0){
			__Last_Filter = filter;
			__Last_FilterTmp = filter;
			return;
		}
		filter++;
	}
}


// Create the filter object and return it's pointer
RealtimeFilter *CreateFilter(int32 filter)
{
	RealtimeFilter *pFilter = NULL;
	bool bRealtime = __FilterList[filter].type & FILTER_REALTIME;
	
	if (strcmp(__FilterList[filter].name, "Room") == 0)
		pFilter = new RoomWindow(bRealtime);

	else if (strcmp(__FilterList[filter].name, "Delay") == 0)
		pFilter = new DelayWindow(bRealtime);

	else if (strcmp(__FilterList[filter].name, "Reverb") == 0)
		pFilter = new ReverbWindow(bRealtime);

	else if (strcmp(__FilterList[filter].name, "Bass Boost") == 0)
		pFilter = new BassBoostFilter(bRealtime);

	else if (strcmp(__FilterList[filter].name, "Invert") == 0)
		pFilter = new InvertFilter();

	else if (strcmp(__FilterList[filter].name, "Swap") == 0)
		pFilter = new SwapFilter();

	else if (strcmp(__FilterList[filter].name, "Amplifier") == 0)
		pFilter = new AmplifierFilter(bRealtime);

	else if (strcmp(__FilterList[filter].name, "Compressor") == 0)
		pFilter = new CompressorFilter(bRealtime);

	else if (strcmp(__FilterList[filter].name, "Silence") == 0)
		pFilter = new SilenceFilter();

	else if (strcmp(__FilterList[filter].name, "Fade In") == 0)
		pFilter = new FadeInFilter();

	else if (strcmp(__FilterList[filter].name, "Fade Out") == 0)
		pFilter = new FadeOutFilter();

	else if (strcmp(__FilterList[filter].name, "Normalize") == 0)
		pFilter = new NormalizeFilter();

	// set the passes the filter needs
	if (pFilter)
		pFilter->SetPasses( __FilterList[filter].passes );

	return pFilter;
}

// here we need to run the filters. For the lite version it will just use
// the build in ones, while the other will use plug-ins
void RunFilter(const char *tag)
{
	int32 filter = 0;
	while(__FilterList[filter].name != NULL)
	{
		if (strcmp(__FilterList[filter].name, tag) == 0){
			RunFilter(filter);
			return;
		}
		filter++;
	}
}

void RunFilter(int32 filter)
{
	/*if (__FilterCount == PLAY_HOOKS/2)	return;

	__Last_FilterTmp = filter;
	RealtimeFilter *pFilter = CreateFilter(filter);

	if (pFilter)
	{
		__FilterCount++;		// another filter open

		// add the GUI
		BView *view = pFilter->ConfigView();
		if (view){
			// run with GUI
			
			// resize the window
			pFilter->ResizeTo( view->Bounds().Width(), view->Bounds().Height() + 40);
			// add the config view
			if (view->Bounds().Width()<FILTER_MIN_WIDTH)
				view->MoveBy( (FILTER_MIN_WIDTH - view->Bounds().Width())/2, 0);
			pFilter->ChildAt(0)->AddChild(view);

			BRect frame = WindowsManager::MainWindow()->Frame();
			pFilter->MoveTo(frame.left +__FilterCount * 40 +40, frame.top + 80 + __FilterCount * 20);
			
			pFilter->Run();			// start looper
			//pFilter->InitFilter(Pool.system_frequency, 2);	// initiase
			pFilter->Start();		// start media buffers
			pFilter->Show();		// show to user
		}else{
			// run without GUI
			ExecuteFilter(pFilter);
		}
	}*/
}

void WriteBack(float *src, float *dest, int32 size)
{
	/*if (Pool.selection == BOTH || Pool.sample_type == MONO)
		memcpy( dest, src, size*4 );
	else
	if (Pool.selection == LEFT)		//allways stereo 
	{
		for (int32 x = 0; x < size; x+=2)
			dest[x] = src[x];
	}
	else
	{
		src++;
		dest++;
		for (int32 x = 0; x < size; x+=2)
			dest[x] = src[x];
	}*/
}

void ExecuteFilter(RealtimeFilter *pFilter)
{
	if (!pFilter)	return;

	__FilterCount--;		// another filter closed
	
	pFilter->Lock();
	if (pFilter->IsHidden()){
		// run without GUI
	}
	else{
		pFilter->Hide();
		pFilter->Stop();
		pFilter->DeAllocate();		// remove buffers used for realtime effect
	}

	/*if (Pool.PrepareFilter()){
		// init with track data
		
		float *filter_buffer = new float[FILTER_BLOCK];
		
		int32 size = (Pool.r_sel_pointer - Pool.pointer +1)*Pool.sample_type;
		WindowsManager::Get()->StartProgress(B_TRANSLATE("Working..."), size * pFilter->Passes());

		for (int32 filter_pass = 0; filter_pass < pFilter->Passes(); filter_pass++)
		{
			int32 size = (Pool.r_sel_pointer - Pool.pointer +1)*Pool.sample_type;
			float *p = Pool.sample_memory + Pool.pointer*Pool.sample_type;
			pFilter->InitFilter(Pool.system_frequency, Pool.sample_type, filter_pass, size);

			while(size >= FILTER_BLOCK){
				memcpy( filter_buffer, p, FILTER_BLOCK*4 );	// get the data
				pFilter->FilterBuffer( filter_buffer, FILTER_BLOCK);
				WriteBack( filter_buffer, p, FILTER_BLOCK);
				p+=FILTER_BLOCK;
				size-=FILTER_BLOCK;
				WindowsManager::Get()->ProgressUpdate( FILTER_BLOCK );
			}
			if (size)
			{
				memcpy( filter_buffer, p, size*4 );	// get the data
				pFilter->FilterBuffer( filter_buffer, size);
				WriteBack( filter_buffer, p, size);
			}
		}

		WindowsManager::Get()->HideProgress();
		Pool.changed = true;
		
		delete filter_buffer;
	}

	// enable repeat action
	__Last_Filter = __Last_FilterTmp;
	Prefs.repeat_tag.SetTo(__FilterList[__Last_Filter].name);

	pFilter->DeAllocate();
	pFilter->Quit();

	WindowsManager::MainWindow()->ResetIndexView();
	WindowsManager::MainWindow()->UpdateMenu();
	WindowsManager::MainWindow()->RedrawWindow();*/
}

void CancelFilter(RealtimeFilter *pFilter)
{
	if (!pFilter)	return;

	__FilterCount--;		// another filter closed
	pFilter->Stop();
	pFilter->DeAllocate();
	pFilter->Lock();
	pFilter->Quit();
}

void RunLastFilter()
{
	RealtimeFilter *pFilter = CreateFilter(__Last_Filter);
	if (pFilter)
	{
		ExecuteFilter(pFilter);
	}
}

