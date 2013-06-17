/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef _TRACK_H
#define _TRACK_H

#include <MediaDefs.h>


class BString;

class Track {
public:
	virtual const BString&	Name() const = 0;
	virtual void 			SetName(const char* name) = 0;

	virtual float			Volume() const = 0;
	virtual void			SetVolume(float volume) = 0;

	virtual void			SetBalance(float balance) = 0;
	virtual float			Balance() const = 0;

	virtual void			SetRecording(bool solo) = 0;
	virtual bool			IsRecording() const = 0;

	virtual void	 		MuteTrack(bool mute) = 0;
	virtual bool 			IsMute() const = 0;

	virtual void			SetSolo(bool solo) = 0;
	virtual bool			IsSolo() const = 0;

			void 			SetFormat(media_format format);
			media_format 	Format() const;

			media_format	fFormat;
};

#endif
