/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef _AUDIO_OUTPUT_GATE_H
#define _AUDIO_OUTPUT_GATE_H

#include <MediaDefs.h>
#include <ObjectList.h>

#include "MediaGate.h"
#include "SoundPlayer.h"
#include "TracksManager.h"


class FilterHook {
public:
	void*	hook(float* size_t, void*);
	void*	cookie;
};

class OutputAudioGate : public MediaGate {
public:
									OutputAudioGate(
										TracksManager* fTracksManager);

			virtual					~OutputAudioGate();

			status_t 				Start();
			status_t				StartFrom();
			status_t 				Stop();

			void					SetLoop(bool loop);
			bool					Loop() const;

			void 					SetFilterHook(FilterHook* hook);
			void 					RemoveFilterHook(FilterHook* hook);

			//void RegisterWatchHandler(BHandler* handler);

			void					SetFramerate(float framerate);

			void 					SetFormat(media_format format);
			media_format 			Format() const;

private:
			void 					_Init();
			BSoundPlayer* 			fSoundPlayer;
			media_format			fFormat;

			BObjectList<FilterHook> fHooksList;
			TracksManager*			fTrackManager;
			bool					fLoop;
};

extern void Worker(void* cookie, void* buffer,
	size_t size, const media_raw_audio_format& format);

#endif
