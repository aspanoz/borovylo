#pragma once

#include <json.hpp>

#include <WDL/swell/swell.h>
#include <WDL/wdlstring.h>
#include <reaper_plugin/reaper_plugin_functions.h>

midi_Output *CreateThreadedMIDIOutput(midi_Output *output);

nlohmann::json loadJSONConfigFile(const char *filename);

class BorovyloCSurf : public IReaperControlSurface
{
  WDL_String descspace;
  char configtmp[1024];

  nlohmann::json JSONConfig;
  std::string debug; //@TODO delete

  midi_Input *m_midiin0;
  void OnMIDIEvent0(MIDI_event_t *evt);

public:
  BorovyloCSurf();
  virtual ~BorovyloCSurf();

  void Run();

  void SetTrackListChange(); // on track list view change


  midi_Output *m_midiout0;


  const char *GetTypeString();
  const char *GetDescString();
  const char *GetConfigString();
};
