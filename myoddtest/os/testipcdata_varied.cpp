#include "os\ipcdata.h"
#include "..\testcommon.h"
#include <gtest/gtest.h>
#include <limits>

const struct test_varied
{
  std::wstring uuid;

  struct ValueType
  {
    ExpectedIpcDataType type;
    std::wstring stringW;
    std::string stringA;
    int32_t int32;
    int64_t int64;
  };

  typedef std::vector<ValueType> VALUE_TYPE;
  VALUE_TYPE values;
};

struct MyoddOsVariedTest : testing::Test, testing::WithParamInterface<test_varied>
{
  myodd::os::IpcData* ipc;
  MyoddOsVariedTest() : ipc(nullptr)
  {
    ipc = new myodd::os::IpcData(GetParam().uuid);

    auto values = GetParam().values;
    for (auto it = values.begin(); it != values.end(); ++it)
    {
      switch(it->type )
      {
      case Int32:
        ipc->Add(it->int32);
        break;

      case Int64:
        ipc->Add(it->int64);
        break;

      case String:
        ipc->Add(it->stringW);
        break;

      case StringAscii:
        ipc->Add(it->stringA);
        break;

      case None:
      case Guid:

      default: 
        throw std::exception("Type not supported");
      }
    }
  }

  ~MyoddOsVariedTest()
  {
    delete ipc;
  }

  bool IsValid( const test_varied::ValueType& vt, unsigned int index ) const
  {
    switch (vt.type)
    {
    case Int32:
      return (ipc->Get<int32_t>(index) == vt.int32);

    case Int64:
      return (ipc->Get<int64_t>(index) == vt.int64);

    case String:
      return (ipc->Get<std::wstring>(index) == vt.stringW);

    case StringAscii:
      return (ipc->Get<std::string>(index) == vt.stringA);

    case None:
    case Guid:
    default:
      throw std::exception("Type not supported");
    }
  }
};

TEST_P(MyoddOsVariedTest, CheckVarriedValues)
{
  auto param = GetParam();
  ASSERT_EQ(param.uuid, ipc->GetGuid());

  auto values = GetParam().values;
  unsigned int index = 0;

  ASSERT_EQ(values.size(), ipc->GetNumArguments());
  for (auto it = values.begin(); it != values.end(); ++it)
  {
    ASSERT_TRUE( IsValid( *it, index ));
    index++;
  }
}

INSTANTIATE_TEST_CASE_P(Default_Varied, MyoddOsVariedTest,
  testing::Values(
    test_varied{ Uuid(),{{ Int32, L"", "", 0, 0 },{ Int64, L"","", 0, 0 },{ Int32, L"","", 0, 0 }} },
    test_varied{ Uuid(),{{ Int32, L"", "", std::numeric_limits<int32_t>::min(), 0 },
                         { Int64, L"", "", 0, std::numeric_limits<int64_t>::max() },
                         { Int32, L"", "", std::numeric_limits<int32_t>::min(), 0 },
                        } 
               },
    test_varied{ Uuid(),{{ Int32, L"", "", std::numeric_limits<int32_t>::max(), 0 },
                         { Int64, L"", "", 0, std::numeric_limits<int64_t>::min() },
                         { Int32, L"", "", std::numeric_limits<int32_t>::max(), 0 },
                        } 
               },
    test_varied{ Uuid(),{{ String, L"Hello", "", 0, 0 },
                         { Int64, L"", "", 0, std::numeric_limits<int64_t>::max() },
                         { Int32, L"", "", std::numeric_limits<int32_t>::min(), 0 },
                         { Int64, L"", "", 0, std::numeric_limits<int64_t>::min() },
                         { StringAscii, L"", "Hello", 0, 0 },
                        } 
               },
    test_varied{ Uuid(),{{ String, L"Hello", "", 0, 0 },
                         { String, L"", "", 0, 0 },   //  empty string
                         { Int64, L"", "", 0, std::numeric_limits<int64_t>::max() },
                         { Int32, L"", "", std::numeric_limits<int32_t>::min(), 0 },
                         { Int64, L"", "", 0, std::numeric_limits<int64_t>::min() },
                         { StringAscii, L"", "", 0, 0 },   //  empty string
                        } 
               }
  ));
