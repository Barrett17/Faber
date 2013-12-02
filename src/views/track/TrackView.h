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

#ifndef TRACK_VIEW__H
#define TRACK_VIEW__H

#include <ObjectList.h>
#include <GroupView.h>

#include "Track.h"


class TrackView : public BGroupView
{
public:
							TrackView(const char* name,
								Track* track, uint32 resizingMode);

	virtual 				~TrackView();

			uint32			ID() const;

	virtual bool			IsSelected() const = 0;

	virtual Track*			GetTrack() const;

	virtual void			SelectAll() = 0;
	virtual void			Unselect() = 0;

	virtual void			GetSelection(int64 start, int64 end) const = 0;
	virtual void			Select(int64 start, int64 end) = 0;

	virtual void			ScrollBy(int64 value) = 0;

	virtual void			SetPointer(int64 pointer) = 0;
	virtual int64			Pointer() const = 0;

			bigtime_t		Duration() const;
			int64			Frames() const;

			const BString&	Name() const;
			void 			SetName(const char* name);

			float			Volume() const;
			void			SetVolume(float volume);

			void			SetBalance(float balance);
			float			Balance() const;

			bool			IsPlaying() const;
			void			SetPlaying(bool playing);

			void			SetRecording(bool solo);
			bool			IsRecording() const;

			void	 		MuteTrack(bool mute);
			bool 			IsMute() const;

			void			SetSolo(bool solo);
			bool			IsSolo() const;

	virtual void			ZoomIn() = 0;
	virtual void			ZoomOut() = 0;
	virtual void			ZoomFull() = 0;
	virtual void			ZoomSelection() = 0;

	virtual bool			HasChanged() const;
	virtual bool			HasUndo() const;
	virtual bool			HasRedo() const;

protected:
			Track*			fTrack;

			int64			fPointer;
			int64			fTrackStart;
			int64			fTrackEnd;
			int64			fSelectionPointer;
};


typedef BObjectList<TrackView> TrackViewList;

#endif
