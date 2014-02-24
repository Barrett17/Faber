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

#ifndef FABER_SETTINGS_H
#define FABER_SETTINGS_H

#include <File.h>
#include <InterfaceDefs.h>
#include <Message.h>
#include <Path.h>
#include <String.h>


class Settings : public BMessage {
public:
						Settings(const char* path);
virtual					~Settings();

		status_t		InitCheck() const;

		status_t		ReadColor(const char* name, rgb_color* color);
		status_t		WriteColor(const char* name, rgb_color color);
		
		status_t		RemoveSetting(const char* name);

private:
		status_t		OpenSettings();
		status_t		FlattenSettings();
		status_t		DeleteSettings();

		BFile*			fSettingsFile;
		BPath			fSettingsPath;

		int32			fControl;

		status_t		fError;
}; 

#endif /* SETTINGS_H */
