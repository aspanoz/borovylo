#define REAPERAPI_IMPLEMENT
#define REAPERAPI_DECL

#include <reaper_plugin/reaper_plugin_functions.h>

#include "csurf_cabl.h"

REAPER_PLUGIN_HINSTANCE g_hInst;
HWND g_hwnd;

/*
 * Entry point to reaper csurf 
 */
static IReaperControlSurface *createFunc(const char *type_string, const char *configString, int *errStats)
{
  return new CSurf_Borovylo();
}


/*
 * Creates the dialog.
 */
static HWND configFunc(const char *type_string, HWND parent, const char *initConfigString)
{
  // @TODO: build WDL/swell
  return 0;
}


/*
 * Registration array containing the info of our plugin
 */
reaper_csurf_reg_t csurf_borovylo_reg =
{
  "BOROVYLO",
  "Borovylo Surface Control",
  createFunc,
  configFunc,
};


/*
 * Reaper plugin entry point
 */
extern "C"
{
  REAPER_PLUGIN_DLL_EXPORT int REAPER_PLUGIN_ENTRYPOINT(REAPER_PLUGIN_HINSTANCE hInstance, reaper_plugin_info_t *rec)
  {
    if (!rec || rec->caller_version != REAPER_PLUGIN_VERSION || !rec->GetFunc)
      return 0;

    // load Reaper API functions
    if (REAPERAPI_LoadAPI(rec->GetFunc) > 0)
      return 0;


    g_hInst = hInstance;
    g_hwnd = rec->hwnd_main;

    rec->Register("csurf", &csurf_borovylo_reg);

    return 1;
  }
}
