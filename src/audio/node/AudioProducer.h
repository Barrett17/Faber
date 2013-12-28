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


class AudioProducer : public MediaNode, BBufferProducer {
public:

					AudioProducer(const char* name,
						const media_format format);
	virtual 		~AudioProducer();

protected:

	virtual 	void 		NodeRegistered();

	virtual		BMediaAddOn*	AddOn(int32 *id) const;
	virtual		status_t 	HandleMessage(int32 code, const void *data,
								size_t size);
	virtual 	status_t 	FormatSuggestionRequested(media_type type,
									int32 quality, media_format *format);
	virtual 	status_t 	FormatProposal(const media_source &src,
									media_format *format);
	virtual 	status_t 	FormatChangeRequested(const media_source &src,
									const media_destination &dst,
									media_format *format,
									int32* _deprecated_);
	virtual 	void 		LateNoticeReceived(const media_source &src,
									bigtime_t late,	bigtime_t when);
	virtual 	status_t	GetNextOutput(int32 *cookie, media_output *output);
	virtual 	status_t 	DisposeOutputCookie(int32 cookie);
	virtual 	status_t	SetBufferGroup(const media_source &src,
									BBufferGroup *group);
	virtual 	status_t 	PrepareToConnect(const media_source &src,
									const media_destination &dst,
									media_format *format,
									media_source *out_source,
									char *name);
	virtual 	void 		Connect(status_t status,
									const media_source &src,
									const media_destination &dst,
									const media_format &format,
									char* name);
	virtual		void 		Disconnect(const media_source &src,
									const media_destination &dst);		
	virtual 	void 		EnableOutput(const media_source &src,
									bool enabled, int32* _deprecated_);
	virtual 	status_t 	GetLatency(bigtime_t *outLatency);
	virtual 	void 		LatencyChanged(	const media_source &src,
									const media_destination &dst,
									bigtime_t latency, uint32 flags);

			void		ProducerDataStatus(const media_destination &dst,
									int32 status, bigtime_t when);
};
