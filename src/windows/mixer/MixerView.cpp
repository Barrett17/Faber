/*
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#include <Button.h>
#include <GroupLayout.h>
#include <Slider.h>
#include <TextControl.h>
#include <Window.h>
#include <View.h>
#include <Box.h>
#include <Catalog.h>
#include <CheckBox.h>
#include <GroupView.h>
#include <List.h>

#include "Knob.h"
#include "TrackRepository.h"
#include "CommandRepository.h"
#include "MixerView.h"


MixerView::~MixerView()
{
	int i;
	
	for(i = 0; i < fEntryList->CountItems(); i++)
	{
		MixerTrackEntry	*trackEntry = (MixerTrackEntry*) fEntryList->ItemAt(i);
		
		if(trackEntry != NULL)
			delete trackEntry;
	}
	delete fEntryList;
}


MixerView::MixerView()
	:
	BView("MixerView", B_WILL_DRAW, 0)
{	
	rgb_color barColor = { 0, 0, 240 };
	rgb_color fillColor = { 240, 0, 0 };

	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	// Main layout
	BGroupLayout* rootLayout = new BGroupLayout(B_HORIZONTAL);
	SetLayout(rootLayout);
	
	// Root slider
	fMasterVolume = new BSlider(BRect(0, 0, 20, 100), " ", "Master",
				new BMessage(MSG_MIXER_MASTER), 0, 100, B_VERTICAL);
	fMasterVolume->SetBarColor(barColor);
	fMasterVolume->UseFillColor(true, &fillColor);
	fMasterVolume->SetHashMarks(B_HASH_MARKS_BOTTOM);
	fMasterVolume->SetHashMarkCount(10);
	fMasterVolume->SetValue(TrackRepository::Instance()->MasterVolume());

	rootLayout->AddView(fMasterVolume);

			
	// Add tracks
	int32 i;
	BList *trackList = TrackRepository::Instance()->TrackList();
	
	fEntryList = new BList(10);
		
	for(i = 0; i < trackList->CountItems(); i++)
	{
		MixerTrackEntry* trackEntry = new MixerTrackEntry;
		
		trackEntry->TrackId = i;
		
		BGroupView* audioGroup = new BGroupView(B_HORIZONTAL);
		BBox* audioBox = new BBox("audiobox");
		audioBox->SetLabel("Audio files");

		BGroupLayout* audioLayout = new BGroupLayout(B_VERTICAL);
		audioLayout->SetInsets(10, audioBox->TopBorderOffset() * 2 + 10, 10, 10);
		audioBox->SetLayout(audioLayout);

		audioGroup->GroupLayout()->AddView(audioBox);
		rootLayout->AddView(audioGroup);
	
		
		Track *track = (Track *) trackList->ItemAt(i);
		char label[32];
		
		sprintf(label, "%s (%d)", track->Name(), (i + 1));
		
		audioBox->SetLabel(label);
		
		trackEntry->VolumeSlider = new BSlider(BRect(0, 0, 20, 100), " ", "Volume",
				new BMessage((int32) (MSG_MIXER_VOLUME + i)), 0, 100, B_VERTICAL);
		
		trackEntry->VolumeSlider->SetBarColor(barColor);
		trackEntry->VolumeSlider->UseFillColor(true, &fillColor);
		trackEntry->VolumeSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
		trackEntry->VolumeSlider->SetHashMarkCount(10);
		trackEntry->VolumeSlider->SetValue(track->Volume());
		
		audioLayout->AddView(trackEntry->VolumeSlider);
		
		
		//trackEntry->PanKnob = new Knob(BRect(0, 0, 30, 30), " ", "Pan", 
		//		new BMessage((int32) (MSG_MIXER_PAN + i)));
		trackEntry->PanKnob = new BSlider(BRect(0, 0, 20, 100), " ", "Pan",
				new BMessage((int32) (MSG_MIXER_PAN + i)), -10, 10, B_HORIZONTAL);
		
		trackEntry->PanKnob->SetBarColor(barColor);
		trackEntry->PanKnob->UseFillColor(true, &fillColor);
		trackEntry->PanKnob->SetHashMarks(B_HASH_MARKS_BOTTOM);
		trackEntry->PanKnob->SetHashMarkCount(10);
		trackEntry->PanKnob->SetValue(track->Pan());
				
		audioLayout->AddView(trackEntry->PanKnob);



		
		trackEntry->MuteBox = new BCheckBox(BRect(), " ", "Mute",
				new BMessage((int32) (MSG_MIXER_MUTE + i)));
		trackEntry->MuteBox->SetValue(track->Mute());		
		audioLayout->AddView(trackEntry->MuteBox);
		
		
		
		trackEntry->SoloBox = new BCheckBox(BRect(), " ", "Solo",
				new BMessage((int32) (MSG_MIXER_SOLO + i)));
		trackEntry->SoloBox->SetValue(track->Solo());		
		audioLayout->AddView(trackEntry->SoloBox);
				
		
		fEntryList->AddItem(trackEntry);		
	}
}


void MixerView::AttachedToWindow()
{
	int i;
	
	fMasterVolume->SetTarget(this);
	
	for(i = 0; i < fEntryList->CountItems(); i++)
	{
		MixerTrackEntry	*trackEntry = (MixerTrackEntry*) fEntryList->ItemAt(i);
		
		if(trackEntry != NULL)
		{
			trackEntry->VolumeSlider->SetTarget(this);
			trackEntry->SoloBox->SetTarget(this);
			trackEntry->MuteBox->SetTarget(this);
			trackEntry->PanKnob->SetTarget(this);	
		}
	}
	
	
	Window()->ResizeTo(80 * fEntryList->CountItems() + 80, 200);
	Window()->CenterOnScreen();
}


void MixerView::MessageReceived(BMessage *message)
{
	int32 tracks = fEntryList->CountItems();
	int i;
	
	if(message->what == MSG_MIXER_MASTER)
	{
		CommandRepository::Instance()->SetMasterVolume((int32) fMasterVolume->Value());
	}
	else if((message->what >= MSG_MIXER_SOLO) && (message->what <= (MSG_MIXER_SOLO + tracks)))
	{
		MixerTrackEntry *trackEntry = NULL;
		int32 trackId = message->what - MSG_MIXER_SOLO + 1;	
					
		for(i = 0; i < tracks; i++)
		{
			if((((MixerTrackEntry*) (fEntryList->ItemAt(i)))->SoloBox->Message()->what) == message->what)
			{
				trackEntry = ((MixerTrackEntry*) (fEntryList->ItemAt(i)));
				break;
			}
		}
		
		if(trackEntry != NULL)
		{
			bool state = (bool) trackEntry->SoloBox->Value();
	
			CommandRepository::Instance()->SetTrackSolo(trackEntry->TrackId, state);
			//printf("Solo: %d %d\n", state, trackEntry->TrackId);
		}
	}
	else if((message->what >= MSG_MIXER_MUTE) && (message->what <= (MSG_MIXER_MUTE + tracks)))
	{
		MixerTrackEntry *trackEntry = NULL;
		int32 trackId = message->what - MSG_MIXER_MUTE + 1;	
					
		for(i = 0; i < tracks; i++)
		{
			if((((MixerTrackEntry*) (fEntryList->ItemAt(i)))->MuteBox->Message()->what) == message->what)
			{
				trackEntry = ((MixerTrackEntry*) (fEntryList->ItemAt(i)));
				break;
			}
		}
		
		if(trackEntry != NULL)
		{
			bool state = (bool) trackEntry->MuteBox->Value();
	
			CommandRepository::Instance()->SetTrackMute(trackEntry->TrackId, state);
			//printf("Mute: %d %d\n", state, trackEntry->TrackId);
		}
	}
	else if((message->what >= MSG_MIXER_VOLUME) && (message->what <= (MSG_MIXER_VOLUME + tracks)))
	{
		MixerTrackEntry *trackEntry = NULL;
		int32 trackId = message->what - MSG_MIXER_VOLUME + 1;	
					
		for(i = 0; i < tracks; i++)
		{
			if((((MixerTrackEntry*) (fEntryList->ItemAt(i)))->VolumeSlider->Message()->what) == message->what)
			{
				trackEntry = ((MixerTrackEntry*) (fEntryList->ItemAt(i)));
				break;
			}
		}
		
		if(trackEntry != NULL)
		{
			int32 vol = trackEntry->VolumeSlider->Value();
	
			CommandRepository::Instance()->SetTrackVolume(trackEntry->TrackId, (int32) vol);
			//printf("Volume: %d %d\n", vol, trackEntry->TrackId);
		}
	}
	else if((message->what >= MSG_MIXER_PAN) && (message->what <= (MSG_MIXER_PAN + tracks)))
	{
		MixerTrackEntry *trackEntry = NULL;
		int32 trackId = message->what - MSG_MIXER_PAN + 1;	
					
		for(i = 0; i < tracks; i++)
		{
			if((((MixerTrackEntry*) (fEntryList->ItemAt(i)))->PanKnob->Message()->what) == message->what)
			{
				trackEntry = ((MixerTrackEntry*) (fEntryList->ItemAt(i)));
				break;
			}
		}
		
		if(trackEntry != NULL)
		{
			int32 pan = trackEntry->PanKnob->Value();
	
			CommandRepository::Instance()->SetTrackPan(trackEntry->TrackId, pan);
			//printf("Volume: %d %d\n", vol, trackEntry->TrackId);
		}
	}
	else
		BView::MessageReceived(message);
		
}


