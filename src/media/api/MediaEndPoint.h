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

// TODO define a way to check the type of the endpoint, eg audio/video


class MediaEndPoint
{
public:

							MediaEndPoint(int32 id, const char* name = NULL);
	virtual						~MediaEndPoint();

		const int32				ID() const;

		const char*				Name() const;

		const media_node&			Node() const;

		status_t				SetOutput(media_output* out);
		media_output				Output() const;

		status_t				SetInput(media_input* in);
		media_input				Input() const;

		status_t				SetSource(media_source& source);
		const media_source&			Source() const;

		status_t				SetDestination(media_destination& dest);
		const media_destination&		Destination() const;

		status_t				SetFormat(media_format* format);
		const media_format&			Format() const;

		void					SetDataStatus(int32 status);
		int32					DataStatus() const;

		void					SetOutputEnabled(bool enabled);
		bool					OutputEnabled() const;

		void					SetBufferGroup(BBufferGroup* buffers, bool own);
		const BBufferGroup*			Buffers() const;

		void					SetNextBuffer(BBuffer* buffer);
		BBuffer*				NextBuffer() const;

		void					SetLatencies(bigtime_t* process, bigtime_t* downstream);

		bool					BuffersOwned() const;

		bigtime_t				ProcessLatency() const;
		bigtime_t				DownstreamLatency() const;

		void					GetLatencies(bigtime_t* process, bigtime_t* downstream);

};