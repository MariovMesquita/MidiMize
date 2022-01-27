#ifndef CSYNTH_H
#define CSYNTH_H

#include <pthread.h>
#include <fluidsynth.h>
#include <string>

using namespace std;

#define FS_SAMPLE_RATE 44100.0
#define FS_CPU_CORES 2
#define FS_AUDIO_DRIVER "alsa"
#define FS_MIDI_DRIVER "alsa_seq"

//struct init_settings
//{
//    float fs_sampleRate = 44100.0;
//    float fs_cpuCores = 2;
//    string fs_audioDriver = "alsa";
//    string fs_midiDriver = "alsa_seq";
//    string fs_soundFonts[3] = {"/etc/MIDImize/soundFonts/sine.sf2" , "/etc/MIDImize/soundFonts/saw.sf2" , "/etc/MIDImize/soundFonts/triangle.sf2"};
//};

enum oscillator_t
{
    SINE,
    SAW,
    TRIANGLE,
};

struct chorus_settings_t
{

    float active;
    float speed;
    int nr;
    float lvl;
    float depth;
    float waveType;
};

struct reverb_settings_t
{
    bool active;
    float width; // 0.0 - 100.0
    float room;  // 0.0 - 1.0
    float lvl;   // 0.0 - 1.0
    float damp;  // 0.0 - 1.0
};

class cSynth
{
    private:
        fluid_audio_driver_t* FsAudioDriver;
        fluid_midi_driver_t* FsMidiDriver;
        fluid_settings_t* FsSettings;
        fluid_synth_t* FsSynth;

        bool synthOn;
        int synthID;


        chorus_settings_t chorus;
        reverb_settings_t reverb;
        oscillator_t oscillator;
        float gain;
        int pitchBend;

    public:
        cSynth();
        ~cSynth();

        void noteOn(int chan, int key, int vel);
        void noteOff(int chan, int key);
        void setReverb();
        void setChorus();
        void setOscillator(chorus_settings_t chorus);
        void setGain();
        void setPitch();

};

#endif // CSYNTH_H
