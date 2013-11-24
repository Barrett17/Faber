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

#include "SampleView.h"
#include "Track.h"


class TrackView : public BGroupView
{
public:
							TrackView(const char* name,
								Track* track, uint32 resizingMode);

	virtual 				~TrackView();

			bool			IsSelected() const;

			/*
			void			CurrentSelection(int64* start,int64* end) const;
			void			SelectAt(int64 start,int64 end);
			*/

			void			SelectAll();
			void			Unselect();

			int64			Start() const;
			int64			End() const;

			void			SetPointer(int64 pointer);

			int64			Pointer() const;
			int64			SelectionPointer() const;

			bigtime_t		Duration() const;

	virtual const BString&	Name() const;
	virtual void 			SetName(const char* name);

	virtual float			Volume() const;
	virtual void			SetVolume(float volume);

	virtual void			SetBalance(float balance);
	virtual float			Balance() const;

	virtual bool			IsPlaying() const;
	virtual void			SetPlaying(bool playing);

	virtual void			SetRecording(bool solo);
	virtual bool			IsRecording() const;

	virtual void	 		MuteTrack(bool mute);
	virtual bool 			IsMute() const;

	virtual void			SetSolo(bool solo);
	virtual bool			IsSolo() const;

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
			int64			fStart;
			int64			fEnd;
			int64			fSelectionPointer;
};


typedef BObjectList<TrackView> TrackViewList;

#endif
