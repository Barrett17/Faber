/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef WINDOWS_MANAGER
#define WINDOWS_MANAGER

#include "ProgressWindow.h"
#include "SettingsWindow.h"

class WindowsManager {
public:
							WindowsManager();
							~WindowsManager();

	static WindowsManager*	Get();

	FaberWindow*			MainWindow();
	// this is used to set the window from FaberApp
	void					SetMainWindow(FaberWindow* win);

	void					ShowSettings();
	SettingsWindow*			GetSettingsWindow();

	// App tool windows
	status_t				ShowSpectrumWindow();
	status_t				ShowSampleScopeWindow();
	status_t				ShowResampleWindow();
	status_t				ShowFrequencyWindow();

	// Progress window methods
	void					ProgressUpdate(int32 delta);
	void					StartProgress(const char *label,
								int32 max = 100);
	void					HideProgress();
	void					SetProgressName(const char *name);

	
private:
	static	WindowsManager*	fInstance;

	FaberWindow*			fMainWindow;
	SettingsWindow*			fSettingsWindow;
	ProgressWindow*			fProgress;

};

#endif
