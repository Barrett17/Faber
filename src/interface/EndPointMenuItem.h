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
#ifndef _MEDIAENDPOINT_MENU_ITEM_H_
#define _MEDIAENDPOINT_MENU_ITEM_H_


#include <MediaDefs.h>


class MediaEndPointMenuItem : public BMenuItem {
public:
					// TODO at some point it should take a BMediaEndPoint
					MediaEndPointMenuItem(media_input input);
					MediaEndPointMenuItem(media_output output);

	virtual			~MediaEndPointMenuItem();

			bool	IsInput() const;
			bool	IsOutput() const;

private:
	bool			fIsInput;

	media_input		fInput;
	media_output	fOutput;
};

#endif
