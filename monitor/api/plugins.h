#pragma once

// this is the various API we are using
// remove/comment if we are not using it.
#define ACTIONMONITOR_API_LUA
#define ACTIONMONITOR_API_PY
#define ACTIONMONITOR_API_PLUGIN
#define ACTIONMONITOR_PS_PLUGIN

#ifdef ACTIONMONITOR_API_LUA
#   include "../api/luavirtualmachine.h"
#endif

#ifdef ACTIONMONITOR_API_PY
#   include "../api/pythonvirtualmachine.h"
#endif

#ifdef ACTIONMONITOR_API_PLUGIN
#   include "../api/pluginvirtualmachine.h"
#endif

#ifdef ACTIONMONITOR_PS_PLUGIN
#   include "../api/powershellvirtualmachine.h"
#endif

