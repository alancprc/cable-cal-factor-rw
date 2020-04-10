// ----------------------------------------------------------------------------
// Copyright (C) 2002-2006 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------

#include "cable-cal-factor-rw.h"
#include "util.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <exception>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <cstdio>

using namespace std;
namespace pt = boost::property_tree;

void CableLoss::load(const std::string& filename)
{
  pt::ptree tree;
  pt::read_xml(filename, tree, pt::xml_parser::trim_whitespace);
  time = tree.get("caltime", 0);

  BOOST_FOREACH (pt::ptree::value_type& v, tree.get_child("")) {
    if (v.first == "info") {
      string pin = v.second.get<string>("pin");
      double gain = v.second.get<double>("gain");
      double atten = v.second.get<double>("atten");

      cout << "pin: " << pin
           << "\tgain: " << gain
           << "\tatten: " << atten;

      BOOST_FOREACH (pt::ptree::value_type& f, v.second.get_child("powers")) {
        cout << "\t" << f.second.get<double>("");
      }
      cout << endl;
    } else if (v.first == "factors") {
      string pin = v.second.get<string>("pin");
      double freq = v.second.get<double>("freq");
      double power = v.second.get<double>("power");
      int site = v.second.get<int>("site");
      double value = v.second.get<double>("value");
      factors[pin][freq][power][site] = value;
    }
  }
}

double CableLoss::getCalFactor(const std::string& name, double freq,
                               double power, int site) const
{
  return factors.at(name).at(freq).at(power).at(site);
}

void CableLoss::printConfig() const
{
}

void CableLoss::print() const
{
  std::ostringstream oss;
  for (FactorType::const_iterator pin = factors.begin(); pin != factors.end(); ++pin) {
    for (FactorType::mapped_type::const_iterator freq = pin->second.begin(); freq != pin->second.end(); ++freq) {
      for (FactorType::mapped_type::mapped_type::const_iterator pwr = freq->second.begin(); pwr != freq->second.end(); ++pwr) {
        for (FactorType::mapped_type::mapped_type::mapped_type::const_iterator si = pwr->second.begin(); si != pwr->second.end(); ++si) {
          // clang-format off
          oss << "pin: " << pin->first << ";\t"
              << "freq: " << freq->first << ";\t"
              << "power: " << pwr->first << ";\t"
              << "site: " << si->first << ";\t"
              << "factor: " << si->second << ";\t"
              << endl;
          // clang-format on
        }
      }
    }
  }
  cout << oss.str();
}
