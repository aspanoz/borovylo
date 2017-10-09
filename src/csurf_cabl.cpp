#include <sys/timeb.h>
#include "csurf_cabl.h"


extern REAPER_PLUGIN_HINSTANCE g_hInst;
extern HWND g_hwnd;


CSurf_Borovylo::CSurf_Borovylo()
{
  m_midiin0 = CreateMIDIInput(0);
  if (m_midiin0)
      m_midiin0->start();

  // m_midiout0 = CreateMIDIOutput(0, false, NULL);
}


CSurf_Borovylo::~CSurf_Borovylo()
{
  delete m_midiin0;
  delete m_midiout0;
}


void CSurf_Borovylo::Run()
{

/*
  if (m_midiin0)
  {
    m_midiin0->SwapBufs(timeGetTime());
    int l = 0;
    MIDI_eventlist *list = m_midiin0->GetReadBuf();
    MIDI_event_t *evts;
    while ((evts = list->EnumItems(&l))) OnMIDIEvent0(evts);
  }
*/

  ShowConsoleMsg("Run");
}


void CSurf_Borovylo::OnMIDIEvent0(MIDI_event_t *evt)
{
  // m_midiout0->SendMsg(evt, 0);
}


const char *CSurf_Borovylo::GetTypeString()
{
  return "BOROVYLO";
}


const char *CSurf_Borovylo::GetDescString()
{
  descspace.Set("Borovylo Surface Control");
  return descspace.Get();
}


const char *CSurf_Borovylo::GetConfigString()
{
  sprintf(configtmp,"0 0");
  return configtmp;
}
