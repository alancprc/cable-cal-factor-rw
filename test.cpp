#include "cable-cal-factor-rw.h"

#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include "gmock/gmock.h"

using namespace std;

TEST(CableLossTest, LoadTest)
{
  CableLoss cl;
  cl.load("sample.cf");
}

TEST(CableLossTest, KeyTest)
{
  CableLoss::Key key("ant", 824e6, -3, 1);
  auto key2 = key;
  EXPECT_EQ(key, key2);

  CableLoss::Key k1("PH", 824e6, 0, 1);
  CableLoss::Key k2("PL", 824e7, 0, 1);
  EXPECT_TRUE(k1 < k2);
  EXPECT_FALSE(k2 < k1);

  CableLoss::Key k3("Ph", 824e8, 0, 1);
  CableLoss::Key k4("Pl", 824e9, 0, 1);
  EXPECT_TRUE(k3 < k4);
  EXPECT_FALSE(k4 < k3);

  EXPECT_FALSE(k4 == k3);

  EXPECT_TRUE(k1 < k3);
  EXPECT_FALSE(k3 < k1);
}

TEST(CableLossTest, GetTest)
{
  CableLoss cl;
  cl.load("sample.cf");

  CableLoss::Key key("RFIN_L_pin", 824e6, 0.0, 1);
  EXPECT_EQ(cl.get(key), -36.1);

  CableLoss::Key invalidKey("RFIN_L_pin", 824e6, 0.0, 2);
  EXPECT_ANY_THROW(cl.get(invalidKey));

  CableLoss::Key invalid("abc", 824e6, 0.0, 2);
  EXPECT_ANY_THROW(cl.get(invalid));
}

TEST(CableLossTest, LoadSaveTest)
{
  CableLoss cl;
  cl.load("sample.cf");
  cl.save("sample.save");
}

TEST(CableLossTest, ReadSavedTest)
{
  CableLoss cl;
  cl.load("sample.save");
  cl.print();
}
