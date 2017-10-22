#include <sys/timeb.h>

#include <fplus/fplus.hpp>
#include <json.hpp>

#include "csurf_cabl.h"

#define CONFIG "/Scripts/Aspanoz/package.json"

extern REAPER_PLUGIN_HINSTANCE g_hInst;
extern HWND g_hwnd;


auto reaperConsoleMessage = [](std::string str){
  ShowConsoleMsg(
    str.append("\n").c_str()
  ); 
};


nlohmann::json loadJSONConfig(const char *filename)
{
  // @TODO: fix paths seperator for windows
  // @TODO: check is file return not empty string
  // @TODO: check is json parse without errors
  // @TODO: add unit tests

  auto curredStringAppend = [](auto *c){
    return [=](auto str){
      return std::string(str).append(c);
    };
  };

  return fplus::fwd::apply(
    GetResourcePath(),             // reaper function. path to reaper resource
    curredStringAppend(filename),  // create absolute config file path
    [](auto path){
      return fplus::read_text_file(path)();
    },
    [](auto str){
      return nlohmann::json::parse(str);  // parse json string
    }
  );
}


void BorovyloCSurf::SetTrackListChange()
{
  reaperConsoleMessage("called by host then change track list view");

  /*
   * reaperConsoleMessage("Read json config file");
   * auto action1 = JSONConfig["/button1"_json_pointer];
   * reaperConsoleMessage(action1);
   */

}


BorovyloCSurf::BorovyloCSurf()
{
  // constructor
  JSONConfig = loadJSONConfig(CONFIG);

  m_midiin0 = CreateMIDIInput(0);
  if (m_midiin0)
      m_midiin0->start();
  // m_midiout0 = CreateMIDIOutput(0, false, NULL);
}


BorovyloCSurf::~BorovyloCSurf()
{
  // destructor
  delete m_midiin0;
  delete m_midiout0;
}


void BorovyloCSurf::Run()
{
  // called by host 30x/sec or so.
}


void BorovyloCSurf::OnMIDIEvent0(MIDI_event_t *evt)
{
  // m_midiout0->SendMsg(evt, 0);
}


const char *BorovyloCSurf::GetTypeString()
{
  return "BOROVYLO";
}


const char *BorovyloCSurf::GetDescString()
{
  descspace.Set("Borovylo Surface Control");
  return descspace.Get();
}


const char *BorovyloCSurf::GetConfigString()
{
  sprintf(configtmp,"0 0");
  return configtmp;
}
