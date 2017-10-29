#pragma once

#include <json.hpp>

#include <cabl/cabl.h>

#include <WDL/swell/swell.h>
#include <WDL/wdlstring.h>
#include <reaper_plugin/reaper_plugin_functions.h>


class Borovylo;
class CablDevice;


struct DeviceControl
{
  auto getValue(std::string name) const
  {
    return json[name];
  };
  int state = 0; // @TODO make ENUM state
  nlohmann::json json;
};

typedef std::map<int, DeviceControl> Store;


nlohmann::json loadJSONConfigFile(const char *filename);

using namespace sl::cabl;
class CablDevice : public Client
{
	friend class Borovylo;

	private:
    Borovylo*	borovylo;

  public:
    CablDevice(Borovylo* borovylo);
    ~CablDevice();

    void initDevice() override;
    void render() override;

    void buttonChanged(Device::Button button, bool buttonState, bool shiftState) override;
    void encoderChanged(unsigned encoder, bool valueIncreased, bool shiftPressed) override;
    void keyChanged(unsigned index, double value, bool shiftPressed) override;
    void controlChanged(unsigned pot, double value, bool shiftPressed) override;
};


class Borovylo : public IReaperControlSurface
{
  private:
    WDL_String descspace;         // for GetDescString
    char configtmp[1024];         // GetConfigString

    CablDevice *cablDevice;       // cabl device
    Store state;                  // app state

    std::string debug;            //@TODO delete

  public:
    Borovylo();
    virtual ~Borovylo();

    void Run();                   // called by host 30x/sec or so.
    void SetTrackListChange();    // on track list view change

    void onCablDeviceButton(int id); // run, then press button in Maschine
    void execAction(int id);         // exec action for state[id]

    // State operation
    Store initState();
    auto getStateControl(int id);

    const char *GetTypeString();
    const char *GetDescString();
    const char *GetConfigString();
};
