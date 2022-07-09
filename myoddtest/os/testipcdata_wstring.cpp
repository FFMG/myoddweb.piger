#include "os\ipcdata.h"
#include "..\testcommon.h"
#include <gtest/gtest.h>

const struct test_wstring
{
  std::wstring uuid;

  typedef std::vector<std::wstring> VALUE_TYPE;
  VALUE_TYPE values;
};

struct MyoddOsWStringTest : testing::Test, testing::WithParamInterface<test_wstring>
{
  myodd::os::IpcData* ipc;
  MyoddOsWStringTest() : ipc(nullptr)
  {
    ipc = new myodd::os::IpcData(GetParam().uuid);

    auto values = GetParam().values;
    for (auto it = values.begin(); it != values.end(); ++it)
    {
      ipc->Add(*it);
    }
  }

  ~MyoddOsWStringTest()
  {
    delete ipc;
  }
};

TEST_P(MyoddOsWStringTest, CheckWStringValues)
{
  auto param = GetParam();
  ASSERT_EQ(param.uuid, ipc->GetGuid());

  auto values = GetParam().values;
  unsigned int index = 0;

  ASSERT_EQ(values.size(), ipc->GetNumArguments());
  for (auto it = values.begin(); it != values.end(); ++it)
  {
    ASSERT_EQ(*it, ipc->Get<std::wstring>(index));
    ASSERT_TRUE(ipc->IsString(index));
    index++;
  }
}

INSTANTIATE_TEST_SUITE_P(Default_wstring, MyoddOsWStringTest,
  testing::Values(
    test_wstring{ Uuid(),{ L"" } },
    test_wstring{ Uuid(),{ L"Hello world" } },
    test_wstring{ Uuid(),{ L"           " } },
    test_wstring{ Uuid(),{ L"  \n \t " } },
    test_wstring{ Uuid(),{ L"\0" } }
));

INSTANTIATE_TEST_SUITE_P(Multiple_stringValues, MyoddOsWStringTest,
  testing::Values(
    test_wstring{ Uuid(),{ L"", L"     ", L"Hello", L"!@#$%^&*(" } },
    test_wstring{ Uuid(),{ L"12345", L"     ", L"Hello", L"!@#$%^&*(" } }
));