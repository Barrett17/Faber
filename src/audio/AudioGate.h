/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef _AUDIO_GATE_H
#define _AUDIO_GATE_H

#include <MediaDefs.h>
#include <ObjectList.h>

#include "MediaGate.h"


class FilterHook {
public:
	void	(*hook)(float*, size_t, void*);
	void*	cookie;
};

class AudioGate : public MediaGate {
public:
									AudioGate(
										/*TracksManager* fTracksManager*/);

			virtual					~AudioGate();

			static AudioGate*	Get();

			status_t				Init();
			status_t				InitNode();
			

			status_t 				Start();
			status_t				StartFrom(int64 position);
			status_t 				Stop();
			bool					IsStarted() const;

			void					SetLoop(bool loop);
			bool					Loop() const;

			void					SetPause(bool pause);
			bool					IsPaused() const;

			void 					SetFilterHook(FilterHook* hook);
			void 					RemoveFilterHook(FilterHook* hook);


			//void RegisterWatchHandler(BHandler* handler);

			void					SetFramerate(float framerate);

			void 					SetFormat(media_format format);
			media_format 			Format() const;

private:
			static AudioGate* fInstance;
			media_format			fFormat;

			BObjectList<FilterHook> fHooksList;
			bool					fLoop;
};

#endif
