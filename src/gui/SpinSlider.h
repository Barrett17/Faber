/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef SPIN_SLIDER_H
#define SPIN_SLIDER_H

#include "VolumeSlider.h"
#include "Spinner.h"


class SpinSlider : public BView {
public:
								SpinSlider(const char* name, const char* label,
									BMessage* message, int32 minValue,
									int32 maxValue);

	virtual						~SpinSlider();

	virtual void				AttachedToWindow();
	virtual void 				MessageReceived(BMessage* msg);
			void				MouseDown(BPoint point);

	// BSlider mimics
	virtual void				SetLabel(const char* label);
	virtual float				Value() const;
	virtual void				SetValue(float value);
			VolumeSlider*			Slider() { return fSlider; }

private:
			void				_Init(int32 minValue, int32 maxValue);
			Spinner*			fSpinControl;
			VolumeSlider*		fSlider;
};


#endif	//SEEK_SLIDER_H
