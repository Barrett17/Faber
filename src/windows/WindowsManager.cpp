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

#include <Alert.h>
#include <Application.h>

#include "AboutWindow.h"
#include "MediaAgent.h"
#include "FaberDefs.h"

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


FaberWindow* WindowsManager::fMainWindow = NULL;
SettingsWindow* WindowsManager::fSettingsWindow = NULL;
ProgressWindow* WindowsManager::fProgress = NULL;

BFilePanel*	WindowsManager::fOpenPanel = NULL;
BFilePanel* WindowsManager::fSavePanel = NULL;
ExportWindow* WindowsManager::fExportWindow = NULL;

ParameterWindow* WindowsManager::fFaberMixer = NULL;
ParameterWindow* WindowsManager::fSystemMixer = NULL;


FaberWindow*
WindowsManager::MainWindow()
{
	if (fMainWindow == NULL) {
		BRect rect(50, 50, WINDOW_DEFAULT_SIZE_X, WINDOW_DEFAULT_SIZE_Y);
		fMainWindow = new FaberWindow(rect);
	}
	return fMainWindow;
}


void
WindowsManager::PostMessage(BMessage* message, BHandler* handler)
{
	if (fMainWindow != NULL)
		fMainWindow->PostMessage(message, handler);
}


void
WindowsManager::ShowSettings()
{
	if (fSettingsWindow == NULL ||
		!IsWindowValid(fSettingsWindow))
		fSettingsWindow = new SettingsWindow();

	if (fSettingsWindow->LockLooper()) {

		if (fSettingsWindow->IsHidden())
			fSettingsWindow->Show();

		fSettingsWindow->Activate();
		fSettingsWindow->UnlockLooper();
	}
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
	if (fOpenPanel == NULL) {
		fOpenPanel = new BFilePanel(B_OPEN_PANEL, new BMessenger(be_app),
			NULL, true, new BMessage(FABER_FILE_OPEN));

		fOpenPanel->Window()->SetTitle(B_TRANSLATE("Open file(s)"));	
	}
	return fOpenPanel;
}


BFilePanel*
WindowsManager::GetSavePanel()
{
	if (fSavePanel == NULL) {

		fSavePanel = new BFilePanel(B_SAVE_PANEL,
			new BMessenger(fMainWindow),
			NULL, false, new BMessage(B_SAVE_REQUESTED));

		fSavePanel->Window()->SetTitle(B_TRANSLATE("Save Project"));
	}

	return fSavePanel;
}


ExportWindow*
WindowsManager::GetExportPanel()
{
	if (!IsWindowValid(fExportWindow))
		fExportWindow = new ExportWindow();


	return fExportWindow;
}


ParameterWindow*
WindowsManager::GetSystemMixer()
{
	if (!IsWindowValid(fSystemMixer)) {
		media_node node;
		live_node_info info;

		BMediaRoster::Roster()->GetAudioMixer(&node);
		info.node = node;

		strcpy(info.name, B_TRANSLATE("System Mixer"));
		fSystemMixer = new ParameterWindow(
			_CalculateWindowPoint(), info);
	}

	return fSystemMixer;
}


ParameterWindow*
WindowsManager::GetFaberMixer()
{
	if (!IsWindowValid(fFaberMixer)) {

		live_node_info info;

		info.node = MediaAgent::GetAudioGate()->Node();

		strcpy(info.name, B_TRANSLATE("Mixer"));

		fFaberMixer = new ParameterWindow(
			_CalculateWindowPoint(), info);
	}

	return fFaberMixer;
}


void
WindowsManager::StartProgress(const char* label, int32 max)
{
	if (fProgress == NULL)
		fProgress = new ProgressWindow();

	fProgress->Show();
	fProgress->SetTitle(label);
	fProgress->Start(MainWindow(), true);
}


void
WindowsManager::ProgressUpdate(float percent, const char* text)
{
	if (fProgress == NULL)
		fProgress = new ProgressWindow();

	BMessenger mess(fProgress);
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
	if (fProgress == NULL)
		fProgress = new ProgressWindow();

	fProgress->Stop();
}


bool
WindowsManager::IsWindowValid(BWindow* window)
{
	BMessenger mess(window);
	return mess.IsValid();
}


void
WindowsManager::SimpleAlert(const char* text)
{
	BAlert* alert = new BAlert(B_TRANSLATE("Alert!"), text,
		"Ok", NULL, NULL,
		B_WIDTH_AS_USUAL, B_OFFSET_SPACING,
		B_WARNING_ALERT);

	alert->Go();
}


void
WindowsManager::SimpleError(const char* text)
{
	BString err(B_TRANSLATE("Error: "));
	err << text;
	SimpleAlert(err.String());
}


BPoint
WindowsManager::_CalculateWindowPoint()
{
	BRect frame = fMainWindow->Frame();
	return BPoint((frame.left + frame.right) / 2,
		(frame.top + frame.bottom) / 2);
}
