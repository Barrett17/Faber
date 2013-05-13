/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Dario Casalinuovo, b.vitruvio@gmail.com
 */
#include "Settings.h"

#include <Entry.h>
#include <FindDirectory.h>
#include <new>

#include <Globals.h>

#include <stdio.h>

const uint32 kMsgSettings = 'stng';

/*
Settings* Settings::fInstance = NULL;

Settings* Settings::Instance()
{
	if(fInstance != NULL)
		return fInstance;

	fInstance = new Settings();
	return fInstance;
}*/

Settings::Settings(const char* path)
	:
	BMessage(kMsgSettings)
{
	status_t result = find_directory(B_USER_SETTINGS_DIRECTORY, &fSettingsPath);
	if (result == B_OK) {
		fSettingsPath.Append(path);
		fSettingsFile = new(std :: nothrow) BFile(fSettingsPath.Path(),
			B_READ_WRITE | B_CREATE_FILE);
	} else {
		printf("Error with settings\n");
		// BAlert ERROR!
	}
	OpenSettings();
}


Settings::~Settings()
{
	FlattenSettings();
	delete fSettingsFile;
}


status_t
Settings::InitCheck() const
{
	return fError;	
}


status_t
Settings::OpenSettings()
{
	printf("open\n");
	fError = fSettingsFile->InitCheck();

	if (fError < B_OK) {
		printf("Settings: initchek error %s\n", strerror(fError));
		return fError;	
	}

	fError = Unflatten(fSettingsFile);

	if (fError < B_OK) {
		printf("Settings: unflatten error %s\n", strerror(fError));
		// it seems the file is empty or corrupt, go with default values.
		return fError;
	}

	_SetTo();
	return B_OK;	
}


status_t
Settings::DeleteSettings()
{
	delete fSettingsFile;
	BEntry* entry = new BEntry(fSettingsPath.Path(), false);
	printf("Settings::DeleteSettings() the entry is %s\n",
		fSettingsPath.Path());

	status_t ret = entry->Remove();
	if (ret < B_OK)
		return ret;

	delete entry;
	return B_OK;
}


status_t
Settings::FlattenSettings()
{
	printf("flatten\n");
	_CheckSettings();

	//PrintToStream();
	fSettingsFile->Seek(0, SEEK_SET);
	return Flatten(fSettingsFile);
}

#define COLOR_TYPE 'clrt'

status_t
Settings::WriteColor(const char* name, rgb_color setting)
{
	if (HasData(name, COLOR_TYPE))
		return ReplaceData(name, COLOR_TYPE, &setting, sizeof(rgb_color));
	else
		return AddData(name, COLOR_TYPE, &setting, sizeof(rgb_color), true);
}

status_t
Settings::ReadColor(const char* name, rgb_color* setting)
{
	ssize_t size;
	return FindData(name, COLOR_TYPE, (const void**)&setting,
		&size);
}


status_t
Settings::RemoveSetting(const char* name)
{
	return RemoveData(name);
}


void
Settings::_CheckSettings()
{
	//MakeEmpty();
}


void
Settings::_SetTo()
{
}
