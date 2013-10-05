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
#include "SampleView.h"

#include <Bitmap.h>
#include <MenuItem.h>
#include <PopUpMenu.h>

#include "AudioTrackView.h"
#include "FaberDefs.h"
#include "FaberMath.h"
#include "Globals.h"
#include "PeakFile.h"
#include "BitmapDrawer.h"
#include "Shortcut.h"
//#include "MyClipBoard.h"
#include "WindowsManager.h"
#include "MouseIcons.h"

#include <stdio.h>

#define POINTER_BAR_HEIGHT	3


SampleView::SampleView(AudioTrackView* track)
	:
	BView("", B_FRAME_EVENTS | B_WILL_DRAW),
	fTrack(track->Track()),
	fOwner(track),
	fSelected(false)
{
	SetViewColor(B_TRANSPARENT_COLOR);
	drag = false;
	edit = false;
	fUpdatePeak = true;
	stop_following = false;
	drag_selection = false;
	drag_border = false;
	cache_left_valid = false;
	cache_right_valid = false;
	fOld.Set(-1,-1);
	fOldX = -1;
	
	fLeftPeakBuffer = NULL;
	fRightPeakBuffer = NULL;
	fWidth = -1;

	fOffScreen = NULL;
	fLeftCache = NULL;
	fRightCache = NULL;
	fLeftSelected = NULL;
	fRightSelected = NULL;

	fOldLeftPointer = -1;
	fOldRightPointer = -1;

	// need to create background buffers
	m_resized = false;		

	if ((fViewSem = create_sem(1, "SampleView Sem")) < 0) {
//		debugger(CREATE_SEM_FAIL_MSG);
	}

	Init();
}


SampleView::~SampleView()
{
	if (fLeftPeakBuffer)
		delete fLeftPeakBuffer;

	if (fRightPeakBuffer)
		delete fRightPeakBuffer;

	if (fLeftCache)
		delete fLeftCache;

	if (fRightCache)
		delete fRightCache;

	if (fLeftSelected)
		delete fLeftSelected;

	if (fRightSelected)
		delete fRightSelected;

	if (fOffScreen)
		delete fOffScreen;
}


void
SampleView::Init()
{
	fLeftPeakBuffer = new float[ WINDOW_MAX_SIZE_X * 2 ];
	fRightPeakBuffer = new float[ WINDOW_MAX_SIZE_X * 2 ];

	// wipe buffers
	memset( fLeftPeakBuffer, 0, WINDOW_MAX_SIZE_X * 2 * sizeof(float));
	memset( fRightPeakBuffer, 0, WINDOW_MAX_SIZE_X * 2 * sizeof(float));
}


void
SampleView::Pulse()
{
	/*if (Pool.sample_type == NONE)	return;

	BRect r = Bounds();
	r.top += POINTER_BAR_HEIGHT;

	if (!Pool.IsPlaying() && !play_cookie.pause) {
		if (fOldX != -1) {
			SetDrawingMode(B_OP_INVERT);
			StrokeLine( BPoint( fOldX, r.top), BPoint( fOldX, r.bottom));
			SetDrawingMode(B_OP_COPY);
			fOldX = -1;
		}
		return;
	}
	int64 p = 0;

	if (Pool.sample_type != NONE) {
		if (Pool.IsPlaying()) {
			p = Pool.last_pointer;
		} else {
			p = fOwner->Pointer();
		}
	}	
	if (p!=pointer) {
		pointer = p;
		
		int32 xx = fOwner->End() - fOwner->Start();
		if (Prefs.follow_playing && !stop_following && pointer < fTrack->Size() - xx/2)
		{
			int64 ptr;
			ptr = p - xx/2;
			if ( ptr > (fTrack->Size()-xx))	ptr = fTrack->Size()-xx;
			if ( ptr < 0 )				ptr = 0;

			if ( fOwner->Start() != ptr )
			{
				fOwner->Start() = ptr;
				fOwner->End() = fOwner->Start() + xx;
			}
		}	

		// play pointer
		float x = (pointer-fOwner->Start()) * Bounds().Width() /(fOwner->End() - fOwner->Start());
		SetDrawingMode(B_OP_INVERT);

		StrokeLine( BPoint( fOldX, r.top), BPoint( fOldX, r.bottom));
		fOldX = x;
		StrokeLine( BPoint( x, r.top), BPoint( x, r.bottom));

		SetDrawingMode(B_OP_COPY);
	}*/
}


void
SampleView::Draw(BRect rect)
{
	//printf("SampleView::Draw\n");

	Looper()->Lock();

	if (fOwner->UpdateDrawCache() && !m_resized)
		CalculateCache();

	if (m_resized) {
		//printf("SampleView::Draw fResized == true\n");

		acquire_sem(fViewSem);

		// allocate offscreen bitmap here
		if (fOffScreen)
			delete fOffScreen;

		BRect r = Bounds();
		r.InsetBy(0,-2);
		fOffScreen = new BBitmap(r, B_RGB32);

		fScreenBits = (rgb_color*)fOffScreen->Bits();
		fScreenWidth = fOffScreen->Bounds().IntegerWidth()+1;

		// adjust for the top bar
		fScreenBits += POINTER_BAR_HEIGHT*fScreenWidth;		

		// fill upper with gray bar
		int32 width = Bounds().IntegerWidth();
		BitmapDrawer draw(fOffScreen);
		rgb_color a;
		for (int32 y=0; y<POINTER_BAR_HEIGHT; y++) {
			if (y==0)
				a = (rgb_color) {255,255,255};
			else if (y==POINTER_BAR_HEIGHT-1)
				a = (rgb_color) {128,128,128};
			else
				a = (rgb_color) {192,192,192};

			for (int32 x=0; x<=width; x++)
				draw.PlotBGR(x,y,a);

		}
		
		release_sem(fViewSem);

		// recalculate caches for resized screen
		CalculateCache();
	}

	rect.top = Bounds().top;
	rect.bottom = Bounds().bottom;

	if (fOwner->Dirty() && rect.IsValid()) {

		if (fTrack->IsMono() && cache_left_valid) {

			rect.bottom = Bounds().bottom-POINTER_BAR_HEIGHT;
			DrawMono(rect, true, fSelected);
			rect.bottom = Bounds().bottom;

		} else if (fTrack->IsStereo()) {
			DrawStereo(rect);
		} else {

			BitmapDrawer draw(fOffScreen);

			rgb_color a = Prefs.back_color;
			uint8 t = a.red;
			a.red = a.blue;
			a.blue = t;
				
			int32 bottom = Bounds().IntegerHeight();
			int32 right = Bounds().IntegerWidth();
			// need to fix this !

			for (int32 y=POINTER_BAR_HEIGHT; y<=bottom; y++) {
				for (int32 x=0; x<=right; x++)
					draw.PlotBGR(x,y,a);
			}
		}
	}	

	DrawBitmapAsync(fOffScreen, rect, rect);

	if (fTrack->Size() == 0) {
		Looper()->Unlock();
		return;
	}

	// Draw the pointer
	BRect r = Bounds();
	int32 xx = POINTER_BAR_HEIGHT/2 -2;

	if (!fSelected) {
		float x = (fOwner->Pointer()-fOwner->Start()) * Bounds().Width()
			/ (fOwner->End() - fOwner->Start());

		SetHighColor(Prefs.pointer_color);
		SetLowColor(Prefs.back_color);

		for (float y = POINTER_BAR_HEIGHT; y<r.bottom; y+=4)
			StrokeLine( BPoint( x, y), BPoint( x, y));

		StrokeLine( BPoint( x, POINTER_BAR_HEIGHT), BPoint( x, r.bottom));
	} else {
		float x = (fOwner->Pointer()-fOwner->Start())
			* Bounds().Width() /(fOwner->End() - fOwner->Start());
		SetHighColor(Prefs.pointer_color);

		StrokeLine( BPoint( x, POINTER_BAR_HEIGHT), BPoint( x, r.bottom));

		x = (fOwner->SelectionPointer()-fOwner->Start() +1)
			* Bounds().Width() /(fOwner->End() - fOwner->Start());

		StrokeLine( BPoint( x, POINTER_BAR_HEIGHT), BPoint( x, r.bottom));
	}
	
	/*if (Pool.IsPlaying()) {				// remove line
		SetDrawingMode(B_OP_INVERT);
		StrokeLine( BPoint( fOldX, Bounds().top+POINTER_BAR_HEIGHT), BPoint( fOldX, Bounds().bottom));
		SetDrawingMode(B_OP_COPY);
	}*/

	// these are to notify screen changes to update peak-caches or scroll fast
	fOldLeftPointer = fOwner->Start();
	fOldRightPointer = fOwner->End();
	m_resized = false;					//  put here, so all sub-routines can benefit
	fOwner->SetUpdateDrawCache(false);
	fUpdatePeak = false;				// needed for pencil edit

	Looper()->Unlock();
}


void
SampleView::MouseDown(BPoint p)
{
	if (fTrack->Size() == 0)
		return;

	BMessage *currentMsg = Window()->CurrentMessage();
	uint32 button = 0, clicks = 0;
	currentMsg->FindInt32("buttons", (int32 *)&button);
	currentMsg->FindInt32("clicks", (int32 *)&clicks);

	SetMouseEventMask(B_POINTER_EVENTS, B_NO_POINTER_HISTORY);

	if (p.y < POINTER_BAR_HEIGHT) {
		/* Handle the triangles here */
		return;
	}

	// The selection tool handles all select modes
	switch(Prefs.tool_mode) {
		case SELECT_TOOL:
		{
			if (clicks == 1) {
				float middle = (Bounds().Height()-POINTER_BAR_HEIGHT)*0.50+POINTER_BAR_HEIGHT;	// middle
				// calculate position of cursors on screen

				float pointer_x = (fOwner->Pointer()-fOwner->Start())
					* Bounds().Width()/(fOwner->End() - fOwner->Start());

				float sel_pointer_x = (fOwner->SelectionPointer()-fOwner->Start()+1)
					* Bounds().Width() /(fOwner->End() - fOwner->Start());
		
				bool left_select = false, right_select = false;

				bool left_pointer = (p.x < pointer_x+3 && p.x > pointer_x-3)
					&& fOwner->IsSelected();

				bool right_pointer = 
					(p.x < sel_pointer_x+3 && p.x > sel_pointer_x-3)
					&& fOwner->IsSelected();

				if (fSelected) {
					left_select = true;
					right_select = true;
				}

				bool drag_area = (p.x > pointer_x && p.x < sel_pointer_x) && fOwner->IsSelected()
								&& !left_pointer && !right_pointer && (left_select || right_select);
	
				// Do the selection
				if (button == B_SECONDARY_MOUSE_BUTTON) {
					// show a pop-up menu here

				}
				// Check for extending the selection
				else if (((left_pointer || right_pointer)
					&& (left_select || right_select))
					|| (button == B_PRIMARY_MOUSE_BUTTON
						&& (modifiers() & B_SHIFT_KEY))) {
					// Drag the end-pointers 
					drag_border = true;

					t = (int32)(fOwner->Start() + p.x 
						* (fOwner->End() - fOwner->Start())/Bounds().Width());

					// drag the right part
					if (t > (fOwner->Pointer() + fOwner->SelectionPointer())/2) {
						drag = true;

						// use original begin of selection
						t = (int32)fOwner->Pointer();			
						MouseMoved( p, 0, NULL );
					} else {
						drag = true;
						t = (int32)(fOwner->SelectionPointer());
						MouseMoved( p, 0, NULL );
					}
				} else if (drag_area) { 
					// start a dragging session
					drag_selection = true;
					fStartSelection = p;
				}
				// Here we start a new selection
				else if (button == B_PRIMARY_MOUSE_BUTTON && !(modifiers() & B_SHIFT_KEY)) {					// do new selection
					fOldX = -1;
					drag = true;
					fSelected = false;
					fOwner->SetSelectionPointer(0);
					t = (int32)(fOwner->Start() + p.x * (fOwner->End() - fOwner->Start())/Bounds().Width());
					fOwner->SetPointer(t);
				
					fOld = p;
					fStartSelection = p;
				} else {
					// interactive zooming
				
					float factor;
					float y = p.y;
					int32 m = (fOwner->End()+fOwner->Start())/2;				// center
					int32 x = fOwner->End() - fOwner->Start();
					float old_y = p.y;
	
					int32 in = (int32)(x - Bounds().Width()/16);		// possible to zoom in
					int32 out = fTrack->Size() -x;							// possible to zoom out
					if (out > in*6)			out = in*6;
					int32 z;
				
					float fOldX = p.x;
	
					while(button) {
						GetMouse(&p, &button);
						
						if (p.y != old_y || p.x != fOldX) {
							// scaled zoom
							factor = (y-p.y) *0.005;
							if (factor<0) {
								if (factor<-1)	factor =-1;
								z = (int32)((factor+1) * in/2 +Bounds().Width()/32);
							} else {
								if (factor>1)	factor =1;
								z = (int32)(factor * out/2 +x/2);
							}
	
							int32 add = (int32)((fOldX-p.x)*x/Bounds().Width());
			
							fOwner->SetStart(m - z + add);
							fOwner->SetEnd(m + z + add);
						
							if (fOwner->Start() < 0)
								fOwner->SetStart(0);
					
							if (fOwner->End() > fTrack->Size())
								fOwner->SetEnd(fTrack->Size());
	
							Invalidate();

							old_y = p.y;
							fOldX = p.x;
						}
						snooze(100000);
					}
				}
			} else if (clicks == 2 && Prefs.select_all_on_double) {
				/* The double-click selection */
				fOwner->SelectAll();
			}
			break;
		}
		case DRAW_TOOL:												
		{
			/* Drawing with the Pencil */
			/*if (Prefs.tool_mode == DRAW_TOOL && clicks == 1) {
				// save undo data
				//Hist.Save(H_REPLACE, fOwner->Start(), fOwner->End());
				WindowsManager::MainWindow()->UpdateMenu();
	
				fEditChannel = NONE;
				// needed to track stereo editing
				fOld = p;
				EditPoint( p );
				edit = true;
			}	*/
			break;
		}

		case PLAY_TOOL:
		{
			stop_following = true;
			fSelected = false;

			// Set the play-pointer
			fOwner->SetSelectionPointer(0);
			fOwner->SetPointer((int32)(fOwner->Start() + p.x
				* (fOwner->End() - fOwner->Start())/Bounds().Width()));

			if (button) {
				bool bRight = (button & B_SECONDARY_MOUSE_BUTTON);
				
				//Pool.StartPlaying(fOwner->Pointer()*Pool.sample_type, true);	// play till end
				
				Draw(Bounds());
				snooze(10000);

				while(button) {
					// Keep playing as long as the mouse is pressed
					GetMouse(&p, &button);
					Pulse();
	
					Window()->FindView("InfoToolBar")->Pulse();
					snooze(10000);
				}

				/*Pool.StopPlaying();
				if (bRight)
					fOwner->SetPointer(Pool.last_pointer);*/
	
				stop_following = false;
			}
			break;
		}
		default:
			break;
	}

	Invalidate();

	WindowsManager::MainWindow()->RedrawWindow();
	WindowsManager::MainWindow()->UpdateMenu();
}



void
SampleView::MouseMoved(BPoint p, uint32 button, const BMessage *msg)
{
	// area where the cursors work for left/right selection
	float top = (Bounds().Height() - POINTER_BAR_HEIGHT)
		* 0.20 + POINTER_BAR_HEIGHT;

	float bottom = (Bounds().Height() - POINTER_BAR_HEIGHT)
		* 0.80 + POINTER_BAR_HEIGHT;

	float middle = (Bounds().Height() - POINTER_BAR_HEIGHT)
		* 0.50 + POINTER_BAR_HEIGHT;

	// calculate position of cursors on screen
	float pointer_x = (fOwner->Pointer() - fOwner->Start())
		* Bounds().Width() /(fOwner->End() - fOwner->Start());

	float sel_pointer_x = (fOwner->SelectionPointer() - fOwner->Start()+1)
		* Bounds().Width() /(fOwner->End() - fOwner->Start());
	
	bool left_pointer = (p.x < pointer_x+3 && p.x > pointer_x-3)
		&& fOwner->IsSelected();

	bool right_pointer = (p.x < sel_pointer_x+3 && p.x > sel_pointer_x-3)
		&& fOwner->IsSelected();

	bool left_select = false, right_select = false;

	if (fSelected) {
		left_select = true;
		right_select = true;
	}

	bool drag_area = (p.x > pointer_x && p.x < sel_pointer_x)
		&& fOwner->IsSelected() && !left_pointer && !right_pointer
		&& (left_select || right_select);

	if (Prefs.tool_mode == DRAW_TOOL)
		// pencil mouse cursor 
		SetViewCursor( MouseIcons::MousePencil());
	else if (Prefs.tool_mode == SELECT_TOOL) {
		printf("move\n");
		// mousecursors for selections
		if (drag_selection)
			// drag&drop cursor goes above all
			SetViewCursor( MouseIcons::MouseArrow() );	
		else if (drag_border || (!drag && (left_pointer
			|| right_pointer) && (left_select || right_select)))
			SetViewCursor( MouseIcons::MouseLeftRight() );
		else if (fTrack->IsStereo())
			SetViewCursor( MouseIcons::MouseArrow() );
		else {
			if (drag_area && !drag)
				SetViewCursor( MouseIcons::MouseMove() );
			else if (p.y < top && p.y >= POINTER_BAR_HEIGHT)
				SetViewCursor( MouseIcons::MouseArrowLeft() );
			else if (p.y > bottom)
				SetViewCursor( MouseIcons::MouseArrowRight() );
			else
				SetViewCursor( MouseIcons::MouseArrow() );
		}
	} else
		SetViewCursor(B_CURSOR_SYSTEM_DEFAULT);

	// Handle the mouse-move events
	// decide which part would be selected
	bool full_update = false;

	if (Prefs.drag_drop && drag_selection) {
		// drag & drop
		if (fabs(p.x - fStartSelection.x) > 3) {
			BRect r = Bounds();
			r.top += POINTER_BAR_HEIGHT;
			r.left = p.x;
			r.right = p.x+1;
				
			/*if (modifiers() & B_COMMAND_KEY)
				ClipBoard.Cut(true);
			else
				ClipBoard.Copy();*/

			BMessage msg(FABER_DROP_PASTE);
			DragMessage(&msg, r);
			
			drag_selection = false;
		}
		fOld = p;
	} else if (drag && p!=fOld) {

		// do the selection
		if (fTrack->IsMono()) {
			fSelected = true;
			SetViewCursor(MouseIcons::MouseArrow());
		} else {
			// Check to see which channels are selected 
				if (fSelected)
					full_update = true;

				fSelected = true;
		}

		t2 = (int32)(fOwner->Start() + p.x * (fOwner->End()
			- fOwner->Start())/Bounds().Width());
		
		if (t > t2) {
			fOwner->SetPointer(t2);
			fOwner->SetSelectionPointer(t);
		} else {
			fOwner->SetPointer(t);
			fOwner->SetSelectionPointer(t2);
		}

		if (fOwner->Pointer() < 0)
			fOwner->SetPointer(0);

		if (fOwner->SelectionPointer() > fTrack->Size())
			fOwner->SetSelectionPointer(fTrack->Size());

		int32 step = (int32)((fOwner->End()
			- fOwner->Start())/Bounds().Width());

		// normal case just extend the width of the selection triangles
		int32 zoom_x = 4;
		if (step==0)
			zoom_x = (int32)MAX(ceil(Bounds().Width()
				/ (fOwner->End() - fOwner->Start())),POINTER_BAR_HEIGHT);

		BRect update;
		if (full_update)
			update = Bounds();
		else if (p.x>fOld.x)
			update.Set(fOld.x - zoom_x, 0, p.x + zoom_x, Bounds().bottom);
		else
			update.Set(p.x - zoom_x, 0, fOld.x + zoom_x, Bounds().bottom);

		/*if (Pool.IsPlaying()) {				// remove line
			SetDrawingMode(B_OP_INVERT);
			StrokeLine( BPoint( fOldX, Bounds().top+POINTER_BAR_HEIGHT), BPoint( fOldX, Bounds().bottom));
			SetDrawingMode(B_OP_COPY);
		}*/

		Draw(update);
		fOld = p;
	} else if (edit) {
		EditLine( fOld, p );
		fOld = p;
	}
}


void
SampleView::MouseUp(BPoint p)
{
	if (drag_selection && fStartSelection == p) {
		// single clicked in selection without drag -> deselect 
		fOldX = -1;
		fSelected = false;

		fOwner->SetSelectionPointer(0);
		fOwner->SetPointer((fOwner->Start() + p.x
			* (fOwner->End() - fOwner->Start())/Bounds().Width()));

		Invalidate();
	}

	drag = false;
	drag_selection = false;
	edit = false;
	drag_border = false;
	fOld.Set(-1,-1);

	WindowsManager::MainWindow()->UpdateMenu();
}


//Set a sample to a new value indicated by p
void
SampleView::EditPoint(BPoint p)
{
	/*// the sampleView area
	BRect r = Bounds();
	r.top += POINTER_BAR_HEIGHT;

	if ( p.x < 0 )
		p.x = 0;
	if ( p.x > r.right )
		p.x = r.right;

	// get the offset sample
	int64 ptr = (int64)(fOwner->Start() + p.x
		* (fOwner->End() - fOwner->Start())/Bounds().Width());

	// step between samples
	//float step = (fOwner->End()-fOwner->Start())/Bounds().Width();
	float v;

	if (fTrack->IsMono()) {

		float amp = r.Height() /2.0;
		float mid = amp  +POINTER_BAR_HEIGHT;

		v = (mid - p.y)/amp;
		if (v < -1.0)	v = -1.0;
		if (v > 1.0)	v = 1.0;

		DoDraw(ptr, 1, v);

	}else if (fTrack->IsStereo()) {
		// calc left area
		r.bottom = r.Height()/2.0f -2.0f +r.top;					
		float amp = r.Height()/2.0;
		float mid = amp  +r.top;
		if (r.Contains(p) && fEditChannel == NONE)
			fEditChannel = LEFT;
		
		if (fEditChannel == LEFT) {
			v = (mid - p.y)/amp;
			if (v < -1.0)	v = -1.0;
			if (v > 1.0)	v = 1.0;

			DoDraw(ptr*2, 2, v);
		} else {
			// calc right area
			r.OffsetTo(0, r.bottom+4.0f);					
			mid = amp  +r.top;
			if (r.Contains(p) && fEditChannel == NONE)
				fEditChannel = RIGHT;

			if (fEditChannel == RIGHT) {
				v = (mid - p.y)/amp;
				if (v < -1.0)	v = -1.0;
				if (v > 1.0)	v = 1.0;

				DoDraw(ptr*2+1, 2, v);
			}
		}
	}

	// Do some screen updating
	fUpdatePeak = true;

	int32 zoom_x =  (int32)MAX(ceil(Bounds().Width()
		/ (fOwner->End() - fOwner->Start())),POINTER_BAR_HEIGHT);

	if (p.x>fOld.x) {
		Invalidate(BRect(BPoint(fOld.x-zoom_x*2, 0),
			BPoint(p.x+zoom_x, Bounds().bottom)));
	} else {
		Invalidate(BRect(BPoint(p.x-zoom_x*2, 0),
			BPoint(fOld.x+zoom_x, Bounds().bottom)));
	}
	fOld = p;*/
}


void
SampleView::DoDraw(int64 ptr, int32 add, float v)
{
	int32 step = (int32)ceil((fOwner->End()-fOwner->Start())/Bounds().Width());

	float* area = fTrack->Area();

	if (step<=1) {

		if (ptr > fTrack->Size()*add)
			ptr = fTrack->Size()*add;

		if (ptr < 0 ) ptr = 0;
			area[ptr] = v;

	} else {

		// left part
		int32 start = ptr - step*add;

		if (start < 0)
			start = 0;
		
		for (int32 i=start; i<=ptr; i+=add) {
			area[i] = v;
		}

		// left part
		int32 end = ptr + step*add;
		if (end > fTrack->Size()*add)
			end = fTrack->Size()*add;

		for (int32 i=ptr; i<=end; i+=add) {
			area[i] = v;
		}
	}
}


//	Indicate that the frame is resized
void
SampleView::FrameResized(float width, float height)
{
	//printf("SampleView::FrameResized\n");

	// re-allocate offscreen bitmap
	m_resized = true;				

	// update the sample-view
	fOwner->SetDirty(true);

	// update the draw cache
	fOwner->SetUpdateDrawCache(true);	
	Invalidate();
}


// Create the cache bitmaps for drawing
// This is needed on resize, start and recoloring
void
SampleView::CalculateCache()
{
	if (fTrack->Size() == 0)
		return;

	acquire_sem(fViewSem);

	BFont font;
	GetFont(&font);
	font_height fh;
	font.GetHeight(&fh);

	int conv[] = {0, 2, 4, 8, 8, 10, 10, 10, 20, 20, 20, 20};

	// delete used bitmaps for cache
	if (fLeftCache) {
		delete fLeftCache;
		fLeftCache = NULL;
		cache_left_valid = false;
	}

	if (fRightCache) {
		delete fRightCache;
		fRightCache = NULL;
		cache_right_valid = false;
	}

	if (fLeftSelected) {
		delete fLeftSelected;
		fLeftSelected = NULL;
	}
	
	if (fRightSelected) {
		delete fRightSelected;
		fRightSelected = NULL;
	}

	BRect r = Bounds();
	r.bottom -= POINTER_BAR_HEIGHT;
	float amp = (r.IntegerHeight()+1)/2.0;
	int32 size;

	if (fTrack->IsMono())
		size = (int32)amp;
	else
		size = (int32)(amp+1)/2;

	int32 height_div = (int)ceil(amp/font.Size()) & 0xfffffe;

	if (height_div>20)
		height_div = 20;

	height_div = conv[height_div/2];

	int32 peak = (int32)(r.top + amp*(1.0f - Prefs.peak));

	// size of cache
	BRect rect(0, 0, size, size);
		
	fLeftCache = new BBitmap(rect, B_RGB32);
	fLeftSelected = new BBitmap(rect, B_RGB32);
		
	fLeftBits = (rgb_color*)fLeftCache->Bits();
	fLeftSelectedBits = (rgb_color*)fLeftSelected->Bits();
	fLeftWidth = fLeftCache->Bounds().IntegerWidth()+1;
	
	BitmapDrawer draw_left(fLeftCache);
	BitmapDrawer draw_fLeftSelected(fLeftSelected);

	rgb_color back2 = Prefs.back_color;				// background top
	rgb_color back1 = Prefs.back_color2;			// background middle
	rgb_color back2s = Prefs.back_selected_color;	// background top selected
	rgb_color back1s = Prefs.back_selected_color2;	// background middle selected
	rgb_color fore2s = Prefs.left_selected_color;	// foreground top
	rgb_color fore1s = Prefs.left_selected_color2;	// foreground middle
	rgb_color fore2 = Prefs.left_color;				// foreground top selected
	rgb_color fore1 = Prefs.left_color2;			// foreground middle selected

	float grid_y = 0;
	// draw the background
	rgb_color a, b, c, d;
	int32 temp;
	for (int32 y=0; y<=size; y++) {
		uint8 alpha = y*255/size;
		if (y==size) {
			// middle
			a = Prefs.mid_left_color;
			b = Prefs.mid_left_selected_color;
		} else if (Prefs.show_grid && y == (int)grid_y && y != size-1) {
			// horizontal grid
			a = Prefs.grid_color;
			b = Prefs.grid_selected_color;
			grid_y += r.Height()/height_div;
		} else if (y == peak && Prefs.show_peak) {
			// peak lines
			a = Prefs.peak_color;
			b = Prefs.peak_selected_color;
		} else {
			a.red = INT_BLEND(back1.red, back2.red, alpha, temp);
			a.green = INT_BLEND(back1.green, back2.green, alpha, temp);
			a.blue = INT_BLEND(back1.blue, back2.blue, alpha, temp);
			
			b.red = INT_BLEND(back1s.red, back2s.red, alpha, temp);
			b.green = INT_BLEND(back1s.green, back2s.green, alpha, temp);
			b.blue = INT_BLEND(back1s.blue, back2s.blue, alpha, temp);
		}

		c.red = INT_BLEND(fore1.red, fore2.red, alpha, temp);
		c.green = INT_BLEND(fore1.green, fore2.green, alpha, temp);
		c.blue = INT_BLEND(fore1.blue, fore2.blue, alpha, temp);
			
		d.red = INT_BLEND(fore1s.red, fore2s.red, alpha, temp);
		d.green = INT_BLEND(fore1s.green, fore2s.green, alpha, temp);
		d.blue = INT_BLEND(fore1s.blue, fore2s.blue, alpha, temp);
			
		// background
		for (int32 x=0; x<y; x++) {
			draw_left.PlotRGB( size - x, y, c );
			draw_fLeftSelected.PlotRGB( size - x, y, d );
		}
		// foreground
		for (int32 x=y; x<=size; x++) {
			draw_left.PlotRGB( size - x, y, a );
			draw_fLeftSelected.PlotRGB( size - x, y, b );
		}
	}
	
	cache_left_valid = true;

	if (fTrack->IsStereo()) {
		fRightCache = new BBitmap(rect, B_RGB32);
		fRightSelected = new BBitmap(rect, B_RGB32);
		
		fRightBits = (rgb_color*)fRightCache->Bits();
		fRightSelectedBits = (rgb_color*)fRightSelected->Bits();
	
		BitmapDrawer draw_right(fRightCache);
		BitmapDrawer draw_fRightSelected(fRightSelected);

		rgb_color fore2s = Prefs.right_selected_color;	// foreground top
		rgb_color fore1s = Prefs.right_selected_color2;	// foreground middle
		rgb_color fore2 = Prefs.right_color;			// foreground top selected
		rgb_color fore1 = Prefs.right_color2;			// foreground middle selected

		float grid_y = 0;
		// draw the background
		rgb_color a, b, c, d;
		int32 temp;
		for (int32 y=0; y<=size; y++) {

			uint8 alpha = y*255/size;
			if (y==size) {
				// middle
				a = Prefs.mid_right_color;
				b = Prefs.mid_right_selected_color;
			} else if (Prefs.show_grid && y == (int)grid_y && y != size-1) {
				// horizontal grid
				a = Prefs.grid_color;
				b = Prefs.grid_selected_color;
				grid_y += r.Height()/height_div;
			} else if (y == peak && Prefs.show_peak) {
				// peak lines
				a = Prefs.peak_color;
				b = Prefs.peak_selected_color;
			} else {
				a.red = INT_BLEND(back1.red, back2.red, alpha, temp);
				a.green = INT_BLEND(back1.green, back2.green, alpha, temp);
				a.blue = INT_BLEND(back1.blue, back2.blue, alpha, temp);
	
				b.red = INT_BLEND(back1s.red, back2s.red, alpha, temp);
				b.green = INT_BLEND(back1s.green, back2s.green, alpha, temp);
				b.blue = INT_BLEND(back1s.blue, back2s.blue, alpha, temp);
			}

			c.red = INT_BLEND(fore1.red, fore2.red, alpha, temp);
			c.green = INT_BLEND(fore1.green, fore2.green, alpha, temp);
			c.blue = INT_BLEND(fore1.blue, fore2.blue, alpha, temp);
			
			d.red = INT_BLEND(fore1s.red, fore2s.red, alpha, temp);
			d.green = INT_BLEND(fore1s.green, fore2s.green, alpha, temp);
			d.blue = INT_BLEND(fore1s.blue, fore2s.blue, alpha, temp);
			
			// background
			for (int32 x=0; x<y; x++) {
				draw_right.PlotRGB( size - x, y, c );
				draw_fRightSelected.PlotRGB( size - x, y, d );
			}
			// foreground
			for (int32 x=y; x<=size; x++) {
				draw_right.PlotRGB( size - x, y, a );
				draw_fRightSelected.PlotRGB( size - x, y, b );
			}
		}
		cache_right_valid = true;
	}
	release_sem(fViewSem);
}


//	Render MONO view
void
SampleView::DrawMono(BRect rect, bool left, bool draw_selection)
{
	acquire_sem(fViewSem);

	// Update the peak-cache if needed
	if (fUpdatePeak
	 || fOwner->UpdateDrawCache()
	 || fOldLeftPointer != fOwner->Start()
	 || fOldRightPointer != fOwner->End()) {

		if (left) {
			if (fTrack->IsMono()) {

				fTrack->PeakFile()->MonoBuffer(fLeftPeakBuffer, fOwner->Start(),
					fOwner->End(), Bounds().IntegerWidth()+1);

			} else if (fTrack->IsStereo()) {

				fTrack->PeakFile()->StereoBuffer(fLeftPeakBuffer, fRightPeakBuffer,
					fOwner->Start(), fOwner->End(), Bounds().IntegerWidth()+1);
			}
		}
	}

	int32 size = rect.IntegerHeight();
	int32 size2 = size/2;
	if ((size-size2)==size2) {
		// even
		size = size2;
	} else {
		// odd
		size = size2 +1;
	}

	rgb_color *inBits, *inSelectedBits, *outBits;
	rgb_color col, colS;

	// this one is used for line drawing
	float *peak_buffer;
	if (left) {
		// fill pointers for left channel
		outBits = fScreenBits;
		inBits = fLeftBits;
		inSelectedBits = fLeftSelectedBits;
		col = Prefs.left_color2;
		colS = Prefs.left_selected_color2;
		peak_buffer = fLeftPeakBuffer;
	} else {
		// fill pointers for right channel
		outBits = fScreenBits + size*2 * fScreenWidth;
		inBits = fRightBits;
		inSelectedBits = fRightSelectedBits;
		col = Prefs.right_color2;
		colS = Prefs.right_selected_color2;
		peak_buffer = fRightPeakBuffer;
	}

	// swap red/blue for direct draw
	uint8 red = col.red; col.red = col.blue; col.blue = red;

	// swap red/blue for direct draw
	red = colS.red; colS.red = colS.blue; colS.blue = red;

	// do the left part of the back
	if (!draw_selection || fOwner->Start()<fOwner->Pointer()
		|| fOwner->SelectionPointer()<fOwner->Start()) {
		BRect r = Bounds();
		if (draw_selection && fOwner->End()>fOwner->SelectionPointer()
			&& !(fOwner->SelectionPointer() < fOwner->Start()))
			// clip
			r.right -= (fOwner->End()-fOwner->Pointer())
				* Bounds().Width() /(fOwner->End() - fOwner->Start());
		
		// only draw when in update section
		if (r.right >= rect.left) {
			if (r.left < rect.left)
				r.left = rect.left;
			DrawPart( inBits, outBits, col, peak_buffer, r, size, size2);
		}
	}

	// do the middle part of the back
	if (draw_selection && fOwner->SelectionPointer()>=fOwner->Start()
		&& fOwner->Pointer()<=fOwner->End()) {

		BRect r = Bounds();
		if (fOwner->Pointer()>fOwner->Start())		// clip left
			r.left += floor((fOwner->Pointer()-fOwner->Start())
				* Bounds().Width() /(fOwner->End() - fOwner->Start())+1);

		if (fOwner->SelectionPointer()<fOwner->End())	// clip right
			r.right -= ceil((fOwner->End()-fOwner->SelectionPointer()-1)
				* Bounds().Width() /(fOwner->End() - fOwner->Start())-1);

		if (r.right == r.left)
			// make sure there is always a selection visible, even when 1 pixel
			r.left--;	

		if (r.right >= rect.left && r.left <= rect.right) {
			if (r.right > rect.right)
				r.right = rect.right;

			if (r.left < rect.left)
				r.left = rect.left;

			DrawPart(inSelectedBits, outBits, colS,
				peak_buffer, r, size, size2);
		}
	}

	// do the right part of the back
	if (draw_selection && fOwner->SelectionPointer() > fOwner->Start()
		&& fOwner->SelectionPointer()<fOwner->End()) {

		BRect r = Bounds();
		r.left += (fOwner->SelectionPointer()-fOwner->Start()+1)
			* Bounds().Width() / (fOwner->End() - fOwner->Start())+1;

		if (r.left <= rect.right) {

			if (rect.left > r.left)
				r.left = rect.left;
	
			if (rect.right < r.right)
				r.right = rect.right;

			DrawPart( inBits, outBits, col,
				peak_buffer, r, size, size2);
		}
	}
	release_sem(fViewSem);
}


//	Render STEREO view
void
SampleView::DrawPart(rgb_color *inBits, rgb_color *outBits,
	rgb_color col, float *peak_buffer,
	BRect r, int32 size, int32 size2)
{
	#ifndef DEBUG_DISABLE_DRAWPART 

	rgb_color *src = NULL;
	rgb_color *dest = NULL;

	int32 draw_mode;
	int32 step = (fOwner->End()-fOwner->Start())
		/ Bounds().IntegerWidth();

	if (step < 1)
		draw_mode = DRAW_POINTS;
	else if ( step < 64 )
		draw_mode = DRAW_PLAIN;
	else
		draw_mode = DRAW_PEAK;

	switch (draw_mode)
	{
		case DRAW_POINTS:
		case DRAW_PLAIN:
		{
			int middle;
			// calc startpoint in buffer
			int32 index = (int32)r.left*2;

			int32 old_index = index-2;
			if (old_index<0)
				old_index = 0;
	
			int32 fOldX1 = (int32)(size*peak_buffer[old_index]);

			for (int32 x = (int32)r.left; x<=(int32)r.right; x++) {

				int32 x1 = (int32)(size*peak_buffer[index]);

				// screen
				dest = outBits +x;

				// background		
				src = inBits;
				for (int32 y=0; y<=size; y++) {
					*dest = *src;
					dest += fScreenWidth;
					src += fLeftWidth;
				}
				src -= fLeftWidth;
				for (int32 y=0; y<size2; y++) {
					src -= fLeftWidth;
					*dest = *src;
					dest += fScreenWidth;
				}
	
				if (x1 == fOldX1) {
					*(outBits + x + (size-x1)*fScreenWidth) = col;
				} else if (x1 < fOldX1) {
					middle = ( x1 + fOldX1 +1)>>1;
					for (int32 offset = x1; offset < middle; offset++)
						*(outBits + x + (size-offset)*fScreenWidth) = col;
	
					if (x != 0)
						for (int32 offset = middle; offset < fOldX1; offset++)
							*(outBits + x-1 + (size-offset)*fScreenWidth) = col;
				} else {
					middle = ( x1 + fOldX1 )>>1;
					if (x != 0)
						for (int32 offset = fOldX1; offset < middle; offset++)
							*(outBits + x-1 + (size-offset)*fScreenWidth) = col;
	
					for (int32 offset = middle; offset < x1; offset++)
						*(outBits + x + (size-offset)*fScreenWidth) = col;
				}
				fOldX1 = x1;
						
				index += 2;
			}
		}	break;
					
		default:
		{
			// need to do the first unselected part here
			// calc startpoint in buffer
			int32 index = (int32)r.left*2;

			for (int32 x = (int32)r.left; x<=(int32)r.right; x++) {

				int32 x1 = (int32)(-size*peak_buffer[index]);
				int32 x2 = (int32)(size*peak_buffer[index+1]);
	
				// offset in screenbuffer
				dest = outBits +x;
	
				// offset in cache
				src = inBits + x2;
	
				for (int32 y=0; y<=size; y++) {
					*dest = *src;
					dest += fScreenWidth;
					src += fLeftWidth;
				}
	
				// offset in cache
				src = inBits + x1 +size * fLeftWidth - fLeftWidth;
	
				for (int32 y=0; y<size2; y++) {
					*dest = *src;
					dest += fScreenWidth;
					src -= fLeftWidth;
				}
	
				index += 2;
			}
			break;
		}
	}
	#endif
}


//	Render STEREO view
void SampleView::DrawStereo(BRect rect)
{
	BRect r = rect;
	r.bottom -= POINTER_BAR_HEIGHT;
	r.bottom /= 2;
	if (cache_left_valid)
		DrawMono(r, true, fSelected);
	
	r.OffsetBy(0, r.Height());
	if (cache_right_valid)
		DrawMono(r, false, fSelected);
}


//Edit a line
void SampleView::EditLine(BPoint pa, BPoint pb)
{
	if  (pa==pb)
		EditPoint(pa);
	else {
		float delta_x = (pa.x - pb.x);
		float delta_y = (pa.y - pb.y);

		if (delta_x < 0)
			delta_x *= -1;

		if (delta_y < 0)
			delta_y *= -1;

		float delta = sqrt((delta_x*delta_x) + (delta_y*delta_y));
		float nbpts;

		nbpts = delta / 1.0;
   
		BPoint pos;
		for (float i = 0; i < nbpts-0; i++) {
			// was floor
			pos.x = ROUND(pa.x + i*(pb.x-pa.x)/nbpts);
			pos.y = ROUND(pa.y + i*(pb.y-pa.y)/nbpts);

			EditPoint(pos);
		}
	}
}
