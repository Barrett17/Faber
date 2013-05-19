/*
 * Copyright 2011 Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#ifndef __TIME_TRACK_H
#define __TIME_TRACK_H

#include "Globals.h"
#include <View.h>


class TimeTrack : public BView
{
public:
					TimeTrack(BRect frame, const char* name, uint32 resizingMode, uint32 flags);
	virtual 		~TimeTrack();
	virtual	void 	Draw(BRect updateRect);
};

#endif
