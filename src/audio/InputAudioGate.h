/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */

class InputAudioGate {
public:

	status_t Start();
	status_t Stop();

	void SetFilterHook(void* hook ...);

	void RegisterWatchHandler(BHandler* handler);

	/*


	*/
private:
	FaberSoundPlayer* fSoundPlayer;
};
