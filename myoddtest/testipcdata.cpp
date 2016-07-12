#include <gtest/gtest.h>
#include "myoddinclude.h"
#include "os\ipcdata.h"
#include <string>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

std::wstring Uuid()
{
  return boost::lexical_cast<std::wstring>(boost::uuids::random_generator()());
}

TEST(MyoddOs, CheckThatTheGuiIsSavedProperly) {
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData( uuid );
  ASSERT_EQ(uuid, ipc.GetGuid());
}

TEST(MyoddOs, FromGuidToPtrAndBack) {
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData(uuid);

  auto pData = ipc.GetPtr();
  auto dataSize = (unsigned int)ipc.GetSize();

  auto copyIpc = myodd::os::IpcData(pData, dataSize);
  ASSERT_EQ(uuid, copyIpc.GetGuid());
}

TEST(MyoddOs, GetStringOnly) {
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData(uuid);

  ipc.Add(L"Hello");
  ipc.Add(L"World");

  ASSERT_EQ(L"Hello", ipc.Get<std::wstring>(0));
  ASSERT_EQ(L"World", ipc.Get<std::wstring>(1));
}
