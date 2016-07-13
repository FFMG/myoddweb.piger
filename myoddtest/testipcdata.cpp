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

TEST(MyoddOs, TryAndPassAnEmptyByteArray) {
  // create the ipc
  unsigned char* pData = nullptr;
  unsigned int dataSize = 0;

  EXPECT_THROW( myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOs, TryAndPassArrayThatIsTooSmallToEvenStart) {
  // create the ipc
  unsigned char pData[3] = {};
  unsigned int dataSize = 3;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOs, TheVersionNumberIsPastOurVersionNumber) {
  // create the ipc
  unsigned char pData[4] = {200, 0, 0, 0};
  unsigned int dataSize = 4;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOs, UnknownDataType) {
  // create the ipc
  unsigned char pData[6] = { 100, 0, 0, 0, // version
                             23, 0         // unknown data type
                           };
  unsigned int dataSize = 6;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOs, NoDataForGuid) {
  // create the ipc
  unsigned char pData[6] = { 100, 0, 0, 0, // version
                             1, 0          // No data for guid
                           };
  unsigned int dataSize = 6;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOs, NotEnoughDataForGuid) {
  // create the ipc
  unsigned char pData[8] = { 100, 0, 0, 0, // version
                              1, 0,        // No data for guid
                              0, 0
                            };
  unsigned int dataSize = 8;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOs, TheGivenGuiSizeDoesNotExist) {
  // create the ipc
  unsigned char pData[10] = { 100, 0, 0, 0, // version
                               1, 0,        // uuid
                              10, 0, 0, 0
                                            // no data
                            };
  unsigned int dataSize = 10;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOs, NoDataForInt32) {
  // create the ipc
  unsigned char pData[6] = { 100, 0, 0, 0, // version
                             2, 0          // No data for int
                           };
  unsigned int dataSize = 6;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOs, NotEnoughDataForInt32) {
  // create the ipc
  unsigned char pData[9] = { 100, 0, 0, 0, // version
                             2, 0,         // No data for int
                             0, 0, 0 
                           };
  unsigned int dataSize = 9;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOs, NoDataForInt64) {
  // create the ipc
  unsigned char pData[6] = { 100, 0, 0, 0, // version
                             3, 0          // No data for long
                           };
  unsigned int dataSize = 6;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOs, NotEnoughDataForInt64) {
  // create the ipc
  unsigned char pData[9] = { 100, 0, 0, 0, // version
                             3, 0,         // No data for long
                             0, 0, 0
                           };
  unsigned int dataSize = 9;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOs, NotEnoughDataForString) {
  // create the ipc
  unsigned char pData[6] = { 100, 0, 0, 0, // version
                             4, 0          // No data for string
                           };
  unsigned int dataSize = 6;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOs, NotEnoughDataForAsciiString) {
  // create the ipc
  unsigned char pData[6] = { 100, 0, 0, 0, // version
                             5, 0          // No data for ascii string
                           };
  unsigned int dataSize = 6;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
}

TEST(MyoddOs, NotEnoughDataForFirstDataType) {
  // create the ipc
  unsigned char pData[5] = { 100, 0, 0, 0, // version
                             2             // No data for short
                           };
  unsigned int dataSize = 5;

  EXPECT_THROW(myodd::os::IpcData(pData, dataSize), std::exception);
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

TEST(MyoddOs, GetMixtedDataTypes) {
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

TEST(MyoddOs, MakeSureThatArgumentCountIsValid) {
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

TEST(MyoddOs, TryingToGetAnOutofRangeItemInEmptyIpc) {
  // create the ipc
  auto ipc = myodd::os::IpcData(Uuid());

  EXPECT_THROW( ipc.Get<std::wstring>(0), std::exception );
}

TEST(MyoddOs, TryingToGetAnOutofRangeItemInNonEmptyIpc) {
  // create the ipc
  auto ipc = myodd::os::IpcData(Uuid());
  
  // add a couple of values
  ipc.Add(42);
  ipc.Add((int64_t)42);

  // we have item 0, but not 2
  EXPECT_THROW(ipc.Get<std::wstring>(2), std::exception);
}
