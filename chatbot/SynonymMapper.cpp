#include "synonymmapper.h"

#include <fstream>
#include <sstream>
#include <algorithm>

SynonymMapper::SynonymMapper()
{
    loadFromFile("data/synonyms.txt");
}

std::string SynonymMapper::toLower(const std::string& str) const
{
    std::string result = str;

    std::transform(result.begin(),
                   result.end(),
                   result.begin(),
                   [](unsigned char c)
                   {
                       return std::tolower(c);
                   });

    return result;
}

bool SynonymMapper::loadFromFile(const std::string& filename)
{
    synonymMap.clear();
    phraseMap.clear();

    std::ifstream file(filename);

    if(!file.is_open())
        return false;

    std::string line;

    while(getline(file, line))
    {
        if(line.empty() || line[0] == '#')
            continue;

        size_t equalPos = line.find('=');

        if(equalPos == std::string::npos)
            continue;

        std::string canonical = toLower(line.substr(0, equalPos));

        // Canonical word maps to itself
        synonymMap[canonical] = canonical;

        std::stringstream ss(line.substr(equalPos + 1));

        std::string synonym;

        while(getline(ss, synonym, ','))
        {
            // Trim whitespace
            synonym.erase(0, synonym.find_first_not_of(" \t"));

            synonym.erase(synonym.find_last_not_of(" \t") + 1);

            synonym = toLower(synonym);

            if(synonym.empty())
                continue;

            // Multi-word synonym
            if(synonym.find(' ') != std::string::npos)
            {
                phraseMap[synonym] = canonical;
            }
            else
            {
                synonymMap[synonym] = canonical;
            }
        }
    }

    return true;
}

std::string SynonymMapper::normalizeWord(const std::string& word) const
{
    std::string lower = toLower(word);

    auto it = synonymMap.find(lower);

    if(it != synonymMap.end())
        return it->second;

    return lower;
}

std::string SynonymMapper::normalizeSentence(const std::string& sentence) const
{
    // Convert to lowercase first
    std::string normalized = toLower(sentence);

    // Replace multi-word phrases first
    for(const auto& entry : phraseMap)
    {
        const std::string& phrase = entry.first;
        const std::string& canonical = entry.second;

        size_t pos = 0;

        while((pos = normalized.find(phrase, pos)) != std::string::npos)
        {
            normalized.replace(pos,
                               phrase.length(),
                               canonical);

            pos += canonical.length();
        }
    }

    // Replace single words
    std::stringstream input(normalized);
    std::stringstream output;

    std::string word;

    bool first = true;

    while(input >> word)
    {
        if(!first)
            output << " ";

        output << normalizeWord(word);

        first = false;
    }

    return output.str();
}