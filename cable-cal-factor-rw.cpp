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
#include <ctime>
#include <exception>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace std;
namespace pt = boost::property_tree;

void CableLoss::load(const std::string& filename)
{
  pt::ptree tree;
  pt::read_xml(filename, tree, pt::xml_parser::trim_whitespace);
  time = tree.get("caltime", 0);

  BOOST_FOREACH (pt::ptree::value_type& v, tree.get_child("infos")) {
    string pin = v.second.get<string>("pin");
    double gain = v.second.get<double>("gain");
    double atten = v.second.get<double>("atten");

    cout << "pin: " << pin << "\tgain: " << gain << "\tatten: " << atten;

    BOOST_FOREACH (pt::ptree::value_type& f, v.second.get_child("powers")) {
      cout << "\t" << f.second.get<double>("");
    }
    cout << endl;
  }
  BOOST_FOREACH (pt::ptree::value_type& v, tree.get_child("factors")) {
    string pin = v.second.get<string>("pin");
    double freq = v.second.get<double>("freq");
    double power = v.second.get<double>("power");
    int site = v.second.get<int>("site");
    double value = v.second.get<double>("value");
    factors[pin][freq][power][site] = value;
  }
}

void CableLoss::save(const string& filename) const
{
  pt::ptree tree;
  time_t now;
  std::time(&now);
  tree.put("caltime", long(now));

  BOOST_FOREACH (const FactorType::value_type& pin, factors) {
    BOOST_FOREACH (const FreqMap::value_type& freq, pin.second) {
      BOOST_FOREACH (const PwrMap::value_type& pwr, freq.second) {
        BOOST_FOREACH (const SiteMap::value_type& si, pwr.second) {
          pt::ptree child;
          child.add("pin", pin.first);
          child.add("freq", freq.first);
          child.add("power", pwr.first);
          child.add("site", si.first);
          child.add("value", si.second);
          tree.add_child("factors.factor", child);
        }
      }
    }
  }
  pt::xml_writer_settings<char> settings('\t', 1);
  pt::write_xml(filename, tree, std::locale(), settings);
}

double CableLoss::get(const string& name, double freq, double power,
                      int site) const
{
  if (exist(name, freq, power, site))
    return factors.at(name).at(freq).at(power).at(site);
  return 0;
}

void CableLoss::set(const std::string& name, double freq, double power,
                    int site, double value)
{
  factors[name][freq][power][site] = value;
}

bool CableLoss::empty() const { return factors.empty(); }

void CableLoss::init() { factors.clear(); }

bool CableLoss::exist(const std::string& name, double freq, double power,
                      int site) const
{
  // clang-format off
  return factors.count(name) and
      factors.at(name).count(freq) and
      factors.at(name).at(freq).count(power) and
      factors.at(name).at(freq).at(power).count(site);
  // clang-format off
}

void CableLoss::printConfig() const {}

void CableLoss::print() const
{
  std::ostringstream oss;

  BOOST_FOREACH (const FactorType::value_type& pin, factors) {
    BOOST_FOREACH (const FreqMap::value_type& freq, pin.second) {
      BOOST_FOREACH (const PwrMap::value_type& pwr, freq.second) {
        BOOST_FOREACH (const SiteMap::value_type& si, pwr.second) {
          // clang-format off
          oss << "pin: " << pin.first << ";\t"
              << "freq: " << freq.first << ";\t"
              << "power: " << pwr.first << ";\t"
              << "site: " << si.first << ";\t"
              << "factor: " << si.second << ";\t"
              << endl;
          // clang-format on
        }
      }
    }
  }
  cout << oss.str();
}
