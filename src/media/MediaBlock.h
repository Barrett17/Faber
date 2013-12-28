

class MediaBlock
{

			bool			IsEmpty() const;
			void			MakeEmpty();

			status_t		ReadFrames(void* buffer, int64* frameCount);
			int64			CountFrames() const;

			status_t		WriteFrames(const void* data, int32 frameCount,
								int32 flags = 0);

			status_t		ReplaceFrames(const void* buffer,
								int64* frameCount);

			status_t		SeekToTime(bigtime_t* _time, int32 flags = 0);
			status_t		SeekToFrame(int64* _frame, int32 flags = 0);

			void			ReplaceWith(MediaBlock* block);

			status_t		DetachFromFile();

			status_t		DeleteFile();

private:
			int64			fSize;

			int64			fStart;
			int64			fEnd;

			void*			fBuffer;

			BMediaFile*		fFile;
			BMediaTrack*	fTrack;
};


class EmptySpaceBlock : public MediaBlock
{

};
