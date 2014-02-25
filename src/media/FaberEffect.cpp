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

#include "FaberEffect.h"

#include "FaberDefs.h"


FaberEffect::FaberEffect(const char* name, uint32 flags)
	:
	fName(name),
	fFlags(flags)
{
}


FaberEffect::~FaberEffect()
{
}



const char*
FaberEffect::Name() const
{
	return fName;
}


BMenuItem*
FaberEffect::BuildItem()
{
	BMessage* mess = MessageBuilder(FABER_EFFECT_CALL);
	mess->AddPointer("effect", this);

	return new BMenuItem(Name(), mess, 0, 0);
}


int32
FaberEffect::Flags() const
{
	return fFlags;
}


status_t
FaberEffect::ArchiveSettings(BMessage* message)
{

	return B_OK;
}


status_t
FaberEffect::UnarchiveSettings(BMessage* message)
{

	return B_OK;
}
