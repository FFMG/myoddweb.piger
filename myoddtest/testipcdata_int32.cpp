#include "os\ipcdata.h"
#include "testcommon.h"
#include <gtest/gtest.h>
#include <limits>

const struct test_int32_t
{
  std::wstring uuid;

  typedef std::vector<int32_t> VALUE_TYPE;
  VALUE_TYPE values;
};

struct MyoddOsInt32Test : testing::Test, testing::WithParamInterface<test_int32_t>
{
  myodd::os::IpcData* ipc;
  MyoddOsInt32Test() : ipc(nullptr)
  {
    ipc = new myodd::os::IpcData(GetParam().uuid);

    auto values = GetParam().values;
    for (auto it = values.begin(); it != values.end(); ++it)
    {
      ipc->Add(*it);
    }
  }

  ~MyoddOsInt32Test()
  {
    delete ipc;
  }
};

TEST_P(MyoddOsInt32Test, CheckIntValues)
{
  auto param = GetParam();
  ASSERT_EQ(param.uuid, ipc->GetGuid());

  auto values = GetParam().values;
  unsigned int index = 0;

  ASSERT_EQ(values.size(), ipc->GetNumArguments());
  for (auto it = values.begin(); it != values.end(); ++it)
  {
    ASSERT_EQ(*it, ipc->Get<int32_t>(index));
    ASSERT_TRUE(ipc->IsInt(index));
    ASSERT_FALSE(ipc->IsString(index));
    index++;
  }
}

INSTANTIATE_TEST_CASE_P(Default_int32, MyoddOsInt32Test,
  testing::Values(
    test_int32_t{ Uuid(),{ 0 } },
    test_int32_t{ Uuid(),{ 42 } },
    test_int32_t{ Uuid(),{ 100 } },
    test_int32_t{ Uuid(),{ std::numeric_limits<int32_t>::max() } },
    test_int32_t{ Uuid(),{ std::numeric_limits<int32_t>::min() } }
));

INSTANTIATE_TEST_CASE_P(Multiple_int32Values, MyoddOsInt32Test,
  testing::Values(
    test_int32_t{ Uuid(),{ 0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0 } },
    test_int32_t{ Uuid(),{ 0, -1, 2, -3, 4, -4, 3, -2, 1, 0 } },
    test_int32_t{ Uuid(),{ std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min() } },
    test_int32_t{ Uuid(),{ std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max() } }
));