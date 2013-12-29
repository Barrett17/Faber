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

#include "AudioFilter.h"


AudioFilter::AudioFilter(const char* name,
	const media_format format)
	:
	BMediaNode(name),
	AudioProducer(name, format),
	AudioConsumer(name, format)
{
	
}


AudioFilter::~AudioFilter()
{

}
/*
void
AudioFilter::NodeRegistered()
{
}


BMediaAddOn*
AudioFilter::AddOn(int32* id) const
{
	return NULL;
}

*/
status_t
AudioFilter::HandleMessage(int32 code, const void* data, size_t size)
{
	return B_OK;
}
