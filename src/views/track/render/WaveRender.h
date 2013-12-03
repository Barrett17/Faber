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

#include "AudioTrack.h"
#include <View.h>

#include "WavePeak.h"


class WaveRender : public BView
{
public:
							WaveRender(AudioTrack* track);	
			 				~WaveRender();

	virtual void			Draw(BRect rect);
	virtual void			MouseDown(BPoint point);
	virtual void			MouseUp(BPoint point);

	virtual void			MouseMoved(BPoint point, uint32 ,
								const BMessage* message);

	virtual void			FrameResized(float width, float height);

	virtual	void			MakeFocus(bool focused = true);

			void			UpdateRequested(BRect rect);

			void			ScrollBy(int64 value);

			bool			IsSelected();
			void			CurrentSelection(int64* start, int64* end);
			void			Select(int64 start, int64 end);
			void			SelectAll();
			void			Unselect();

			int64			Pointer();

			void			ZoomIn();
			void			ZoomOut();
			void			ZoomFull();
			void			ZoomSelection();

private:
			void			_RenderChannel(float* buffer, float center);

			bool			fIsSelected;

			WavePeak*		fWavePeak;
			AudioTrack*		fTrack;
			bool			fUpdate;

			int64			fPointer;
			int64			fStart;
			int64			fEnd;
			int64			fSelectionLeft;
			int64			fSelectionRight;

			friend class	AudioTrackView;
};

