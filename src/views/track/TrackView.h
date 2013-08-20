/*
 * Copyright 2013 Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Dario Casalinuovo
 */
#ifndef TRACK_VIEW__H
#define TRACK_VIEW__H

#include <ObjectList.h>
#include <View.h>

#include "Globals.h"
#include "SampleView.h"
#include "Track.h"


class TrackView : public BView
{
public:
							TrackView(const char* name,
								Track* track, uint32 resizingMode);

	virtual 				~TrackView();

			bool			IsSelected() const { };
			bool			RightSelected() const;
			bool			LeftSelected() const;

			void			CurrentSelection(int64* start,int64* end) const;
			void			SelectAt(int64 start,int64 end);

			int64			SelectionStart() const;
			int64			SelectionEnd() const;



			void			SelectAll();
			void			Unselect();

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

private:
			Track*			fTrack;

			int64			fStart;
			int64			fEnd;

			int64			fStartSelection;
			int64			fEndSelection;
};


typedef BObjectList<TrackView> TrackViewList;

#endif
