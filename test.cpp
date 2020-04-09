#include "cable-cal-factor-rw.h"

#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include "gmock/gmock.h"

using namespace std;

TEST(CableLossTest, LoadTest)
{
  CableLoss cl;
  cl.load("sample.cf");

  EXPECT_EQ(cl.getCalFactor("RFIN_L_pin", 824e6, 0.0, 1), -36.1);

  EXPECT_ANY_THROW(cl.getCalFactor("RFIN_L_pin", 824e6, 0.0, 2));

  cl.print();
}
