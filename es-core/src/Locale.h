#pragma once
#ifndef ES_CORE_LOCALE_H
#define ES_CORE_LOCALE_H

#include <pugixml.hpp>
#include "utils/FileSystemUtil.h"

#include <vector>
#include <string>
#include <map>
#include <locale>

class Locale
{
  public:
    static Locale* getInstance();
    std::string gettext(std::string key);
    void setLanguage(std::string language);
    std::vector<std::string> getLanguages();
  private:
    static Locale* sInstance;
    std::string language;
    std::map<std::string, std::string> localeMap;
    std::vector<std::string> languages;
    
    Locale();
};

#endif // ES_CORE_LOCALE_H