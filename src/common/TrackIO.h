/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <Message.h>

#include "Track.h"


class TrackIO
{
public:
	static TrackIO*				Get();
	Track*						Load(BMessage* msg);
	Track*						Save(BMessage* msg);

private:
	static TrackIO*				fInstance;
	bool						fSaveMode;
};
