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
#include <ctime>

using namespace std;
namespace pt = boost::property_tree;

CableLoss::Key::Key(string pinName, double frequency, double power, int site)
    : pin(pinName), freq(frequency), power(power), site(site)
{
}

bool CableLoss::Key::operator<(const Key& rhs) const
{
  if (pin < rhs.pin) {
    return true;
  } else if ( pin > rhs.pin ) {
    return false;
  }

  if ( freq < rhs.freq ) {
    return true;
  } else if ( freq > rhs.freq ) {
    return false;
  }

  if ( power < rhs.power ) {
    return true;
  } else if ( power > rhs.power ) {
    return false;
  }

  if ( site < rhs.site ) {
    return true;
  } else if ( site > rhs.site ) {
    return false;
  }
  return false;
}

bool CableLoss::Key::operator==(const Key& rhs) const
{
  return pin == rhs.pin and freq == rhs.freq and power == rhs.power and
         site == rhs.site;
}

std::ostream& CableLoss::Key::operator<<(std::ostream& os) const
{
  return os << pin << ", " << freq << ", " << power << ", " << site << endl;
}

void CableLoss::load(const string& filename)
{
  pt::ptree tree;
  pt::read_xml(filename, tree, pt::xml_parser::trim_whitespace);
  time = tree.get("caltime", 0);

  BOOST_FOREACH (pt::ptree::value_type& v, tree.get_child("")) {
    if (v.first == "info") {
      string pin = v.second.get<string>("pin");
      double gain = v.second.get<double>("gain");
      double atten = v.second.get<double>("atten");

      cout << "pin: " << pin << "\tgain: " << gain << "\tatten: " << atten
           << "\tpowers:";

      BOOST_FOREACH (pt::ptree::value_type& f, v.second.get_child("powers")) {
        cout << " " << f.second.get<double>("");
      }
      cout << endl;
    } else if (v.first == "factors") {
      string pin = v.second.get<string>("pin");
      double freq = v.second.get<double>("freq");
      double power = v.second.get<double>("power");
      int site = v.second.get<int>("site");
      double value = v.second.get<double>("value");

      factors[Key(pin, freq, power, site)] = value;
    }
  }
}

void CableLoss::save(const string& filename)
{
  pt::ptree tree;
  time_t now;
  std::time(&now);
  tree.put("caltime", long(now));

  for (auto it = factors.begin(); it != factors.end(); ++it) {
    pt::ptree child;
    child.add("pin", it->first.pin);
    child.add("freq", double(it->first.freq));
    child.add("power", it->first.power);
    child.add("site", it->first.site);
    child.add("value", it->second);
    tree.add_child("factors", child);
  }
  pt::xml_writer_settings<char> settings('\t', 1);
  pt::write_xml(filename, tree, std::locale(), settings);
}

double CableLoss::get(const string& name, double freq, double power,
                      int site) const
{
  Key key(name, freq, power, site);
  return factors.at(key);
}

double CableLoss::get(const Key& key) const { return factors.at(key); }

void CableLoss::printConfig() const {}

void CableLoss::print() const
{
  ostringstream oss;
  for (FactorType::const_iterator it = factors.cbegin(); it != factors.cend();
       ++it) {
    oss << "pin: " << it->first.pin << ";\t"
        << "freq: " << it->first.freq << ";\t"
        << "power: " << it->first.power << ";\t"
        << "site: " << it->first.site << ";\t"
        << "factor: " << it->second << ";\t" << endl;
  }
  cout << oss.str();
}
