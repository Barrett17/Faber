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


			// Various methods for controlling
			// the toolbar behavior
			void			SetPlay(bool play);
			bool			IsPlay() const;

			void			SetPause(bool pause);
			bool			IsPause() const;

			void			SetRec(bool rec);
			bool			IsRec() const;

			void			SetLoop(bool loop);
			bool			IsLoop() const;

			void			SetSelectionTool(bool selTool);
			void			SetDrawingTool(bool selTool);
			void			SetPlayingTool(bool selTool);
			void			SetJoggWheelTool(bool selTool);

private:
			IconButton*		_BuildButton(const char* tip, BMessage* message,
								int32 resourceID);
};

#endif
