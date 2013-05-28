#ifndef _SOUND_CONSUMER_H
#define _SOUND_CONSUMER_H


class BSoundConsumer {
public:
			status_t			InitCheck();
			media_raw_audio_format Format() const;

			status_t			Start();
			void				Stop(bool block = true, bool flush = true);

			BufferPlayerFunc	BufferPlayer() const;
			void				SetBufferPlayer(void (*PlayBuffer)(void*,
									void* buffer, size_t size,
									const media_raw_audio_format& format));

			EventNotifierFunc	EventNotifier() const;
			void				SetNotifier(
									EventNotifierFunc eventNotifierFunction);
			void*				Cookie() const;
			void				SetCookie(void* cookie);
			void				SetCallbacks(
									BufferPlayerFunc playerFunction = NULL,
									EventNotifierFunc
										eventNotifierFunction = NULL,
									void* cookie = NULL);

	typedef int32 play_id;

			bigtime_t			CurrentTime();
			bigtime_t			PerformanceTime();
			status_t			Preroll();
			play_id				StartPlaying(BSound* sound,
									bigtime_t atTime = 0);
			play_id 			StartPlaying(BSound* sound,
									bigtime_t atTime,
									float withVolume);
			status_t 			SetSoundVolume(play_id sound, float newVolume);
			bool 				IsPlaying(play_id id);
			status_t 			StopPlaying(play_id id);
			status_t		 	WaitForSound(play_id id);

			// On [0..1]
			float 				Volume();
			void 				SetVolume(float volume);

			float				VolumeDB(bool forcePoll = false);
			void				SetVolumeDB(float dB);
			status_t			GetVolumeInfo(media_node* _node,
									int32* _parameterID, float* _minDB,
									float* _maxDB);
			bigtime_t			Latency();

	virtual	bool				HasData();
			void				SetHasData(bool hasData);

};

#endif
