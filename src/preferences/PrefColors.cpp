/*
   	Copyright (c) 2003, Xentronix
	Author: Frans van Nispen (frans@xentronix.com)
	All rights reserved.
	
	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:
	
	Redistributions of source code must retain the above copyright notice, this list
	of conditions and the following disclaimer. Redistributions in binary form must
	reproduce the above copyright notice, this list of conditions and the following
	disclaimer in the documentation and/or other materials provided with the distribution. 
	
	Neither the name of Xentronix nor the names of its contributors may be used
	to endorse or promote products derived from this software without specific prior
	written permission. 
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
	SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <String.h>
#include <Path.h>
#include <TranslationKit.h>
#include <TranslationUtils.h>
#include <GroupLayoutBuilder.h>
#include <ControlLook.h>

#include "Globals.h"
#include "PrefColors.h"
#include "SwatchView.h"
#include "FStringItem.h"

#include <stdio.h>

#define COLOR_SELECT		'colS'
#define COLOR_CHANGE		'colC'
#define SWATCH_DROP			'swtc'
#define NEW_SCHEME			'schm'

struct defaultSchemeNames {
	const char* schemeString;
};

struct colorNames {
	const char* colorString;
};

static struct defaultSchemeNames gColorSchemes[] = {
{ B_TRANSLATE("Cold Cut") },
{ B_TRANSLATE("Shades of blue") },
{ B_TRANSLATE("Cool Colors") },
{ B_TRANSLATE("Black & White") },
{ B_TRANSLATE("BeAE (Soft Colors)") },
{ NULL }
};

static struct colorNames gColorsNames[] = {
{ B_TRANSLATE("Background") },
{ B_TRANSLATE("Background 2") },
{ B_TRANSLATE("Background selected") },
{ B_TRANSLATE("Background selected 2") },
{ B_TRANSLATE("Index background") },
{ B_TRANSLATE("Index background 2") },
{ B_TRANSLATE("Index middle") },
{ B_TRANSLATE("Index wave") },
{ B_TRANSLATE("Index wave 2") },
{ B_TRANSLATE("Index background selected") },
{ B_TRANSLATE("Index background selected 2") },
{ B_TRANSLATE("Index wave selected") },
{ B_TRANSLATE("Index wave selected 2") },
{ B_TRANSLATE("Index middle selected") },
{ B_TRANSLATE("Index pointer") },
{ B_TRANSLATE("Left channel") },
{ B_TRANSLATE("Left channel 2") },
{ B_TRANSLATE("Left selected") },
{ B_TRANSLATE("Left selected 2") },
{ B_TRANSLATE("Right channel") },
{ B_TRANSLATE("Right channel 2") },
{ B_TRANSLATE("Right selected") },
{ B_TRANSLATE("Right selected 2") },
{ B_TRANSLATE("Grid") },
{ B_TRANSLATE("Grid selected") },
{ B_TRANSLATE("Peak lines") },
{ B_TRANSLATE("Peak lines selected") },
{ B_TRANSLATE("Middle left") },
{ B_TRANSLATE("Middle right") },
{ B_TRANSLATE("Middle left selected") },
{ B_TRANSLATE("Middle right selected") },
{ B_TRANSLATE("Pointer") },
{ B_TRANSLATE("Timeline background") },
{ B_TRANSLATE("Timeline marks") },
{ B_TRANSLATE("Timeline small marks") },
{ B_TRANSLATE("Timeline text") },
{ NULL }
};


/*******************************************************
*   Setup the main view. Add in all the niffty components
*   we have made and get things rolling
*******************************************************/
PrefColors::PrefColors()
	:
	BView(BRect(0,0,490,380), "Prefs color", B_FOLLOW_ALL, B_WILL_DRAW)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	colors[0] = (void*)&Prefs.back_color;
	colors[1] = (void*)&Prefs.back_color2;
	colors[2] = (void*)&Prefs.back_selected_color;
	colors[3] = (void*)&Prefs.back_selected_color2;

	colors[4] = (void*)&Prefs.index_back_color;
	colors[5] = (void*)&Prefs.index_back_color2;
	colors[6] = (void*)&Prefs.index_mid_color;
	colors[7] = (void*)&Prefs.index_left_color;
	colors[8] = (void*)&Prefs.index_left_color2;
	colors[9] = (void*)&Prefs.index_back_selected_color;
	colors[10] = (void*)&Prefs.index_back_selected_color2;
	colors[11] = (void*)&Prefs.index_left_selected_color;
	colors[12] = (void*)&Prefs.index_left_selected_color2;
	colors[13] = (void*)&Prefs.index_mid_selected_color;
	colors[14] = (void*)&Prefs.index_pointer_color;

	colors[15] = (void*)&Prefs.left_color;
	colors[16] = (void*)&Prefs.left_color2;
	colors[17] = (void*)&Prefs.left_selected_color;
	colors[18] = (void*)&Prefs.left_selected_color2;

	colors[19] = (void*)&Prefs.right_color;
	colors[20] = (void*)&Prefs.right_color2;
	colors[21] = (void*)&Prefs.right_selected_color;
	colors[22] = (void*)&Prefs.right_selected_color2;

	colors[23] = (void*)&Prefs.grid_color;
	colors[24] = (void*)&Prefs.grid_selected_color;
	colors[25] = (void*)&Prefs.peak_color;
	colors[26] = (void*)&Prefs.peak_selected_color;
	colors[27] = (void*)&Prefs.mid_left_color;
	colors[28] = (void*)&Prefs.mid_right_color;
	colors[29] = (void*)&Prefs.mid_left_selected_color;
	colors[30] = (void*)&Prefs.mid_right_selected_color;
	colors[31] = (void*)&Prefs.pointer_color;
	colors[32] = (void*)&Prefs.time_back_color;
	colors[33] = (void*)&Prefs.time_marks_color;
	colors[34] = (void*)&Prefs.time_small_marks_color;
	colors[35] = (void*)&Prefs.time_text_color;

	color_view = new SwatchView("color_view", new BMessage(SWATCH_DROP));
	color_view->SetEnabled(false);
	color_view->SetExplicitSize(BSize(35, 35));

	scheme = new BPopUpMenu(B_TRANSLATE("Color Scheme"));
	BMenuItem* menuItem;
	BMenuField *menu = new BMenuField("menu",B_TRANSLATE("Color Scheme"),scheme);
	BMessage* m;

	for (int i = 0; gColorSchemes[i].schemeString != NULL; i++) {
		m = new BMessage(NEW_SCHEME);
		m->AddInt32("scheme",i);
		scheme->AddItem(
			menuItem = new BMenuItem(gColorSchemes[i].schemeString, m));

		if (i==0)
			menuItem->SetMarked(true);
	}

	menu->SetDivider(be_plain_font->StringWidth(B_TRANSLATE("Color Scheme"))+10);

	// add the prefs list at the left
	list = new BListView("color list");

	BScrollView *sv = new BScrollView("scroll", list, B_FOLLOW_ALL_SIDES,
		false, true, B_PLAIN_BORDER);

	sv->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	sv->MakeFocus(false);

	for (int i = 0; gColorsNames[i].colorString != NULL; i++)
		list->AddItem(new StringItem(gColorsNames[i].colorString));

	control = new BColorControl(BPoint(8, 16), B_CELLS_32x8, 1, "colorControl",
		new BMessage(COLOR_CHANGE));

	control->SetEnabled(false);

	const float spacing = be_control_look->DefaultItemSpacing();
	SetLayout(new BGroupLayout(B_VERTICAL, spacing));
	AddChild(BGroupLayoutBuilder(B_VERTICAL)
		.AddGroup(B_HORIZONTAL, spacing)
			.Add(menu)
			.Add(color_view)
		.End()
		.Add(sv)
		.Add(control)
		.AddGlue()
	.End()
	.SetInsets(spacing, spacing, spacing, spacing));
}


PrefColors::~PrefColors()
{
}


void
PrefColors::AttachedToWindow()
{
	list->SetTarget(this);
	list->SetSelectionMessage(new BMessage(COLOR_SELECT));
	list->SetInvocationMessage(new BMessage(COLOR_SELECT));
	control->SetTarget(this);
	color_view->SetTarget(this);
	scheme->SetTargetForItems(this);
}


void
PrefColors::MessageReceived(BMessage *msg)
{
	int32 i;
	rgb_color c, *col;

	switch(msg->what){
	case COLOR_SELECT:
		i = list->CurrentSelection();
		if(i < 0){
			control->SetEnabled(false);
			color_view->SetEnabled(false);
			Pool.sample_view_dirty = true;	// update the sample-view
			Pool.update_draw_cache = true;	// update the draw cache
			Pool.update_index = true;		// update the index cache
			Pool.RedrawWindow();
			break; // nothign selected 
		}
		control->SetEnabled(true);
		color_view->SetEnabled(true);
		c = *((rgb_color*)colors[i]);
		c.alpha = 255;
		control->SetValue(c);
		color_view->SetColor(c);
		Pool.sample_view_dirty = true;	// update the sample-view
		Pool.update_draw_cache = true;	// update the draw cache
		Pool.update_index = true;		// update the index cache
		Pool.RedrawWindow();
		break;

	case SWATCH_DROP:
		i = list->CurrentSelection();
		if(i < 0){
			control->SetEnabled(false);
			color_view->SetEnabled(false);
			break; // nothign selected 
		}
		control->SetEnabled(true);
		color_view->SetEnabled(true);
		c = color_view->Color();
		c.alpha = 255;
		col = (rgb_color*)colors[i];
		col->red = c.red;
		col->green = c.green;
		col->blue = c.blue;
		col->alpha = c.alpha;
		control->SetValue(c);
		Pool.sample_view_dirty = true;	// update the sample-view
		Pool.update_draw_cache = true;	// update the draw cache
		Pool.update_index = true;		// update the index cache
		Pool.RedrawWindow();
		break;
		
	case COLOR_CHANGE:
		i = list->CurrentSelection();
		if(i < 0){
			control->SetEnabled(false);
			color_view->SetEnabled(false);
			break; // nothign selected 
		}
		control->SetEnabled(true);
		color_view->SetEnabled(true);
		c = control->ValueAsColor();
		c.alpha = 255;
		col = (rgb_color*)colors[i];
		col->red = c.red;
		col->green = c.green;
		col->blue = c.blue;
		col->alpha = c.alpha;
		color_view->SetColor(c);
		Pool.sample_view_dirty = true;	// update the sample-view
		Pool.update_draw_cache = true;	// update the draw cache
		Pool.update_index = true;		// update the index cache
		Pool.RedrawWindow();
		break;

	case NEW_SCHEME:
		msg->FindInt32("scheme",&i);
		Prefs.SetColorScheme(i);
		Window()->PostMessage(COLOR_SELECT, this);
		break;

	default:
		BView::MessageReceived(msg);
		break;   
	}
}
