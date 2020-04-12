#include "cable-cal-factor-rw.h"

#include <iostream>
#include "gmock/gmock.h"

using namespace std;

class CableLossTest : public ::testing::Test
{
 protected:
  CableLoss cl;
  void SetUp() {}
};

TEST_F(CableLossTest, LoadTest)
{
  cl.load("sample.cf");

  EXPECT_EQ(cl.get("RFIN_L_pin", 824e6, 0.0, 1), -36.1);

  EXPECT_ANY_THROW(cl.get("RFIN_L_pin", 824e6, 0.0, 2));
}

TEST_F(CableLossTest, LoadSaveTest)
{
  cl.load("sample.cf");
  cl.save("sample.save");
}

TEST_F(CableLossTest, ReadSavedTest)
{
  cl.load("sample.save");
  cl.print();
}

TEST_F(CableLossTest, GetSetTest)
{
  EXPECT_ANY_THROW(cl.get("a", 1, 1, 1));
  cl.set("a", 1, 1, 1, 1.8);
  EXPECT_NO_THROW(cl.get("a", 1, 1, 1));
  EXPECT_EQ(1.8, cl.get("a", 1, 1, 1));
}

TEST_F(CableLossTest, InitClearTest)
{
  EXPECT_TRUE(cl.empty());
  cl.set("a", 1, 1, 1, 1);
  EXPECT_FALSE(cl.empty());
  cl.init();
  EXPECT_TRUE(cl.empty());
}
