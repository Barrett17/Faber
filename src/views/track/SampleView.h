/*
   	Copyright (c) 2003, Xentronix
	Author: Frans van Nispen (frans@xentronix.com)
	All rights reserved.
	
	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:
	
	Redistributions of source code must retain the above copyright notice, this list
	of conditions and the following disclaimer. Redistributions in binary form must
	reproduce the above copyright notice, this list of conditions and the following
	disclaimer in the documentation and/or other materials provided with the distribution. 
	
	Neither the name of Xentronix nor the names of its contributors may be used
	to endorse or promote products derived from this software without specific prior
	written permission. 
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
	SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _SAMPLE_VIEW
#define _SAMPLE_VIEW

#include <View.h>
#include <Bitmap.h>

#include "AudioTrack.h"

class AudioTrackView;

// peak: from peakfile
// plain: lines, editable
// points: draggable points
enum {
	DRAW_PEAK,
	DRAW_PLAIN,
	DRAW_POINTS
};


class SampleView : public BView
{
public:
							SampleView(AudioTrackView* track);
	virtual 				~SampleView();

	virtual void			Draw(BRect);
	virtual void			MouseDown(BPoint);
	virtual void			MouseUp(BPoint);
	virtual void			MouseMoved(BPoint, uint32, const BMessage* );
	virtual void			Pulse();
	virtual void			FrameResized(float width, float height);

			void			ZoomIn();
			void			ZoomOut();
			void			ZoomFull();
			void			ZoomSelection();

			void			UpdateScroll(float newValue,
								float max, float min);

			bool			IsSelected() const;

			void			EditPoint(BPoint p);
			void			EditLine(BPoint, BPoint);
			void			DoDraw(int64 ptr, int32 add, float v);

			void			Init();

private:
 			void 			CalculateCache();
 			void 			DrawMono(BRect, bool left, bool draw_selection);
 			void 			DrawStereo(BRect);

			void 			DrawPart(rgb_color* inBits, rgb_color* outBits,
								rgb_color col, float* peak_buffer,
								BRect r, int32 size, int32 size2);

			int32			_CalculateStep();
			int32			_CalculateProportion();

 			BBitmap*		fOffScreen;

 			BBitmap*		fLeftCache;
 			BBitmap*		fRightCache;
 			BBitmap*		fLeftSelected;
 			BBitmap*		fRightSelected;

			BPoint			fOld;
			BPoint			fStartSelection;

 			rgb_color*		fLeftBits;
			rgb_color*		fLeftSelectedBits;
			rgb_color*		fRightBits;
			rgb_color*		fRightSelectedBits;
 			rgb_color*		fScreenBits;

			int32			fLeftWidth;
			int32			fRightWidth;
			int32			fScreenWidth;
			int32 			fEditChannel;

			int64			fOldLeftPointer;
			int64			fOldRightPointer;

			int64			t;
			int64			t2;

			float			fOldX;
			float			fWidth;

			float*			fLeftPeakBuffer;
			float*			fRightPeakBuffer;

			bool			drag;
			bool			edit;
			bool			stop_following;
			bool			draw_selection;
			bool			drag_border;
			bool			drag_selection;
		 	bool			fResized;
		 	bool			cache_left_valid;
		 	bool			cache_right_valid;

			sem_id			fViewSem;

			AudioTrackView* fOwner;
			AudioTrack*		fTrack;
			bool			fUpdatePeak;

			bool			fSelected;

			float			fOldScroll;
};

#endif
