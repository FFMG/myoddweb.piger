// pythoncore64.test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "../monitor/api/python/Include/Python.h"

// void Clear( PyThreadState *state )
// {
//   // one last lock.
//   PyEval_AcquireLock();
//
//   // restore the thread
//   PyThreadState_Swap(state);
//
//   if (PyGILState_Check()) {
//     /* I have the GIL */
//     // 
//     PyEval_ReleaseLock();
//   }
//
//   // simply close python
//   Py_Finalize();
//
// }

int main()
{
  // PyThreadState* mainThreadState = nullptr;
  // Py_SetPythonHome(L"Z:/projects/github/myoddweb.piger/Output/Debug/x64/python/");
  Py_SetPath(L"Z://projects//github//myoddweb.piger//Output/Debug//x64//python//;Z://projects//github//myoddweb.piger//Output/Debug//x64//python//python37.zip");
  Py_InitializeEx(0);

  // Clear(mainThreadState);

  std::cout << "Hello World!\n"; 
}
