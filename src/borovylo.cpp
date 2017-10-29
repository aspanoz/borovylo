#include <sys/timeb.h>
#include <memory>
#include <thread>

#include <fplus/fplus.hpp>
#include <json.hpp>

#include "borovylo.h"

#define CONFIG "/Scripts/Aspanoz/package.json"

extern REAPER_PLUGIN_HINSTANCE g_hInst;
extern HWND g_hwnd;


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
      return fplus::read_text_file(path)(); // read config file
    },
    [](auto str){
      return nlohmann::json::parse(str);  // parse json string
    }
  );
}


auto Borovylo::getStateControl(int id)
{
  return [=](std::string name){
    return state[id].getValue(name);
  };
}


void Borovylo::execAction(int id)
{
  // @TODO make it with maybe
  if (fplus::map_contains(state, id))
    fplus::fwd::apply(
      getStateControl(id),
      [=](auto control){ Main_OnCommandEx(control("commandId"), 0, NULL); }
    );
}


void Borovylo::onCablDeviceButton(int id)
{
  std::cout << "onCablDeviceButton get id: " << id << '\n';

  std::thread thr(&Borovylo::execAction, this, id); // exec action in detached thread
  thr.detach();
}


Borovylo::Borovylo()
: cablDevice(NULL)
{
  // constructor
  state = initState();
  cablDevice = new CablDevice(this);
}


void Borovylo::SetTrackListChange()
{
  // reaperConsoleMessage("called by host then change track list view");
  // M_LOG("[Borovylo] SetTrackListChange");
}


Store Borovylo::initState()
{
  Store store;  
  for (nlohmann::json j : loadJSONConfig(CONFIG))
    store[j["deviceKey"]].json = j;
  return store;
}


Borovylo::~Borovylo()
{
  M_LOG("[Borovylo] destructor");
  // destructor
  // delete cablDevice;
}


void Borovylo::Run()
{
  // called by host 30x/sec or so.
}


const char *Borovylo::GetTypeString()
{
  return "BOROVYLO";
}


const char *Borovylo::GetDescString()
{
  descspace.Set("Borovylo Surface Control");
  return descspace.Get();
}


const char *Borovylo::GetConfigString()
{
  sprintf(configtmp,"0 0");
  return configtmp;
}
