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
  auto k2 = key;
  EXPECT_EQ(key, k2);
}

TEST(CableLossTest, GetTest)
{
  CableLoss cl;
  cl.load("sample.cf");

  CableLoss::Key key("RFIN_L_pin", 824e6, 0.0, 1);
  EXPECT_EQ(cl.get(key), -36.1);

  CableLoss::Key invalidKey("RFIN_L_pin", 824e6, 0.0, 2);
  EXPECT_ANY_THROW(cl.get(invalidKey));
}
