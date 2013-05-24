/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef WINDOWS_MANAGER
#define WINDOWS_MANAGER

#include <Messenger.h>

#include "Analyzers.h"
#include "FaberWindow.h"
#include "OpenPanel.h"
#include "ProgressWindow.h"
#include "SavePanel.h"
#include "SettingsWindow.h"


class WindowsManager {
public:
							WindowsManager();
							~WindowsManager();

	static WindowsManager*	Get();

	// It's preferable to use this one instead of
	// the method returning a pointer.
	static BMessenger*		MainWinMessenger();

	static FaberWindow*		MainWindow();

	// This is used only by the BApplication
	// to set the main window. Don't use it.
	FaberWindow*			IstantiateMainWindow(BRect rect);

	// TODO possible static implementation of BWindow::RedrawWindow()
	// something calling the BWindow version :
	// static void			RedrawWindow();

	// Extending, it could be good to have methods like those
	// static void			UpdateMainMenu();
	// static void			UpdateToolBar();
	// static void			UpdateInfoToolBar();
	// static void			MessageToMainWin(const uint32 msg);
	// static void			MessageToMainWin(BMessage* msg);

	// Continuing this way, would be cool to clean the code
	// by using a function to create alerts, this should
	// avoid duplication of "new BAlert(..)"*, so imagine something like :

	// static void			ShowAlert(const char* msg,
	//						const char* butt1, const char* butt2);

	void					ShowSettings();
	SettingsWindow*			GetSettingsWindow();

	static void				ShowAbout();

	// App panels
	static OpenPanel*		GetOpenPanel();
	static SavePanel*		GetSavePanel();

	// App tool windows
	static void				ShowSpectrumWindow();
	static void				ShowSampleScopeWindow();
	static void				ShowResampleWindow();
	static void				ShowFrequencyWindow();

	// Progress window methods
	// Maybe makes sense to move them in another class.
	// Or implement as static in the ProgressWindow.
	void					ProgressUpdate(int32 delta);
	void					StartProgress(const char *label,
								int32 max = 100);
	void					HideProgress();
	void					SetProgressName(const char *name);

	// Util method which you can use to see if 
	// a window is already opened / valid or not.
	static bool				IsWindowValid(BWindow* window);

private:
	static BPoint			_CalculateWindowPoint();

	static	WindowsManager*	fInstance;

	FaberWindow*			fMainWindow;
	SettingsWindow*			fSettingsWindow;
	ProgressWindow*			fProgress;

	SpectrumWindow*			fSpectrumWindow;
	SampleScopeWindow*		fSampleScopeWindow;

	OpenPanel*				fOpenPanel;
	SavePanel*				fSavePanel;
};

#endif
