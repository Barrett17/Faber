/*
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#ifndef MIXER_WINDOW_H
#define MIXER_WINDOW_H

#include <Application.h>
#include <GroupLayout.h>
#include <Window.h>
#include <View.h>
#include "MixerView.h"


class MixerWindow : public BWindow 
{
public:
							MixerWindow();
	virtual	void			MessageReceived(BMessage* mesage);
	virtual	bool    		QuitRequested();


private:
			MixerView*	fMixerView;
};

#endif
