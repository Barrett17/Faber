
class WaveView : public BView
{
public:
						WaveView(AudioTrack* track);	

	void				UpdateRequested();

private:
	void				_Render();
	void				_UpdatePeaks();

	WavePeaks*			fWavePeaks;
}
