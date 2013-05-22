/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include "WindowsManager.h"

#include "AboutBox.h"
#include "FreqWindow.h"
#include "Filters.h"
#include "FilterDialogs.h"


WindowsManager*	WindowsManager::fInstance = NULL;


WindowsManager::WindowsManager()
	:
	fSampleScopeWindow(NULL),
	fSpectrumWindow(NULL),
	fSavePanel(NULL),
	fOpenPanel(NULL)
{
	fProgress = new ProgressWindow(BRect(0,0,300,30));
	fSettingsWindow = NULL;
}


WindowsManager::~WindowsManager() 
{
	if (fProgress && fProgress->Lock())  {
		fProgress->Quit();
		fProgress->Unlock();
	}

	if (fSettingsWindow && fSettingsWindow->Lock()) {
		fSettingsWindow->Quit();
		fSettingsWindow->Unlock();
	}

	if (fOpenPanel)
		delete fOpenPanel;

	if (fSavePanel)
		delete fSavePanel;
}


WindowsManager*
WindowsManager::Get()
{
	if (fInstance == NULL)
		fInstance = new WindowsManager();

	return fInstance;	
}


FaberWindow*
WindowsManager::IstantiateMainWindow(BRect rect)
{
	fMainWindow = new FaberWindow(rect);
	return fMainWindow;
}



FaberWindow*
WindowsManager::MainWindow()
{
	return fMainWindow;
}


void
WindowsManager::ShowSettings()
{
	if (fSettingsWindow == NULL)
		GetSettingsWindow();

	if (fSettingsWindow->LockLooper()) {
		if (fSettingsWindow->IsHidden())
			fSettingsWindow->Show();
		fSettingsWindow->Activate();
		fSettingsWindow->UnlockLooper();
	}
}


SettingsWindow*
WindowsManager::GetSettingsWindow()
{
	if (fSettingsWindow == NULL)
		fSettingsWindow = new SettingsWindow();

	return fSettingsWindow;
}


void
WindowsManager::ShowAbout()
{
	BPoint point = _CalculateWindowPoint();

	(new AboutBox(point));
}


OpenPanel*
WindowsManager::GetOpenPanel()
{
	if (Get()->fOpenPanel == NULL)
		Get()->fOpenPanel = new OpenPanel(be_app);

	return Get()->fOpenPanel;
}


SavePanel*
WindowsManager::GetSavePanel()
{
	if (Get()->fSavePanel == NULL)
		Get()->fSavePanel = new SavePanel(be_app);	

	return Get()->fSavePanel;
}


void
WindowsManager::ShowSpectrumWindow()
{
	if (Get()->fSpectrumWindow == NULL)
		Get()->fSpectrumWindow = new SpectrumWindow();
	else
		Get()->fSpectrumWindow->Show();
}


void
WindowsManager::ShowSampleScopeWindow()
{
	if (Get()->fSampleScopeWindow == NULL)
		Get()->fSampleScopeWindow = new SampleScopeWindow();
	else
		Get()->fSampleScopeWindow->Show();
}


void
WindowsManager::ShowResampleWindow()
{
	BPoint point = _CalculateWindowPoint();

	(new ResampleWindow(point));
}


void
WindowsManager::ShowFrequencyWindow()
{
	BPoint point = _CalculateWindowPoint();

	(new FreqWindow(point));
}


// Those progress methods were created originally by Frans van Nispen
void WindowsManager::StartProgress(const char *label, int32 max)
{
	if (fProgress->Lock()){
		fProgress->StartProgress(label, max);
		fProgress->Unlock();
	}
}


void WindowsManager::ProgressUpdate(int32 delta)
{
	if (fProgress->Lock()){
		fProgress->SetProgress(delta);
		fProgress->Unlock();
	}
}


void WindowsManager::SetProgressName(const char *name)
{
	if (fProgress->Lock()){
		fProgress->SetTitle(name);
		fProgress->Unlock();
	}
}


void WindowsManager::HideProgress()
{
	if (fProgress->Lock()){
		if (!fProgress->IsHidden())
			fProgress->Hide();
		fProgress->Unlock();
	}
}
// End of progress methods.


BPoint
WindowsManager::_CalculateWindowPoint()
{
	BRect frame = Get()->MainWindow()->Frame();
	return BPoint((frame.left + frame.right) / 2,
		(frame.top+frame.bottom) / 2);
}
