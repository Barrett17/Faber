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

#include <Application.h>

#include "AboutWindow.h"
#include "AudioGate.h"
#include "FaberDefs.h"


WindowsManager*	WindowsManager::fInstance = NULL;

const char* kCopyrights[] = {
	NULL
};

const char* kAuthors[] = {
	"Dario Casalinuovo",
	NULL
};

const char* kThanks[] = {
	"morpe",
	NULL
};

const char* kAdditionalCode[] {
	"BeAE - Effects and Shortcut view",
	"Libwalter - Spinner Widget",
	"Haiku - GUI Widgets and other support classes",
	NULL
};

WindowsManager::WindowsManager()
	:
	fSavePanel(NULL),
	fOpenPanel(NULL),
	fSettingsWindow(NULL),
	fMainWindow(NULL),
	fExportWindow(NULL),
	fFaberMixer(NULL),
	fSystemMixer(NULL)
{
	fProgress = new ProgressWindow();
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


BMessenger&
WindowsManager::MainWinMessenger()
{
	BMessenger* mess = new BMessenger(Get()->fMainWindow);
	return *mess;
}


FaberWindow*
WindowsManager::MainWindow()
{
	if (Get()->fMainWindow == NULL) {
		BRect rect(50, 50, WINDOW_DEFAULT_SIZE_X, WINDOW_DEFAULT_SIZE_Y);
		Get()->fMainWindow = new FaberWindow(rect);
	}
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
	BAboutWindow* box = new BAboutWindow(FABER_APP_NAME, FABER_SIGNATURE);

	box->AddDescription(FABER_DESCRIPTION);
	box->AddText("Faber is a Versut product, it's released under the terms of the GPL v3 license.", NULL);

	box->AddExtraInfo(FABER_HELP_HOMEPAGE);

	box->AddCopyright(2012, FABER_COPYRIGHT, kCopyrights);
	box->SetVersion(FABER_VERSION);
	box->AddAuthors(kAuthors);

	box->AddText("Faber contain code from :", kAdditionalCode);

	box->AddSpecialThanks(kThanks);

	box->Show();
	box->MoveTo(_CalculateWindowPoint());
}


BFilePanel*
WindowsManager::GetOpenPanel()
{
	if (Get()->fOpenPanel == NULL) {
		Get()->fOpenPanel = new BFilePanel(B_OPEN_PANEL, new BMessenger(be_app),
			NULL, true, new BMessage(FABER_FILE_OPEN));

		Get()->fOpenPanel->Window()->SetTitle(B_TRANSLATE("Open file(s)"));	
	}
	return Get()->fOpenPanel;
}


BFilePanel*
WindowsManager::GetSavePanel()
{
	if (Get()->fSavePanel == NULL) {

		Get()->fSavePanel = new BFilePanel(B_SAVE_PANEL, &WindowsManager::MainWinMessenger(),
			NULL, false, new BMessage(B_SAVE_REQUESTED));

		Get()->fSavePanel->Window()->SetTitle(B_TRANSLATE("Save Project"));
	}

	return Get()->fSavePanel;
}


ExportWindow*
WindowsManager::GetExportPanel()
{
	if (!IsWindowValid(Get()->fExportWindow))
		Get()->fExportWindow = new ExportWindow();


	return Get()->fExportWindow;
}


ParameterWindow*
WindowsManager::GetSystemMixer()
{
	if (!IsWindowValid(Get()->fSystemMixer)) {
		media_node node;
		live_node_info info;

		BMediaRoster::Roster()->GetAudioMixer(&node);
		info.node = node;

		strcpy(info.name, B_TRANSLATE("System Mixer"));
		Get()->fSystemMixer = new ParameterWindow(
			_CalculateWindowPoint(), info);
	}

	return Get()->fSystemMixer;
}


ParameterWindow*
WindowsManager::GetFaberMixer()
{
	if (!IsWindowValid(Get()->fFaberMixer)) {

		live_node_info info;

		info.node = AudioGate::Get()->Node();

		strcpy(info.name, B_TRANSLATE("Mixer"));

		Get()->fFaberMixer = new ParameterWindow(
			_CalculateWindowPoint(), info);
	}

	return Get()->fFaberMixer;
}


void
WindowsManager::StartProgress(const char* label, int32 max)
{
	Get()->fProgress->Show();
	Get()->fProgress->SetTitle(label);
	Get()->fProgress->Start(MainWindow(), true);
}


void
WindowsManager::ProgressUpdate(float percent, const char* text)
{
	BMessenger mess(Get()->fProgress);
	if (mess.IsValid()) {
		BMessage* msg = new BMessage(kMsgProgressUpdate);
		msg->AddString("message", text);
		msg->AddFloat("percent", percent);
		mess.SendMessage(msg);
	}
}


void
WindowsManager::HideProgress()
{
	Get()->fProgress->Stop();
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
	BRect frame = Get()->fMainWindow->Frame();
	return BPoint((frame.left + frame.right) / 2,
		(frame.top + frame.bottom) / 2);
}
