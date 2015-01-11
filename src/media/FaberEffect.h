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
#ifndef _FABER_EFFECT_H_
#define _FABER_EFFECT_H_


#include <MenuItem.h>
#include <ObjectList.h>
#include <ParameterWeb.h>

#include "AudioTrack.h"


// Effects kind flags
const uint32 FABER_AUDIO_EFFECT		= 0x80000000UL;
const uint32 FABER_BUILTIN_EFFECT	= 0x40000000UL;
const uint32 FABER_CONSUMER			= 0x20000000UL;
const uint32 FABER_FILTER			= 0x10000000UL;
const uint32 FABER_GUI_EFFECT		= 0x08000000UL;
const uint32 FABER_PARAMWEB_EFFECT	= 0x00100000UL;
const uint32 FABER_MULTIPASS_EFFECT = 0x04000000UL;
const uint32 FABER_PRODUCER			= 0x02000000UL;	
const uint32 FABER_REALTIME_EFFECT	= 0x01000000UL;

#define CONTROL_CHANGED	'chgd'


class FaberEffect {
public:
							FaberEffect(const char* name, uint32 flags);
	virtual 				~FaberEffect();

	const char*				Name() const;

	// Build the config view of this effect.
	// Return null if there is no GUI.
	// NOTE: it's planned to build this GUI
	// using the BParameterWeb.
	virtual BView*			SettingsPanel() = 0;

			BMenuItem*		BuildItem();

 
			BParameterWeb*	ParameterWeb();

			int32			Flags() const;

	// This is called by EffectsManager to filter a track.
	// An effect can reject if some unsupported track is passed.
	virtual status_t		FilterTrack(Track* track,
								int64 start, int64 end) = 0;

	// Save the current settings into message.
	virtual status_t		ArchiveSettings(BMessage* message) = 0;

	// Update effect's settings stored in message.
	virtual status_t		UpdateSettings(BMessage* message) = 0;

private:
			uint32			fFlags;
			const char*		fName;
};

#endif	// _FABER_EFFECT_H_
