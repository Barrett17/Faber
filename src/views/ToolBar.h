/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef TOOLBAR_VIEW
#define TOOLBAR_VIEW

#include <View.h>

#include "Globals.h"

class IconButton;


class ToolBar: public BView
{
public:
							ToolBar();
	virtual 				~ToolBar();
			void			Update();
private:
			IconButton*		_BuildButton(const char* tip, BMessage* message,
								int32 resourceID);
};

#endif
