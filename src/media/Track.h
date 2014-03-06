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

#ifndef _TRACK_H
#define _TRACK_H

#include <MediaDefs.h>
#include <String.h>


class Track : public BArchivable {
public:
									Track();
									Track(BMessage* from);

	virtual 						~Track();

	virtual	status_t 				Archive(BMessage* into,
										bool deep = true) const;

	// Note BArchivable::Instantiate is leaved for derived classes
	// because the Track is pure virtual.

	virtual	bigtime_t				Duration() const;

	virtual bool					IsAudio() const = 0;
			//bool					IsMIDI() const = 0;

	virtual const BString&			Name() const;
	virtual void 					SetName(const char* name);

	virtual float					Volume() const;
	virtual void					SetVolume(float volume);

	virtual void					SetBalance(float balance);
	virtual float					Balance() const;

	virtual bool					IsPlaying() const;
	virtual void					SetPlaying(bool playing);

	virtual void					SetRecording(bool solo);
	virtual bool					IsRecording() const;

	virtual void	 				MuteTrack(bool mute);
	virtual bool 					IsMute() const;

	virtual void					SetSolo(bool solo);
	virtual bool					IsSolo() const;

	virtual bool					HasChanged() const;
	virtual bool					HasUndo() const;
	virtual bool					HasRedo() const;

	virtual uint32					ID();

protected:
			friend class 			AudioGate;

			bigtime_t				fDuration;

	virtual void					SetID(uint32 id);

private:
			BString					fName;

			bool					fMuted;
			bool					fSolo;
			bool					fRecording;
			bool					fPlaying;

			bool					fHasChanged;
			bool					fHasUndo;
			bool					fHasRedo;

			float					fVolume;
			float					fBalance;

			uint32					fID;
};

#endif
