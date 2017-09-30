#include "WDL/swell/swell.h"
#include "stdio.h"

#include <sstream>
#include <iostream>

// super nasty looking macro here but allows importing functions from Reaper with simple looking code
#define IMPAPI(x) if (!((*((void **)&(x)) = (void *)rec->GetFunc(#x)))) errcnt++;

// Lazarus: you need to define REAPERAPI_IMPLEMENT before you include reaper_plugin_functions.h
// https://forum.cockos.com/archive/index.php/t-156903.html
#define REAPERAPI_IMPLEMENT
#include "reaper_plugin/reaper_plugin_functions.h"

int g_registered_command01 = 0;
int g_registered_command02 = 0;

// Action default keyboard shortcut and action text set here
// Type gaccel_register_t define in modules/WDL/reaper_plugin/reaper_plugin.h
gaccel_register_t acreg01 = {{0, 0, 0}, "Description: Aspanoz message box test"};
gaccel_register_t acreg02 = {{0, 0, 0}, "Description: Aspanoz test toggles on/off"};

TrackEnvelope* selectedEnvelop;

char envName[64];

double position;
double value;
double bezier;
bool selected;
int shape;

std::string to_string(double x)
{
  std::ostringstream ss;
  ss << x;
  return ss.str();
}

void doAction1()
{

	selectedEnvelop = GetSelectedEnvelope(NULL);
	GetEnvelopePoint(selectedEnvelop, 1, &position, &value, &shape, &bezier, &selected);

  std::string str = to_string(value);

	char digits[10];
	std::strcpy( digits, str.c_str() );

	ShowMessageBox(digits, "ShowMessageBox Title", 0);
}

// global variable to hold the toggle state of the togglable action
// each togglable action of course needs its own variable for this...

int g_togglestate; // will be inited in main() function to 0 or value from ini-file

void doAction2()
{
	// this action does nothing else but toggles the global variable that keeps track of the toggle state
	// so it's useless as such but you can see the action state changing in the toolbar buttons and the actions list

  g_togglestate = (g_togglestate == 0) ? 1 : 0;

	// store new state of toggle action to ini file immediately
	char buf[8];
  // the REAPER api for ini-files only deals with strings, so form a string from the action
	// toggle state number.
	sprintf(buf, "%d", g_togglestate);
  SetExtState("simple_extension", "toggleaction_state", buf, true);
}

// Reaper calls back to this when it wants to execute an action registered by the extension plugin
bool hookCommandProc(int command, int flag)
{
  // it might happen Reaper calls with 0 for the command and if the action
  // registration has failed the plugin's command id would also be 0
  // therefore, check the plugins command id is not 0 and then if it matches with
  // what Reaper called with
  if (g_registered_command01 != 0 && command == g_registered_command01)
  {
    doAction1();
    return true;
  }
  if (g_registered_command02 != 0 && command == g_registered_command02)
  {
    doAction2();
    return true;
  }
  // failed to run relevant action
  return false;
}

// Reaper calls back to this when it wants to know an actions's toggle state
int toggleActionCallback(int command_id)
{
	if (command_id && command_id == g_registered_command02) return g_togglestate;
	// -1 if action not provided by this extension or is not togglable
	return -1;
}

extern "C"
{
  // this is the only function that needs to be exported by a Reaper extension plugin dll
  // everything then works from function pointers and others things initialized within this
  // function
  REAPER_PLUGIN_DLL_EXPORT int REAPER_PLUGIN_ENTRYPOINT(REAPER_PLUGIN_HINSTANCE hInstance, reaper_plugin_info_t *rec)
  {
    // if no rec plugin abort plugin loading
    if (!rec || rec->caller_version != REAPER_PLUGIN_VERSION || !rec->GetFunc) return 0;

    // initialize API function pointers from Reaper
    // REMEMBER to initialize the needed functions here, otherwise calling them results in a crash!
    int errcnt = 0; // this error counter variable will be "magically" incremented by the IMPAPI macro on errors
    IMPAPI(SetExtState);
    IMPAPI(GetExtState);

    IMPAPI(ShowMessageBox);
    IMPAPI(GetSelectedEnvelope);
    IMPAPI(GetEnvelopePoint);

    // if even one api function fails to initialize, it's not wise to continue, so abort plugin loading
    if (errcnt>0) return 0;

    acreg01.accel.cmd = g_registered_command01 = rec->Register("command_id", (void*)"EXAMPLE_ACTION_18");
    acreg02.accel.cmd = g_registered_command02 = rec->Register("command_id", (void*)"EXAMPLE_ACTION_02");

    if (!g_registered_command01) return 0; // failed getting a command id, fail!
    if (!g_registered_command02) return 0; // failed getting a command id, fail!
      
    rec->Register("gaccel", &acreg01);
    rec->Register("gaccel", &acreg02);
    rec->Register("hookcommand", (void*)hookCommandProc);
    rec->Register("toggleaction", (void*)toggleActionCallback);

    // restore extension global settings
    // saving extension data into reaper project files is another thing and 
    // at the moment not done in this example plugin
    const char* numberString = GetExtState("simple_extension","toggleaction_state");

    g_togglestate = 0;
    if (numberString) g_togglestate = atoi(numberString);
    // our plugin registered, return success

    return 1;
  }

};
