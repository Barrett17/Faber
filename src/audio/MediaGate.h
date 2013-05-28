/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef _MEDIA_GATE_H
#define _MEDIA_GATE_H

class MediaGate {
public:

	virtual status_t Start();
	virtual status_t Stop();

	bool	IsStarted() const;
	// used to set a filter before to
	// pass the audio to the node
	//virtual void SetFilterHook() = 0;

	// Register a handler to obtain
	// data for analyzer windows
	//virtual void RegisterWatchHandler(BHandler* handler) = 0;

	/*

	*/
private:
	bool fStarted;
};

#endif
