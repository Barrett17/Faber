/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef _AUDIO_TRACK_H
#define _AUDIO_TRACK_H

#include <String.h>

#include "Track.h"

class CPeakFile;

class AudioTrack : public Track {
public:
							AudioTrack();

	virtual bool			IsAudio() const;

			void			SetFramerate(float framerate);
			int32			CountChannels() const;

			bool			IsMono() const;
			bool			IsStereo() const;
			
			void			SetArea(float* area, int64 size);
			int64			Size() const;
			float*			Area() const;
			void			InitPeakFile();
			CPeakFile*		PeakFile() const;

private:

			float*			fArea;
			int64			fSize;
			CPeakFile*		fPeak;
};

#endif
