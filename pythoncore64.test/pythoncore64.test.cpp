// pythoncore64.test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

// #pragma comment(lib,"Ws2_32.Lib")
// #pragma comment(lib,"version.lib")
// #pragma comment(lib, "Shlwapi.lib")
//
// # if defined(_DEBUG)
// #   pragma comment(lib,"python37_d.lib")
// # elif defined(Py_LIMITED_API)
// #   pragma comment(lib,"python37.lib")
// # endif /* _DEBUG */

#include "Python.h"

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
//  Py_SetPath(L"H:/projects/github/myoddweb.piger/Output/Debug/x64/python/");
  Py_SetPath(L"H:/projects/github/myoddweb.piger/Output/Debug/x64/python/python37.zip");
  Py_Initialize();

  if (PyErr_ExceptionMatches(PyExc_AttributeError)) {
    PyErr_Clear();
  }

  // Clear(mainThreadState);

  std::cout << "Hello World!\n"; 
}
