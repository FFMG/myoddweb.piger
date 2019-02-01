// pythoncore64.test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>

#define _POSIX_THREADS 1
#include "../monitor/api/python/include/Python.h"

void Clear( PyThreadState *state )
{
  // one last lock.
  PyEval_AcquireLock();

  // restore the thread
  PyThreadState_Swap(state);

  if (PyGILState_Check()) {
    /* I have the GIL */
    // 
    PyEval_ReleaseLock();
  }

  // simply close python
  Py_Finalize();

}

int main()
{
  PyThreadState* mainThreadState = nullptr;
  Py_SetPythonHome(L"../includes/python64/");
  Py_SetPath(L"../includes/python64/;../includes/python64d/;../includes/python64/python37.zip");
  Py_Initialize();

  Clear(mainThreadState);

  std::cout << "Hello World!\n"; 
}
