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

#ifndef MEDIA_FORMAT_BUILDER_H
#define MEDIA_FORMAT_BUILDER_H

#include <MediaDefs.h>


class MediaFormatBuilder {
public:
	static void BuildAudioBlockRawFormat(media_format* format);

	static void BuildAudioBlockEncodedFormat(media_format* format);

	static void BuildAudioBlockFormat(media_format* format);

	static void BuildAudioGateFormat(media_format* format);
};

#endif
