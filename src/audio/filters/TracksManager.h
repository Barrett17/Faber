/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef _TRACKS_MANAGER_H
#define _TRACKS_MANAGER_H

#include "AudioTrack.h"


class TracksManager {
public:

	// The loneNode argument will allow to create separate outputs
	// for tracks
	void RegisterTrack(AudioTrack* track);
	void UnregisterTrack(AudioTrack* track);

	

private:
	AudioTrackList fTrackList;
};

#endif
