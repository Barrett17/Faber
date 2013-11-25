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

/* 
	Contains code from Icon-O-Matic SwatchView by Stephan Aßmus
*/
#include <String.h>
#include "SwatchView.h"

#include <stdio.h>

#define SET_COLOR	'setC'

const rgb_color kAlphaHigh	= { 0xe0, 0xe0, 0xe0, 0xff };
const rgb_color kAlphaLow	= { 0xbb, 0xbb, 0xbb, 0xff };
const pattern kDottedBig	= { { 0x0f, 0x0f, 0x0f, 0x0f, 0xf0, 0xf0, 0xf0, 0xf0 } };


char*
RGBtoText(const rgb_color& color) 
{
	static const char* p="0123456789ABCDEF";

	char *rgbtxt = new char[8];

	rgbtxt[0]='#';
	rgbtxt[1]=p[(color.red & 0xF0)>>4];
	rgbtxt[2]=p[(color.red & 0x0F)];
	rgbtxt[3]=p[(color.green & 0xF0)>>4];
	rgbtxt[4]=p[(color.green & 0x0F)];
	rgbtxt[5]=p[(color.blue & 0xF0)>>4];
	rgbtxt[6]=p[(color.blue & 0x0F)];
	rgbtxt[7]='\0';

	return rgbtxt;
}

SwatchView::SwatchView(const char *name, BMessage* msg)
	:
	BControl("swatchview", "", msg, B_FOLLOW_ALL)
{
	Init();
	fName = name;
	fActive = false;
	fColor = { 0, 0, 0, 0 };
	SetViewColor(B_TRANSPARENT_32_BIT);
	SetHighColor(fColor);
	DrawOffscreen();
}

SwatchView::SwatchView(const char *name, rgb_color color, BMessage* msg)
	:
	BControl("swatchview", "", msg, B_FOLLOW_ALL)
{
	Init();
	fColor = color;
	fName = name;
	SetViewColor(B_TRANSPARENT_32_BIT);
	SetHighColor(fColor);
	DrawOffscreen();
}

SwatchView::~SwatchView()
{
	fBitmap->RemoveChild(fView);
	delete fView;
	delete fBitmap;
}

void
SwatchView::SetActive(bool c)
{
	fActive = c;
}

void
SwatchView::Init()
{
	BRect frame = Bounds();
	fBitmap = new BBitmap(frame, B_RGB32, true);
	fView = new BView("offscreen", B_FOLLOW_LEFT);
	fBitmap->AddChild(fView);
	fColor.red = fColor.green = fColor.blue = 0;
	DrawOffscreen();
}


inline void
blend_color(rgb_color& a, const rgb_color& b, float alpha)
{
	float alphaInv = 1.0 - alpha;
	a.red = (uint8)(b.red * alphaInv + a.red * alpha);
	a.green = (uint8)(b.green * alphaInv + a.green * alpha);
	a.blue = (uint8)(b.blue * alphaInv + a.blue * alpha);
}


void
SwatchView::Draw(BRect updateRect)
{
	BRect r(Bounds());

	rgb_color colorLight = tint_color(fColor, B_LIGHTEN_2_TINT);
	rgb_color colorShadow = tint_color(fColor, B_DARKEN_2_TINT);

	if (fColor.alpha < 255) {
		// left/top
		float alpha = fColor.alpha / 255.0;

		rgb_color h = colorLight;
		blend_color(h, kAlphaHigh, alpha);
		rgb_color l = colorLight;
		blend_color(l, kAlphaLow, alpha);

		SetHighColor(h);
		SetLowColor(l);
		
		StrokeLine(BPoint(r.left, r.bottom - 1),
				   BPoint(r.left, r.top), kDottedBig);
		StrokeLine(BPoint(r.left + 1, r.top),
				   BPoint(r.right, r.top), kDottedBig);

		// right/bottom
		h = colorShadow;
		blend_color(h, kAlphaHigh, alpha);
		l = colorShadow;
		blend_color(l, kAlphaLow, alpha);

		SetHighColor(h);
		SetLowColor(l);
		
		StrokeLine(BPoint(r.right, r.top + 1),
				   BPoint(r.right, r.bottom), kDottedBig);
		StrokeLine(BPoint(r.right - 1, r.bottom),
				   BPoint(r.left, r.bottom), kDottedBig);

		// fill
		r.InsetBy(1.0, 1.0);

		h = fColor;
		blend_color(h, kAlphaHigh, alpha);
		l = fColor;
		blend_color(l, kAlphaLow, alpha);

		SetHighColor(h);
		SetLowColor(l);		

		FillRect(r, kDottedBig);
	} else {
		_StrokeRect(r, colorLight, colorShadow);
		r.InsetBy(1.0, 1.0);
		SetHighColor(fColor);
		FillRect(r);
	}
}


void SwatchView::MouseDown(BPoint where)
{
	if (!IsEnabled())
		return;

	BPoint w2;
	uint32 mods;
	while(true)
	{
		GetMouse(&w2, &mods);
		if (!mods) //releasing the buttons without moving means no drag
		{
			Invoke();
			return;	
		}
		if (w2 != where)
			break;
	}
	snooze(40000);

	BMessage msg(B_PASTE);
	msg.AddData("RGBColor",B_RGB_COLOR_TYPE,(const void**)&fColor,sizeof(rgb_color));
	BString s = RGBtoText(fColor);
	msg.AddData("text/plain",B_MIME_DATA,(const void**)s.String(),s.Length());
	msg.AddString("be:types", "text/plain");
	msg.AddInt32("be:actions", B_COPY_TARGET);
	msg.AddInt32("be:actions", B_TRASH_TARGET);
	  
	BBitmap *bitmap = make_bitmap();
	 
	BPoint pt(bitmap->Bounds().Width()/2.0, bitmap->Bounds().Height()/2);
	DragMessage(&msg, bitmap, B_OP_ALPHA, pt, Window());
}


BBitmap*
SwatchView::make_bitmap(void)
{
	BRect rect(0.0, 0.0, 12.0, 12.0);
	
	BBitmap *bitmap = new BBitmap(rect, B_RGB32, true);
	BView *view = new BView(rect, "", B_FOLLOW_NONE, B_WILL_DRAW);

	bitmap->Lock();

	bitmap->AddChild(view);

	view->SetDrawingMode(B_OP_ALPHA);
	view->SetHighColor(fColor);
	view->FillRect(rect);
	
	view->SetDrawingMode(B_OP_COPY);
	view->SetHighColor(0, 0, 0, 255);
	view->StrokeRect(rect);
	view->Sync();

	bitmap->RemoveChild(view);
	delete view;

	bitmap->Unlock();
	
	return bitmap;
}


void
SwatchView::SetColor(const rgb_color &color)
{
	fColor = color;
	DrawOffscreen();
}


void
SwatchView::SetEnabled(bool enabled)
{
	BControl::SetEnabled(enabled);
	DrawOffscreen();
}


void
SwatchView::DrawOffscreen()
{
	// blit to screen if attached to window
	if (Window()) {
		if (Window()->Lock()) {
			Draw(Bounds());
			Window()->Unlock();
		}
	}
}


rgb_color
SwatchView::Color(void) const
{
	return fColor;
}


void
SwatchView::MessageReceived(BMessage *msg){
	switch(msg->what) {
	case B_PASTE:
	case B_OK:
		{
			if (!IsEnabled())
				return;

			rgb_color *color;
			ssize_t s;
			msg->FindData("RGBColor",B_RGB_COLOR_TYPE,(const void**)&color,&s);
			SetColor(*color);
			Invoke();
		}
		break;

	case SET_COLOR:
		{
			const char *title;
			rgb_color *color;
			ssize_t n = sizeof(struct rgb_color);
			if (msg->FindData("RGBColor", (type_code) B_RGB_COLOR_TYPE,
				(const void **) &color, &n) == B_OK
					&& msg->FindString("Title", (const char **) &title) == B_OK) {
				SetColor(*color);
				Invoke();
			}
		}
		break;

	default:
		BControl::MessageReceived(msg);
	}
}


void
SwatchView::_StrokeRect(BRect r, rgb_color leftTop, rgb_color rightBottom)
{
	BeginLineArray(4);

	AddLine(BPoint(r.left, r.bottom - 1), BPoint(r.left, r.top), leftTop);
	AddLine(BPoint(r.left + 1, r.top), BPoint(r.right, r.top), leftTop);
	AddLine(BPoint(r.right, r.top + 1), BPoint(r.right, r.bottom),
		rightBottom);
	AddLine(BPoint(r.right - 1, r.bottom), BPoint(r.left, r.bottom),
		rightBottom);

	EndLineArray();
}
