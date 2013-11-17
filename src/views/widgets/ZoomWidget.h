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

#ifndef ZOOM_WIDGET
#define ZOOM_WIDGET

#include <GroupView.h>

#include "FaberWidget.h"
#include "IconButton.h"


class ZoomWidget : public FaberWidget
{
public:
							ZoomWidget();
	virtual 				~ZoomWidget();

			// Various methods for controlling
			// the toolbar behavior
			void			SetPlay(bool play);
			bool			IsPlay() const;

			void			SetStop(bool stop);
			bool			IsStop() const;

			void			SetPause(bool pause);
			bool			IsPause() const;

			// Unimplemented right now
			//void			SetRec(bool rec);
			//bool			IsRec() const;

			void			SetLoop(bool loop);
			bool			IsLoop() const;

private:

			IconButton*		fPlayButton;
			IconButton*		fStopButton;
			IconButton*		fLoopButton;

			bool			fPlaying;

};

#endif
