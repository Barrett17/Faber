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

#ifndef MEDIA_END_POINT
#define MEDIA_END_POINT

#include <Buffer.h>
#include <BufferGroup.h>
#include <MediaDefs.h>
#include <MediaNode.h>
#include <ObjectList.h>

enum MediaEndPointKind {
	MEDIA_NO_KIND = 0,
	MEDIA_INPUT = 1,
	MEDIA_OUTPUT = 2
};


class MediaEndPoint
{
public:

									MediaEndPoint(int32 id,
										const char* name = NULL,
										MediaEndPointKind kind
											= MEDIA_NO_KIND);

	virtual							~MediaEndPoint();

			const int32				ID() const;

			const char*				Name() const;

			const media_node&		Node() const;

			void					SetOutput(media_output* out);
			media_output			GetOutput() const;

			void					SetInput(media_input* input);
			media_input				GetInput() const;

			bool					IsInput() const;
			bool					IsOutput() const;

			void					SetSource(media_source& source);
			const media_source&		Source() const;

			void					SetDestination(media_destination& dest);
			const media_destination& Destination() const;

			void					SetFormat(media_format* format);
			const media_format&		Format() const;

			void					SetDataStatus(int32 status);
			int32					DataStatus() const;

			void					SetOutputEnabled(bool enabled);
			bool					OutputEnabled() const;

			void					SetBufferGroup(BBufferGroup* buffers,
										bool own);

			const BBufferGroup*		Buffers() const;

			void					SetNextBuffer(BBuffer* buffer);
			BBuffer*				NextBuffer() const;

			void					SetLatencies(bigtime_t* process,
										bigtime_t* downstream);

			bool					BuffersOwned() const;

			bigtime_t				ProcessLatency() const;
			bigtime_t				DownstreamLatency() const;

			void					GetLatencies(bigtime_t* process,
										bigtime_t* downstream);
private:
			const char*				fName;
			int32					fID;

			media_node				fNode;
			media_format			fFormat;

			media_output			fOutput;
			media_input				fInput;

			media_source			fMediaSource;
			media_destination		fMediaDestination;

			int32					fDataStatus;

			bool					fOutputEnabled;
			bool					fOwnedBuffers;

			BBufferGroup*			fBufferGroup;
			BBuffer*				fNextBuffer;

			bigtime_t				fProcessLatency;
			bigtime_t				fDownstreamLatency;

			MediaEndPointKind		fEndPointKind;
};

// TODO just a temporary solution, replace (maybe composite?) with HashMap.
typedef BObjectList<MediaEndPoint> MediaEndPointMap;

#endif
