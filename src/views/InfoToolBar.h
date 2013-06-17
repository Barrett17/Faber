/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef INFOTOOLBAR_VIEW
#define INFOTOOLBAR_VIEW

#include <View.h>

#include "IconButton.h"
#include "Globals.h"

class BTextView;


class InfoToolBar : public BView
{
public:
							InfoToolBar();
	virtual 				~InfoToolBar();

	void					Pulse();
private:
			IconButton*		_BuildButton(const char* tip, BMessage* message,
								int32 resourceID);

			BTextView*		fPointerTextView;
			int64			pointer;
};

#endif
