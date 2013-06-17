/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef _AUDIO_TRACK_H
#define _AUDIO_TRACK_H

#include <String.h>

#include "Track.h"


class AudioTrack : public Track {
public:
	virtual const BString&	Name() const;
	virtual void 			SetName(const char* name);

	virtual float			Volume() const;
	virtual void			SetVolume(float volume);

	virtual void			SetBalance(float balance);
	virtual float			Balance() const;

	virtual void			SetRecording(bool solo);
	virtual bool			IsRecording() const;

	virtual void	 		MuteTrack(bool mute);
	virtual bool 			IsMute() const;

	virtual void			SetSolo(bool solo);
	virtual bool			IsSolo() const;

			void			SetFramerate(float framerate);
			int32			CountChannels() const;

private:
			bool			fMuted;
			bool			fSolo;
			bool			fRecording;
			BString			fName;
			float			fVolume;
			float			fBalance;
};

#endif
