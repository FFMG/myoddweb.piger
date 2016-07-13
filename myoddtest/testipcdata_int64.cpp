#include "os\ipcdata.h"
#include "testcommon.h"
#include <gtest/gtest.h>
#include <limits>

const struct test_int64_t
{
  std::wstring uuid;

  typedef std::vector<int64_t> VALUE_TYPE;
  VALUE_TYPE values;
};

struct MyoddOsInt64Test : testing::Test, testing::WithParamInterface<test_int64_t>
{
  myodd::os::IpcData* ipc;
  MyoddOsInt64Test() : ipc(nullptr)
  {
    ipc = new myodd::os::IpcData(GetParam().uuid);

    auto values = GetParam().values;
    for (auto it = values.begin(); it != values.end(); ++it)
    {
      ipc->Add(*it);
    }
  }

  ~MyoddOsInt64Test()
  {
    delete ipc;
  }
};

TEST_P(MyoddOsInt64Test, CheckIntValues)
{
  auto param = GetParam();
  ASSERT_EQ(param.uuid, ipc->GetGuid());

  auto values = GetParam().values;
  unsigned int index = 0;

  ASSERT_EQ(values.size(), ipc->GetNumArguments());
  for (auto it = values.begin(); it != values.end(); ++it)
  {
    ASSERT_EQ(*it, ipc->Get<int64_t>(index));
    ASSERT_TRUE(ipc->IsInt(index));
    ASSERT_FALSE(ipc->IsString(index));
    index++;
  }
}

INSTANTIATE_TEST_CASE_P(Default_int64, MyoddOsInt64Test,
  testing::Values(
    test_int64_t{ Uuid(),{ 0 } },
    test_int64_t{ Uuid(),{ 42 } },
    test_int64_t{ Uuid(),{ 100 } },
    test_int64_t{ Uuid(),{ std::numeric_limits<int64_t>::max() } },
    test_int64_t{ Uuid(),{ std::numeric_limits<int64_t>::min() } }
));

INSTANTIATE_TEST_CASE_P(Multiple_int64Values, MyoddOsInt64Test,
  testing::Values(
    test_int64_t{ Uuid(),{ 0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0 } },
    test_int64_t{ Uuid(),{ 0, -1, 2, -3, 4, -4, 3, -2, 1, 0 } },
    test_int64_t{ Uuid(),{ std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::min() } },
    test_int64_t{ Uuid(),{ std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max() } }
));