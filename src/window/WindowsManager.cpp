/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include "WindowsManager.h"


WindowsManager*	WindowsManager::fInstance = NULL;


WindowsManager::WindowsManager()
{
	fProgress = new ProgressWindow(BRect(0,0,300,30));
	fSettingsWindow = new SettingsWindow();
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
}


WindowsManager*
WindowsManager::Get()
{
	if (fInstance == NULL)
		fInstance = new WindowsManager();

	return fInstance;	
}


void
WindowsManager::SetMainWindow(FaberWindow* win)
{
	fMainWindow = win;
}



FaberWindow*
WindowsManager::MainWindow()
{
	return fMainWindow;
}


void
WindowsManager::ShowSettings()
{
	if (fSettingsWindow != NULL && fSettingsWindow->LockLooper()) {
		if (fSettingsWindow->IsHidden())
			fSettingsWindow->Show();
		fSettingsWindow->Activate();
		fSettingsWindow->UnlockLooper();
	}
}


SettingsWindow*
WindowsManager::GetSettingsWindow()
{
	return fSettingsWindow;
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
