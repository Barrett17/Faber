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

#include <MediaEventLooper.h>
#include <BufferConsumer.h>

#include "MediaNode.h"


class MediaRecorderNode : public MediaNode, public BBufferConsumer
{
public:
							MediaRecorderNode(
								const char* name,
								const media_format format,
								int32 priority);

			void			SetFormat(const media_format format);
			media_format&	Format() const;

			status_t		GetInput(media_input* out_input) const;

			void			SetDataEnabled(bool enabled);

protected:

	virtual					~MediaRecorderNode();

	// BMediaNode
	virtual void 			NodeRegistered();

	virtual	BMediaAddOn*	AddOn(int32* id) const;
	virtual	status_t 		HandleMessage(int32 code, const void* data,
								size_t size);

	// BBufferConsumer
	virtual void 			BufferReceived(BBuffer* buffer);

	virtual status_t 		AcceptFormat(const media_destination& dst,
								media_format* format);

	virtual status_t	 	GetNextInput(int32* cookie,	media_input* input);

	virtual void 			DisposeInputCookie(int32 cookie);

	virtual status_t 		FormatChanged(const media_source& src,
								const media_destination& dst,
								int32 change_tag,
								const media_format& format);

	virtual void 			ProducerDataStatus(const media_destination& dst,
								int32 status,
								bigtime_t when);

	virtual status_t	 	GetLatencyFor(const media_destination& dst,
								bigtime_t* latency,
								media_node_id* time_src);

	virtual status_t 		Connected(const media_source&src, 
								const media_destination& dst,
								const media_format& format,
								media_input* input);

	virtual void 			Disconnected(const media_source& src,
								const media_destination& where);

protected:
				BString		fName;
};

