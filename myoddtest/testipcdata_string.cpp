#include "os\ipcdata.h"
#include "testcommon.h"
#include <gtest/gtest.h>

const struct test_string
{
  std::wstring uuid;

  typedef std::vector<std::string> VALUE_TYPE;
  VALUE_TYPE values;
};

struct MyoddOsStringTest : testing::Test, testing::WithParamInterface<test_string>
{
  myodd::os::IpcData* ipc;
  MyoddOsStringTest() : ipc(nullptr)
  {
    ipc = new myodd::os::IpcData(GetParam().uuid);

    auto values = GetParam().values;
    for (auto it = values.begin(); it != values.end(); ++it)
    {
      ipc->Add(*it);
    }
  }

  ~MyoddOsStringTest()
  {
    delete ipc;
  }
};

TEST_P(MyoddOsStringTest, CheckIntValues)
{
  auto param = GetParam();
  ASSERT_EQ(param.uuid, ipc->GetGuid());

  auto values = GetParam().values;
  unsigned int index = 0;

  ASSERT_EQ(values.size(), ipc->GetNumArguments());
  for (auto it = values.begin(); it != values.end(); ++it)
  {
    ASSERT_EQ(*it, ipc->Get<std::string>(index));
    ASSERT_TRUE(ipc->IsString(index));
    index++;
  }
}

INSTANTIATE_TEST_CASE_P(Default_string, MyoddOsStringTest,
  testing::Values(
    test_string{ Uuid(),{ "" } },
    test_string{ Uuid(),{ "Hello world" } },
    test_string{ Uuid(),{ "           " } },
    test_string{ Uuid(),{ "  \n \t " } },
    test_string{ Uuid(),{ "\0" } }
));

INSTANTIATE_TEST_CASE_P(Multiple_stringValues, MyoddOsStringTest,
  testing::Values(
    test_string{ Uuid(),{ "", "     ", "Hello", "!@#$%^&*(" } },
    test_string{ Uuid(),{ "12345", "     ", "Hello", "!@#$%^&*(" } }    
));