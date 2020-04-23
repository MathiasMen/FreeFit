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
    std::string getOptionString() override;
  };
}
