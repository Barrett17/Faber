/*
    Copyright 2013 Dario Casalinuovo. All rights reserved.

    This file is part of Faber.

    Faber is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Faber is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Faber.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef WINDOWS_MANAGER
#define WINDOWS_MANAGER

#include <FilePanel.h>
#include <Messenger.h>

#include "ExportWindow.h"
#include "FaberEffect.h"
#include "FaberWindow.h"
#include "ParameterWindow.h"
#include "ProgressWindow.h"
#include "SettingsWindow.h"


class WindowsManager {
public:
							WindowsManager();
							~WindowsManager();

	static WindowsManager*	Get();

	// It's preferable to use this one instead of
	// the method returning a pointer.
	static BMessenger&		MainWinMessenger();

	static FaberWindow*		MainWindow();

	// Extending, it could be good to have methods like those
	// static void			UpdateMainMenu();
	// static void			UpdateToolBar();
	// static void			UpdateInfoToolBar();

	// static void			MessageToMainWin(BMessage* msg);

	// Continuing this way, would be cool to clean the code
	// by using a function to create alerts, this should
	// avoid duplication of "new BAlert(..)"*, so imagine something like :

	// static int32			ShowAlert(const char* title, const char* text
	//							const char* butt1, const char* butt2 = NULL,
	//							const char* butt3 = NULL,
	//							button_width width = B_WIDTH_AS_USUAL,
	//							alert_type type = B_INFO_ALERT);

	void					ShowSettings();
	SettingsWindow*			GetSettingsWindow();

	static void				ShowAbout();

	// App panels
	static BFilePanel*		GetOpenPanel();
	static BFilePanel*		GetSavePanel();
	static ExportWindow*	GetExportPanel();

	static ParameterWindow*	GetFaberMixer();
	static ParameterWindow*	GetSystemMixer();

	// Progress window methods
	static void				ProgressUpdate(float percent, const char* text);

	static void				StartProgress(const char *label, int32 max = 100);
	static void				HideProgress();

	// Util method which you can use to see if 
	// a window is already opened / valid or not.
	static bool				IsWindowValid(BWindow* window);

private:
	static BPoint			_CalculateWindowPoint();

	static	WindowsManager*	fInstance;

	FaberWindow*			fMainWindow;
	SettingsWindow*			fSettingsWindow;
	ProgressWindow*			fProgress;

	BFilePanel*				fOpenPanel;
	BFilePanel*				fSavePanel;
	ExportWindow*			fExportWindow;

	ParameterWindow*		fFaberMixer;
	ParameterWindow*		fSystemMixer;
};

#endif
