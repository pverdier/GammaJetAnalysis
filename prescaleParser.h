#ifndef PRESCALEPARSER_H
#define PRESCALEPARSER_H

#include <string>
#include <map>

class PrescaleParser
{
 public:
  
  PrescaleParser(const std::string& filename);
  ~PrescaleParser();
  
  void Init();
  float getPrescale(const unsigned int& runnumber) const;
  
 private:
  std::string _filename;
  std::map<unsigned int,float> _data;

};

#endif
