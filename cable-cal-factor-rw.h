#pragma once
#include <map>
#include <string>
#include <vector>

class CableLoss
{
 public:
  struct CableConfig
  {
    std::string pin;
    double gain;
    double atten;
    std::vector<double> powers;
  };

 private:
  typedef std::map<int, double> SiteMap;
  typedef std::map<double, SiteMap> PwrMap;
  typedef std::map<double, PwrMap> FreqMap;
  // [pin][freq][power][site] => double
  typedef std::map<std::string, FreqMap> FactorType;
  // [pin][gain][atten][power] => double
  // typedef std::map<std::string, std::map<double, std::map<double, std::map<int, double> > > > FactorType;
 public:
  /** @brief load cal data from file.*/
  void load(const std::string& filename);
  /** @brief save cal data to file.*/
  void save(const std::string& filename) const;

  /** @brief print cal factors. */
  void print() const;
  /** @brief print pin config. */
  void printConfig() const;
  /** @brief get cal factor with pin/freq/power/site.
   *  return 0 if no cal factor was found.
   * */
  double get(const std::string& name, double freq, double power,
             int site) const;
  /** @brief set cal factor with pin/freq/power/site. */
  void set(const std::string& name, double freq, double power, int site,
           double value);
  /** @brief return true if empty. */
  bool empty() const;
  /** @brief initialize and clear data. */
  void init();

 private:
  bool exist(const std::string& name, double freq, double power,
             int site) const;

 private:
  int time;  // cal date
  FactorType factors;
  std::vector<CableConfig> configs;
};
