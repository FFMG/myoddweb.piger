#include "StdAfx.h"

#include "csapi.h"
#include "pluginVirtualMachine.h"

/**
 * \brief constructor
 * \param action the action being called.
 */
CsApi::CsApi(const ActiveAction& action) :
  PowershellApi(action )
{
}
