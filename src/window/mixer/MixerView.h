/*
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#ifndef MIXER_VIEW_H
#define MIXER_VIEW_H

#include <View.h>
#include <MenuBar.h>
#include <Menu.h>
#include <Button.h>
#include <Slider.h>
#include <CheckBox.h>
#include <List.h>
#include "Knob.h"


const uint32 MSG_MIXER_PAN		=  0x000000F0;
const uint32 MSG_MIXER_MUTE		=  0x000F0000;
const uint32 MSG_MIXER_SOLO		=  0xF0000000;
const uint32 MSG_MIXER_VOLUME	=  0x00000001;
const uint32 MSG_MIXER_MASTER	=  0x00000000;


typedef struct
{
	BSlider*	VolumeSlider;
	BCheckBox*	MuteBox;
	BCheckBox*	SoloBox;
	BSlider*	PanKnob; // Will be a knob object
	
	int32		TrackId;
} MixerTrackEntry;


class MixerView : public BView 
{
public:
						MixerView();
						~MixerView();
			void		AttachedToWindow();	
	virtual	void		MessageReceived(BMessage* message);

private:
			BSlider*	fMasterVolume;
			BList*		fEntryList;

};



#endif
