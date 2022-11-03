#include <gtest/gtest.h>
#include "myoddinclude.h"
#include "os\ipcdata.h"
#include "..\testcommon.h"

TEST(MyoddOsTest, CheckThatTheGuiIsSavedProperly) {
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData( uuid );
  ASSERT_EQ(uuid, ipc.GetGuid());
}

TEST(MyoddOsTest, FromGuidToPtrAndBack) {
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData(uuid);

  auto pData = ipc.GetPtr();
  auto dataSize = (unsigned int)ipc.GetSize();

  auto copyIpc = myodd::os::IpcData(pData, dataSize);
  ASSERT_EQ(uuid, copyIpc.GetGuid());
}

TEST(MyoddOsTest, TryAndPassAnEmptyByteArray) {
  // create the ipc
  unsigned char* pData = nullptr;
  unsigned int dataSize = 0;

  EXPECT_THROW( myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOsTest, TryAndPassArrayThatIsTooSmallToEvenStart) {
  // create the ipc
  unsigned char pData[3] = {};
  unsigned int dataSize = 3;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOsTest, TheVersionNumberIsPastOurVersionNumber) {
  // create the ipc
  unsigned char pData[4] = {200, 0, 0, 0};
  unsigned int dataSize = 4;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOsTest, UnknownDataType) {
  // create the ipc
  unsigned char pData[6] = { 100, 0, 0, 0, // version
                             23, 0         // unknown data type
                           };
  unsigned int dataSize = 6;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOsTest, NoDataForGuid) {
  // create the ipc
  unsigned char pData[6] = { 100, 0, 0, 0, // version
                             ExpectedIpcDataType::Guid, 0          // No data for guid
                           };
  unsigned int dataSize = 6;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOsTest, NotEnoughDataForGuid) {
  // create the ipc
  unsigned char pData[8] = { 100, 0, 0, 0, // version
                              ExpectedIpcDataType::Guid, 0,        // No data for guid
                              0, 0
                            };
  unsigned int dataSize = 8;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOsTest, TheGivenGuiSizeDoesNotExist) {
  // create the ipc
  unsigned char pData[10] = { 100, 0, 0, 0, // version
                               ExpectedIpcDataType::Guid, 0,        // uuid
                              10, 0, 0, 0
                                            // no data
                            };
  unsigned int dataSize = 10;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOsTest, NoDataForInt32) {
  // create the ipc
  unsigned char pData[6] = { 100, 0, 0, 0, // version
                             ExpectedIpcDataType::Int32, 0          // No data for int
                           };
  unsigned int dataSize = 6;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOsTest, NotEnoughDataForInt32) {
  // create the ipc
  unsigned char pData[9] = { 100, 0, 0, 0, // version
                             ExpectedIpcDataType::Int32, 0,         // No data for int
                             0, 0, 0 
                           };
  unsigned int dataSize = 9;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOsTest, NoDataForInt64) {
  // create the ipc
  unsigned char pData[6] = { 100, 0, 0, 0, // version
                             ExpectedIpcDataType::Int64, 0          // No data for long
                           };
  unsigned int dataSize = 6;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOsTest, NotEnoughDataForInt64) {
  // create the ipc
  unsigned char pData[9] = { 100, 0, 0, 0, // version
                             ExpectedIpcDataType::Int64, 0,         // No data for long
                             0, 0, 0
                           };
  unsigned int dataSize = 9;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOsTest, NotEnoughDataForString) {
  // create the ipc
  unsigned char pData[6] = { 100, 0, 0, 0, // version
                             ExpectedIpcDataType::String, 0          // No data for string
                           };
  unsigned int dataSize = 6;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOsTest, NotEnoughDataForAsciiString) {
  // create the ipc
  unsigned char pData[6] = { 100, 0, 0, 0, // version
                             ExpectedIpcDataType::StringAscii, 0          // No data for ascii string
                           };
  unsigned int dataSize = 6;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOsTest, NotEnoughDataForFirstDataType) {
  // create the ipc
  unsigned char pData[5] = { 100, 0, 0, 0, // version
                             2             // No data for short
                           };
  unsigned int dataSize = 5;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOsTest, GetStringOnly) {
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData(uuid);

  ipc.Add(L"Hello");
  ipc.Add(L"World");

  ASSERT_EQ(L"Hello", ipc.Get<std::wstring>(0));
  ASSERT_EQ(L"World", ipc.Get<std::wstring>(1));
}

TEST(MyoddOsTest, GetMixtedDataTypes) {
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData(uuid);

  ipc.Add(L"Hello");
  ipc.Add(L"World");
  ipc.Add(42);
  ipc.Add("Something" );

  ASSERT_EQ(L"Hello", ipc.Get<std::wstring>(0));
  ASSERT_EQ(L"World", ipc.Get<std::wstring>(1));
  ASSERT_EQ(42, ipc.Get<int>(2));
  ASSERT_EQ(L"Something", ipc.Get<std::wstring>(3));
}

TEST(MyoddOsTest, MakeSureThatArgumentCountIsValid) {
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData(uuid);
  ASSERT_EQ(0, ipc.GetNumArguments());

  ipc.Add(L"Hello");
  ASSERT_EQ(1, ipc.GetNumArguments());

  ipc.Add(L"World");
  ASSERT_EQ(2, ipc.GetNumArguments());

  ipc.Add(42);
  ASSERT_EQ(3, ipc.GetNumArguments());

  ipc.Add("Something");
  ASSERT_EQ(4, ipc.GetNumArguments());
}

TEST(MyoddOsTest, TryingToGetAnOutofRangeItemInEmptyIpc) {
  // create the ipc
  auto ipc = myodd::os::IpcData(Uuid());

  EXPECT_THROW( ipc.Get<std::wstring>(0), std::exception );
}

TEST_MEM(MyoddOsTest, CompareWitNoData)
{
  // create the ipc
  auto uuid = Uuid();
  auto ipc1 = myodd::os::IpcData(uuid);

  // copy it
  auto ipc2 = ipc1;

  // check it.
  ASSERT_EQ(ipc1, ipc2);
}

TEST_MEM(MyoddOsTest, CompareWitNoDataAndNotSameGuid)
{
  // create the ipc
  auto uuid1 = Uuid();
  auto ipc1 = myodd::os::IpcData(uuid1);

  auto uuid2 = Uuid();
  auto ipc2 = myodd::os::IpcData(uuid2);

  // check it.
  ASSERT_NE(ipc1, ipc2);
}

TEST_MEM(MyoddOsTest, CompareJustAnInt32)
{
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData(uuid);
  ipc.Add( IntRandomNumber<int32_t>());

  // copy it
  auto ipc2 = ipc;

  // check it.
  ASSERT_EQ(ipc, ipc2);
}

TEST_MEM(MyoddOsTest, CompareJustAnInt64)
{
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData(uuid);
  ipc.Add(IntRandomNumber<int64_t>());

  // copy it
  auto ipc2 = ipc;

  // check it.
  ASSERT_EQ(ipc, ipc2);
}

TEST_MEM(MyoddOsTest, CompareJustAString )
{
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData(uuid);
  ipc.Add(RandomString( IntRandomNumber<size_t>(10, 20)));

  // copy it
  auto ipc2 = ipc;

  // check it.
  ASSERT_EQ(ipc, ipc2);
}

TEST_MEM(MyoddOsTest, CompareJustAWideString)
{
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData(uuid);
  ipc.Add(RandomString(IntRandomNumber<size_t>(10, 20)));

  // copy it
  auto ipc2 = ipc;

  // check it.
  ASSERT_EQ(ipc, ipc2);
}

void CreateRandomIpcData( const int size, myodd::os::IpcData& parent)
{
  for (auto i = 0; i < size; ++i)
  {
    auto type = IntRandomNumber<unsigned int>(0, 3);
    switch (type)
    {
    case 0:
      parent.Add(IntRandomNumber<int32_t>());
      break;

    case 1:
      parent.Add(IntRandomNumber<int64_t>());
      break;

    case 2:
      parent.Add(RandomWideString(IntRandomNumber<size_t>(10, 20)));
      break;

    case 3:
      parent.Add(RandomString(IntRandomNumber<size_t>(10, 20)));
      break;

    default:
      throw std::runtime_error("Unknown type...");
      break;
    }
  }
}

TEST_MEM(MyoddOsTest, AssignValueToAnItemThatAlreadyHasValues)
{
  // create 2 ipc
  auto uuid1 = Uuid();
  auto ipc1 = myodd::os::IpcData(uuid1);
  auto size1 = IntRandomNumber<int>(20, 50);
  CreateRandomIpcData(size1, ipc1);

  auto uuid2 = Uuid();
  auto ipc2 = myodd::os::IpcData(uuid2);
  auto size2 = IntRandomNumber<int>(20, 50);
  CreateRandomIpcData(size2, ipc2);

  // now assign 1 to 2
  ipc2 = ipc1;

  // and 1 should equal 2
  ASSERT_EQ(ipc1, ipc2);
}

TEST_MEM_LOOP(MyoddOsTest, CompareVariousTypeSizes, NUMBER_OF_TESTS)
{
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData(uuid);
  auto size = IntRandomNumber<int>(20, 50);
  CreateRandomIpcData(size, ipc);

  // copy it
  auto ipc2 = ipc;

  // check it.
  ASSERT_EQ(ipc, ipc2);
}

TEST_MEM_LOOP(MyoddOsTest, CompareVariousTypeSizesButNotQuiteSame, NUMBER_OF_TESTS)
{
  // create the ipc
  auto uuid = Uuid();
  auto ipc = myodd::os::IpcData(uuid);
  auto size = IntRandomNumber<int>(20, 50);
  CreateRandomIpcData(size, ipc);

  // copy it
  auto ipc2 = ipc;

  //   just add a couple of items
  CreateRandomIpcData(2, ipc2);

  // they are no longer equal.
  ASSERT_NE(ipc, ipc2);
}


