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

#include "WindowsManager.h"

#include "FreqWindow.h"
#include "Filters.h"
#include "FilterDialogs.h"


WindowsManager*	WindowsManager::fInstance = NULL;


WindowsManager::WindowsManager()
	:
	fSavePanel(NULL),
	fOpenPanel(NULL),
	fSettingsWindow(NULL)
{
	fProgress = new ProgressWindow(BRect(0, 0, 300, 40));
}


WindowsManager::~WindowsManager() 
{
	if (fProgress && fProgress->Lock()) {
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


// TODO this should not return a pointer
BMessenger*
WindowsManager::MainWinMessenger()
{
	BMessenger* mess = new BMessenger(Get()->fMainWindow);
	return mess;
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
	return Get()->fMainWindow;
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
	/*BPoint point = _CalculateWindowPoint();

	AboutBox* box = new AboutBox();
	*/
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
WindowsManager::ShowResampleWindow()
{
	/*BPoint point = _CalculateWindowPoint();

	(new ResampleWindow(point));*/
}


void
WindowsManager::ShowFrequencyWindow()
{
	/*BPoint point = _CalculateWindowPoint();

	(new FreqWindow(point));*/
}


void WindowsManager::StartProgress(const char *label, int32 max)
{
	fProgress->StartProgress(label, max);
}


void WindowsManager::ProgressUpdate(int32 delta)
{
	fProgress->SetProgress(delta);
}


void WindowsManager::SetProgressName(const char *name)
{
	fProgress->SetProgressName(name);
}


void WindowsManager::HideProgress()
{
	fProgress->HideProgress();
}


bool
WindowsManager::IsWindowValid(BWindow* window)
{
	BMessenger mess(window);
	return mess.IsValid();
}


BPoint
WindowsManager::_CalculateWindowPoint()
{
	BRect frame = Get()->MainWindow()->Frame();
	return BPoint((frame.left + frame.right) / 2,
		(frame.top + frame.bottom) / 2);
}
