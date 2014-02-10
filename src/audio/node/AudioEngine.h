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

#ifndef _AUDIO_ENGINE_H
#define _AUDIO_ENGINE_H

#include <BufferConsumer.h>
#include <BufferProducer.h>
#include <MediaEventLooper.h>
#include <TimeSource.h>

#include "MediaEndPoint.h"

class AudioGate;


class AudioEngine : public BBufferProducer,
	BBufferConsumer, BMediaEventLooper {
public:

							AudioEngine(AudioGate* audioGate);

protected:

	virtual 				~AudioEngine();

	// BMediaEventLooper

	virtual void			HandleEvent(const media_timed_event *event,
								bigtime_t late, bool realTimeEvent);

	// BMediaNode

	virtual BMediaAddOn*	AddOn(int32* id) const;

	virtual void			NodeRegistered();

	virtual status_t		HandleMessage(int32, const void*, size_t);

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

	// BBufferProducer

	virtual	status_t	 	FormatSuggestionRequested(media_type type,
									int32 quality, media_format* format);

	virtual status_t 		FormatProposal(const media_source& src,
									media_format* format);

	virtual status_t 		FormatChangeRequested(const media_source& src,
								const media_destination& dst,
								media_format* format,
								int32* _deprecated_);

	virtual	void 			LateNoticeReceived(const media_source& src,
								bigtime_t late,	bigtime_t when);

	virtual status_t		GetNextOutput(int32* cookie, media_output* output);

	virtual status_t	 	DisposeOutputCookie(int32 cookie);
	
	virtual status_t		SetBufferGroup(const media_source& src,
								BBufferGroup* group);

	virtual status_t	 	PrepareToConnect(const media_source& src,
								const media_destination& dst,
								media_format* format,
								media_source* out_source,
								char* name);

	virtual void 			Connect(status_t status,
								const media_source& src,
								const media_destination& dst,
								const media_format& format,
								char* name);

	virtual	void 			Disconnect(const media_source& src,
								const media_destination& dst);		

	virtual void 			EnableOutput(const media_source& src,
								bool enabled, int32* _deprecated_);

	virtual status_t 		GetLatency(bigtime_t* outLatency);

	virtual void 			LatencyChanged(	const media_source& src,
								const media_destination& dst,
								bigtime_t latency, uint32 flags);
private:
			media_format	fFormat;

			bigtime_t		fDownstreamLatency;
			bigtime_t		fProcessLatency;
			bigtime_t		fLastLateness;

			bigtime_t		fTime;
			size_t			fFramesSent;

			BBufferGroup*	fBufferGroup;

			AudioGate*		fAudioGate;
};

#endif
