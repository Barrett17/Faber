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

#ifndef _WAVE_RENDER
#define _WAVE_RENDER

#include <View.h>

#include "AudioTrack.h"
#include "TracksCoordinator.h"


class AudioTrackView;


class WaveRender : public BView {
public:
								WaveRender(AudioTrackView* owner,
									TracksCoordinator* coordinator);	

	virtual void				Draw(BRect rect);
	virtual void				MouseDown(BPoint point);
	virtual void				MouseUp(BPoint point);

	virtual void				MouseMoved(BPoint point, uint32 ,
									const BMessage* message);

	virtual	void				MakeFocus(bool focused = true);

			AudioTrackView*		GetOwner() const;
private:
			void				_RenderChannel(float* buffer, size_t size,
									float center);
			void				_RenderTrack(BRect rect);
			void				_RenderPointers(BRect rect);

			AudioTrackView*		fOwner;
			AudioTrack*			fTrack;
			TracksCoordinator*	fCoordinator;
};

#endif
