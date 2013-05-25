/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */

class AudioGate {
public:

	virtual status_t Start();
	virtual status_t Stop();

	bool	IsStarted() const;
	// used to set a filter before to
	// pass the audio to the node
	virtual void SetFilterHook(void* hook ...) = 0;

	// Register a handler to obtain
	// data for analyzer windows
	virtual void RegisterWatchHandler(BHandler* handler) = 0;

	// The loneNode argument will allow to create separate outputs
	// for tracks
	virtual void RegisterTrack(Track* track, bool loneNode = false);
	/*

	*/
private:
	bool fStarted;
	TrackList fTrackList;
};
