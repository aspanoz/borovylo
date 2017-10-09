#pragma once

#include <WDL/swell/swell.h>
#include <WDL/wdlstring.h>

#include <reaper_plugin/reaper_plugin_functions.h>

midi_Output *CreateThreadedMIDIOutput(midi_Output *output);

class CSurf_Borovylo : public IReaperControlSurface
{
  WDL_String descspace;
  char configtmp[1024];

  midi_Input *m_midiin0;

  void OnMIDIEvent0(MIDI_event_t *evt);

public:
  CSurf_Borovylo();
  virtual ~CSurf_Borovylo();

  midi_Output *m_midiout0;

  void Run();

  const char *GetTypeString();

  const char *GetDescString();

  const char *GetConfigString();
};
