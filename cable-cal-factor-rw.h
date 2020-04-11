#pragma once
#include <map>
#include <string>
#include <vector>

class CableLoss
{
 public:
  struct Key
  {
    Key(std::string pinName, double frequency, double power, int site);
    std::string pin;
    double freq;
    double power;
    int site;
    bool operator<(const Key& rhs) const;
    bool operator==(const Key& rhs) const;
    std::ostream& operator<<(std::ostream& os) const;
  };
  // [ (pin, freq, power, site) ] => double
  typedef std::map<Key, double> FactorType;

  void load(const std::string& filename);
  void save(const std::string& filename);

  /** @brief print cal factors. */
  void print() const;
  /** @brief print pin config. */
  void printConfig() const;
  /** @grief get cal factor with pin/freq/power/site. */
  double get(const Key& key) const;
  double get(const std::string& name, double freq, double power,
             int site) const;

 private:
  int time;  // cal date
  std::map<Key, double> factors;
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
