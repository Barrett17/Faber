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


class TrackView : public BView
{
public:
							TrackView(const char* name,
								uint32 resizingMode);

	virtual 				~TrackView();

			void			CurrentSelection(int64* start,int64* end) const;
			void			SelectAt(int64 start,int64 end);

			int64			SelectionStart() const;
			int64			SelectionEnd() const;

			int64			TrackStart() const;
			int64			TrackEnd() const;

			void			SelectAll();
			void			Unselect();


	virtual const BString&	Name() const;
	virtual void 			SetName(const char* name);

	virtual float			Volume() const;
	virtual void			SetVolume(float volume);

	virtual void	 		MuteTrack(bool mute);
	virtual bool 			IsMute() const;

	virtual void			SetSolo(bool solo);
	virtual bool			IsSolo() const;

	virtual	void			Invalidate() = 0;
	virtual void			Pulse() = 0;
private:
			bool			fMuted;
			bool			fSolo;
			BString			fName;
			float			fVolume;

			int64			fStart;
			int64			fEnd;

			int64			fStartSelection;
			int64			fEndSelection;
};


typedef BObjectList<TrackView> TrackViewList;

#endif
