#include <gtest/gtest.h>
#include "myoddinclude.h"
#include "os\ipcdata.h"
#include <string>

std::wstring Uuid()
{
  return L"xxx-yyy-zzz";
}

TEST(MyoddOs, Basic) {
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData( uuid );
  ASSERT_EQ(uuid, ipc.GetGuid());
}
