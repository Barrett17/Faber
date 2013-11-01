/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#include "SpinSlider.h"

#include <GroupLayoutBuilder.h>
#include <ControlLook.h>

#include <stdio.h>

#define MSG_SPIN_CHANGED 	'spCh'


SpinSlider::SpinSlider(const char* name, const char* label,
	BMessage* message, int32 minValue, int32 maxValue)
	:
	BView(name, B_FOLLOW_ALL)
{
	fSlider = new VolumeSlider(name, minValue,
		maxValue, maxValue/2, message);

	SetLabel(label);
	_Init(minValue, maxValue);
}


SpinSlider::~SpinSlider()
{
}


void SpinSlider::MessageReceived(BMessage* msg)
{
	msg->PrintToStream();
	switch(msg->what)
	{
		// This is a custom message sent from seekslider
		// to make the spincontrol updated. Feel free to replace it
		// with something better.
		/*case CUSTOM_INVOKED:
			SetValue(fSlider->Value());
		break;*/

		case MSG_SPIN_CHANGED:
			if (fSpinControl) 
				SetValue(fSpinControl->Value());
			fSlider->Invoke();
		break;
		default:
			BView::MessageReceived(msg);
			break;
	}
}


void
SpinSlider::_Init(int32 minValue, int32 maxValue)
{
	fSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	fSlider->SetHashMarkCount(10);

	fSpinControl = new Spinner(BRect(50, 50, 0, 0), "SpinControl",
		NULL, new BMessage(MSG_SPIN_CHANGED));

	fSpinControl->SetRange(minValue, maxValue);

	fSpinControl->SetValue(fSlider->Value());

	SetLayout(new BGroupLayout(B_HORIZONTAL));
	AddChild(BGroupLayoutBuilder(B_HORIZONTAL)
		.AddGroup(B_HORIZONTAL)
			.Add(fSlider)
		.End()
		.Add(fSpinControl)
	);
}


void SpinSlider::AttachedToWindow(void)
{
	BView::AttachedToWindow();

	fSpinControl->SetEnabled(fSlider->IsEnabled());
	fSpinControl->SetTarget(this);
	fSlider->SetTarget(this);

	//Invalidate();
}


void
SpinSlider::SetLabel(const char* label)
{
	fSlider->SetLabel(label);
}


float
SpinSlider::Value() const
{
	return fSlider->Value();
}


void
SpinSlider::SetValue(float value)
{
	if (fSpinControl && (fSpinControl->Value() != value))
		fSpinControl->SetValue(value);
	fSlider->SetValue(value);
}


void
SpinSlider::MouseDown(BPoint point)
{
	if (fSlider->IsEnabled()) {
		SetMouseEventMask(B_POINTER_EVENTS, B_NO_POINTER_HISTORY); 

		BRect frame = fSlider->BarFrame();

		if (point.x >= frame.left && point.x <= frame.right) {
			SetValue(fSlider->ValueForPoint(point));
			fSlider->Invoke();
		}
	}
}
