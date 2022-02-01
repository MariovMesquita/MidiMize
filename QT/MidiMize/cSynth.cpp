#include "cSynth.h"
#include <fluidsynth.h>

using namespace std;

cSynth::cSynth()
{
    /* Create settings object and pass init values */
    this->FsSettings = new_fluid_settings();
    fluid_settings_setnum(FsSettings, "synth.sample-rate", FS_SAMPLE_RATE);
    fluid_settings_setint(FsSettings, "synth.cpu-cores", FS_CPU_CORES);
    fluid_settings_setstr(FsSettings, "audio.driver", FS_AUDIO_DRIVER);
    fluid_settings_setstr(FsSettings, "midi.driver", FS_MIDI_DRIVER);

    /* Create synth instance */
    this->FsSynth = new_fluid_synth(this->FsSettings);

    /* Load soundfonts */
    /*SINE*/
    this->sFonts[0].name="sine";
    this->sFonts[0].fileName=SF_SINE_PATH;
    this->sFonts[0].id = fluid_synth_sfload(this->FsSynth, this->sFonts[0].fileName, 1);
    /*TRIANGLE*/
    this->sFonts[1].name="triangle";
    this->sFonts[1].fileName=SF_TRIANGLE_PATH;
    this->sFonts[1].id = fluid_synth_sfload(this->FsSynth, this->sFonts[0].fileName, 1);
    /*SAW*/
    this->sFonts[2].name="saw";
    this->sFonts[2].fileName=SF_SAW_PATH;
    this->sFonts[2].id = fluid_synth_sfload(this->FsSynth, this->sFonts[0].fileName, 1);

    /* Create Audio Driver */
    this->FsAudioDriver = new_fluid_audio_driver(this->FsSettings, this->FsSynth);

    /* Create Midi Driver */
    //this->FsMidiDriver = new_fluid_midi_driver(this->FsSettings, handle_midi_event, NULL);

}

/*
 * MIDI Note On
 * Sends note on (key) event to MIDI channel (chan)
 * ALL CHANNELS -> chan = -1
 * Key ->  0 - 127
 */
void cSynth::noteOn(int chan, int key, int vel)
{
    fluid_synth_noteon(this->FsSynth, chan, key, vel);
}

/*
 * MIDI Note OFF
 * Sends note off (key) to MIDI channel (chan)
 * ALL CHANNELS -> chan = -1
 * Key ->  0 - 127
 * Vel (velocity) -> 0-127 ( 0 = noteoff )
 */
void cSynth::noteOff(int chan, int key)
{
    fluid_synth_noteoff(this->FsSynth, chan, key);
}

/*
 * Turns reverb on or off
 * Sets reverb values
 * width    0.0 - 100.0
 * room     0.0 - 1.0
 * lvl      0.0 - 1.0
 * damp     0.0 - 1.0
 */
void cSynth::setReverb()
{
    fluid_synth_set_reverb(this->FsSynth, this->reverb.room, this->reverb.damp, this->reverb.width, this->reverb.lvl);

    if(this->reverb.active == 1)
    {
        fluid_synth_set_reverb_on(this->FsSynth, 1);
    }
    else
    {
        fluid_synth_set_reverb_on(this->FsSynth, 0);
    }
}

/*
 * Turns reverb on or off
 * Sets reverb values
 * nr       0 - 99
 * lvl      0.0 - 10.0
 * speed    0.1 - 5.0 Hz
 * depth    0.0 - 21.0
 * type     FLUID_CHORUS_MOD_SINE, FLUID_CHORUS_MOD_TRIANGLE
 */
void cSynth::setChorus()
{
    if(this->chorus.active == 1)
    {
        fluid_synth_set_chorus_on(this->FsSynth, 1);
    }
    else
    {
        fluid_synth_set_chorus_on(this->FsSynth, 0);
    }

    fluid_synth_set_chorus(this->FsSynth, this->chorus.nr, this->chorus.lvl, this->chorus.speed, this->chorus.depth, this->chorus.waveType);
}

/*
 * Sets synth gain
 * range 0.0 - 10.0
 */
void cSynth::setGain(float gain)
{
    this->gain=gain;
    fluid_synth_set_gain(this->FsSynth, this->gain);
}

/*
 * Sets synth gain
 * range 0.0 - 10.0
 * MIDI channel = -1  -> ALL CHANNELS
 */
void cSynth::setPitch()
{
    fluid_synth_pitch_bend(this->FsSynth, -1, this->pitchBend);
}

/*
 * Changes synth oscillator (soundFont)
 * reloads a soundFont loaded at synth initialization
 */
void cSynth::setOscillator(oscillator_t osc)
{   
    switch(osc)
    {
        case SINE:
            this->oscillator = SINE;
            fluid_synth_sfreload(this->FsSynth, this->sFonts[0].id);
            break;

        case TRIANGLE:
            this->oscillator = TRIANGLE;
            fluid_synth_sfreload(this->FsSynth, this->sFonts[1].id);
            break;

        case SAW:
            this->oscillator = SAW;
            fluid_synth_sfreload(this->FsSynth, this->sFonts[2].id);
            break;
    }
}

cSynth::~cSynth()
{
    /* Remove Audio Driver */
    delete_fluid_audio_driver(this->FsAudioDriver);

    /* Remove MIDI Driver */
    delete_fluid_midi_driver(this->FsMidiDriver);

    /* Remove synth settings */
    delete_fluid_settings(this->FsSettings);

    /* Delete synth instance */
    delete_fluid_synth(this->FsSynth);
}
