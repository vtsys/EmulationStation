#include "Locale.h"

#include "Log.h"

#include "utils/FileSystemUtil.h"
#include <pugixml.hpp>

Locale* Locale::sInstance = NULL;

Locale::Locale()
{
	getLanguages();
}

Locale* Locale::getInstance()
{
	if(sInstance == NULL)
		sInstance = new Locale();

	return sInstance;
}

std::vector<std::string> Locale::getLanguages()
{
	if(languages.size() == 0) {
		const std::string path = Utils::FileSystem::getExePath() + "/locales/";
		Utils::FileSystem::stringList list;

		if(!Utils::FileSystem::exists(path))
		{
			LOG(LogError) << "";
			return languages;
		}

		list = Utils::FileSystem::getDirContent(path);

		for (auto iter = list.cbegin(); iter != list.cend(); iter++)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(iter->c_str());

			if(!result)
			{
				LOG(LogError) << "Could not parse Locale files!\n   " << result.description();
				return languages;
			}

			pugi::xml_node meta = doc.child("meta");
			languages.push_back(meta.child("name").text().get());
		}
	}
	
	return languages;
}

void Locale::setLanguage(std::string language)
{
	std::locale loc;
	Locale::language = language;

	const std::string path = Utils::FileSystem::getExePath() + "/locales/";
	Utils::FileSystem::stringList list;

	if(!Utils::FileSystem::exists(path))
	{
		LOG(LogError) << "Locale file not found! - " << path;
	}

	list = Utils::FileSystem::getDirContent(path);

	for (auto iter = list.cbegin(); iter != list.cend(); iter++)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(iter->c_str());

		if(!result)
		{
			LOG(LogError) << "Could not parse Locale files!\n   " << result.description();
		}

		pugi::xml_node meta = doc.child("meta");
		std::string languageName = meta.child("name").text().get();

		if(languageName.compare(language) == 0)
		{
			for(pugi::xml_node node = doc.child("string"); node; node = node.next_sibling("string"))
				Locale::localeMap[node.attribute("key").as_string()] = node.text().get();
		}
	}
}

std::string Locale::gettext(std::string key)
{
	if(Locale::localeMap.find(key) == Locale::localeMap.cend())
	{
		LOG(LogError) << "Tried to use undefined locale key - " << key << "!";
		return "--";

	}
	return Locale::localeMap[key];
}