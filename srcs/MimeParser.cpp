#include "MimeParser.hpp"

MimeParser::mimeMap_t MimeParser::parseMimeFile(const std::string &mimeFile)
{
	std::ifstream ifs(mimeFile.c_str(), std::ios::binary);
	std::string   line;

	MimeParser::mimeMap_t mimeMap;

	if (!ifs.is_open())
		throw FileNotFoundException();

	while (std::getline(ifs, line)) 
	{
		// Skip blank lines
		if (line.find_first_not_of(' ') == line.npos || line.at(0) == '\n')
			continue;
		
		// Content-type is everything between start and first space
		std::string contentType = line.substr(0, line.find_first_of(' '));
		// Extension is everything between last space and end or ';'
		if (line.find_first_of(';') == line.npos)
			throw InvalidFileException();

		// From first space to ending ';' both excluded, then split on space for all extensions with same type
		std::string extensionsStr = line.substr(line.find_first_of(' ') + 1);
		extensionsStr.erase(extensionsStr.length() - 1);
		std::vector<std::string> extensionsVec = Utils::split(extensionsStr, ' ');

		for (std::vector<std::string>::const_iterator it = extensionsVec.begin(); it != extensionsVec.end(); ++it)
		{
			std::string extension = *it;
			mimeMap[extension] = contentType;
		}
	}
	ifs.close();

	#if DEBUG
	std::cout << "MIME Types: [";
	for (MimeParser::mimeMap_t::const_iterator it = mimeMap.begin(); it != mimeMap.end(); ++it)
	{
		std::cout << it->first << ":'" << it->second << "']";
		if (it != --mimeMap.end())
			std::cout << ", [";
		else
			std::cout << "]";
	}
	std::cout << "}\n" << std::endl;
	#endif

	return mimeMap;
}

MimeParser::MimeParser(const std::string &mimeFile)
{
	if (mimeMap.empty())
		this->parseMimeFile(mimeFile);
}

MimeParser::mimeMap_t MimeParser::mimeMap = MimeParser::parseMimeFile(MIME_MAP_FILE);