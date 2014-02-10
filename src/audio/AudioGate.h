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

#ifndef _AUDIO_GATE_H
#define _AUDIO_GATE_H

#include <MediaDefs.h>
#include <MediaRoster.h>
#include <ObjectList.h>

#include "AudioEngine.h"
#include "AudioTrack.h"
#include "MediaEndPoint.h"
#include "MediaGate.h"


class AudioGate : public MediaGate {
public:
									AudioGate();
			virtual					~AudioGate();

			static AudioGate*		Get();

	virtual status_t 				Start();
	virtual status_t 				Stop();

			// Tracks Management

			static status_t			RegisterTrack(Track* track);
			static status_t			UnregisterTrack(Track* track);

			static status_t			RegisterTrack(AudioTrack* track);
			static status_t			UnregisterTrack(AudioTrack* track);

protected:
			friend class			AudioEngine;

			const MediaEndPointMap&	GetInputs();

			const MediaEndPointMap&	GetOutputs();

private:

			static AudioGate*		fInstance;
			media_format			fFormat;

			BMediaRoster*			fRoster;

			AudioEngine*			fAudioEngine;

			MediaEndPointMap*		fInputs;
			MediaEndPointMap*		fOutputs;

			AudioTrackList			fTracks;
			uint32					fLastID;

};

#endif
