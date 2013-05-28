/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef _AUDIO_INPUT_GATE_H
#define _AUDIO_INPUT_GATE_H

#include "MediaGate.h"


class InputAudioGate : public MediaGate {
public:

	status_t Start();
	status_t Stop();

	void SetFilterHook(void* hook ...);



	/*
	void RegisterWatchHandler(BHandler* handler);

	*/
private:

};

#endif
