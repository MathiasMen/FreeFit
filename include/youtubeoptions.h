#pragma once

#include <list>
#include <string>

class YoutubeOptions
{
public:
  std::string getOptions()
  {
    std::string res = {};
    for(std::string s : options)
      res += std::string(" ") + s;
    return res;
  };
  void addOptions(std::string s){this->options.push_back(s);}
private:
  std::list<std::string> options;
};
