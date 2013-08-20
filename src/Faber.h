/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef FABER_APP
#define FABER_APP

#include <Application.h>

#include "FaberWindow.h"
#include "Globals.h"


class FaberApp : public BApplication
{
public:
							FaberApp();
	virtual void			MessageReceived(BMessage* message);
	virtual void			RefsReceived(BMessage* message);
	virtual bool			QuitRequested();

			void			Save(BMessage* message);
private:
 			FaberWindow*	fFaberWindow;
};

#endif
