#pragma once
#include <map>
#include <string>
#include <vector>

class CableLoss
{
 public:
  // [pin][freq][power][site] => double
  typedef std::map<std::string, std::map<double, std::map<double, std::map<int, double> > > > FactorType;
  // [pin][gain][atten][power] => double
  // typedef std::map<std::string, std::map<double, std::map<double, std::map<int, double> > > > FactorType;

  void load(const std::string& filename);
  void save(const std::string& filename);

  /** @brief print cal factors. */
  void print() const;
  /** @brief print pin config. */
  void printConfig() const;
  /** @grief get cal factor with pin/freq/power/site. */
  double getCalFactor(const std::string& name, double freq, double power, int site) const;

 private:
  int time;  // cal date
  FactorType factors;
};

class CableConfig
{
 public:
 private:
  std::string name;
  double gain;
  double atten;
  std::vector<double> powers;
};
