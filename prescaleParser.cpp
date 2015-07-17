#include "prescaleParser.h"

#include <fstream>
#include <iostream>

using namespace std;

PrescaleParser::PrescaleParser(const std::string& filename)
{
  _filename = filename;
  Init();
}

PrescaleParser::~PrescaleParser()
{
}

void PrescaleParser::Init()
{
  cout << "   -> PrescaleParser::Init() from " << _filename << endl;
  //  190645 2486 [10-110] 0.541689 HLT_Photon135_v4 1 L1_SingleEG30 1 0.541689
  
  ifstream infile(_filename.c_str());
  unsigned int a; // run
  unsigned int b; // fill
  string c;       // lumiblocks
  float d;        // delivered lumi
  string e;       // trigger name
  unsigned int f; // HLT prescale
  string g;       // L1 seed
  unsigned int h; // L1 prescale
  float i;        // recorded lumi
  
  while (infile >> a >> b >> c >> d >> e >> f >> g >> h >> i) {
    _data[a] = (float)(f*h);
    //cout << a << b << c << d << e << f << g << h << i << endl;
  }

}

float PrescaleParser::getPrescale(const unsigned int& runnumber) const
{
  float prescale = -1.;

  map<unsigned int,float>::const_iterator it = _data.find(runnumber);
  if ( it != _data.end() ) 
    prescale = it->second;
  else
    cout << "Prescale::getPrescale(): " << runnumber << " not found ... return -1" << endl;
  
  return prescale;
}
