/*
    Copyright 2013 Dario Casalinuovo. All rights reserved.

    This file is part of Faber.

    Faber is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Faber is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Faber.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _MEDIA_AGENT_H
#define _MEDIA_AGENT_H

#include <ObjectList.h>

#include "AudioGate.h"


class MediaAgent {
public:
	static AudioGate*				GetAudioGate();

	static status_t					RegisterTrack(Track* track);
	static status_t					UnregisterTrack(Track* track);


	static BMessage*				ArchiveTracks();
	static status_t					UnarchiveTracks(BMessage* from);

			// TODO implement a pulse interface for the UI widgets
			// void RegisterPulseListener(PulseListener* listener,
			// 		int32 flags, int32 trackID = -1);

protected:
	static AudioGate*				fAudioGate;
	static BObjectList<Track>		fTracks;
	static uint32					fLastID;
};

#endif
