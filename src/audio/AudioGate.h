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
#include "CommandListener.h"
#include "MediaEndPoint.h"
#include "MediaGate.h"


class AudioGate : public MediaGate {
public:
									AudioGate();
			virtual					~AudioGate();

	virtual status_t 				Start();
	virtual status_t 				Stop();

	virtual status_t				ConnectProducer(AudioTrack* track,
										const media_node& node, 
										const media_output* output = NULL, 
										const media_format* format = NULL);

	virtual status_t				ConnectConsumer(
										const media_node& node, 
										const media_input* input = NULL, 
										const media_format* format = NULL);

			media_node				Node();
protected:
			friend class			AudioEngine;

			const MediaEndPointMap&	GetInputs();
			const MediaEndPointMap&	GetOutputs();

private:

			static AudioGate*		fInstance;
			media_format			fFormat;

			AudioEngine*			fAudioEngine;

			// Tracks Management
			MediaEndPointMap*		fInputs;
			MediaEndPointMap*		fOutputs;
};

#endif
