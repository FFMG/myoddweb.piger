// pythoncore64.test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#define ACTIONMONITOR_API_PY
#include "../monitor/api/PythonVirtualMachine.h"

int main()
{
  auto pvm = new PythonVirtualMachine();
  pvm->Initialize();

  delete pvm;

  std::cout << "Hello World!\n"; 
}
