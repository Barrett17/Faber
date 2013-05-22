/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef WINDOWS_MANAGER
#define WINDOWS_MANAGER

#include "Analyzers.h"
#include "OpenPanel.h"
#include "ProgressWindow.h"
#include "SavePanel.h"
#include "SettingsWindow.h"


class WindowsManager {
public:
							WindowsManager();
							~WindowsManager();

	static WindowsManager*	Get();

	FaberWindow*			IstantiateMainWindow(BRect rect);
	FaberWindow*			MainWindow();
	// this is used to set the window from FaberApp

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
