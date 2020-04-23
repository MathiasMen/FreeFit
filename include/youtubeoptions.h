#pragma once

#include <list>
#include <string>

namespace VideoDownload
{
  enum VideoType {Default, MP4, MP3};

  class YoutubeOptions
  {
  public:
    YoutubeOptions() : v_t(VideoType::Default){}

    virtual std::string getOptionString() = 0;
    
    void setVideoFormat(VideoType t){v_t = t;}
    void addAdditionalOption(std::string s){additional_options.push_back(s);}
    
  protected:
    std::list<std::string> additional_options;
    VideoType v_t;
  };

  class YoutubeDLOptions : public YoutubeOptions
  {
  public:
    std::string getOptionString() override
    {
      std::string res = {};
      switch(v_t)
      {
        case VideoType::MP4 :
          res += " -f 'bestvideo[ext=mp4]+bestaudio[ext=m4a]'";
        case VideoType::MP3 :
          res += "";
        default:
          res += "";
      }

      for(std::string s : additional_options)
        res += std::string(" ") + s;

      return res;
    }
  };
}
