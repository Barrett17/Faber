
#include <MediaFile.h>

class WavePeak
{
								WavePeak(AudioTrack* track);

	BObjectList<BPositionIO*>&	PreviewRequested();

	void						UpdateRequested(int64 start, int64 end);

private:
	void						_RenderPeaks(int32 channel = -1;
									int64 start = 0, int64 end = 0);

	AudioTrack*					fTrack;

	BObjectList<BMallocIO*>		fPreview;
}
