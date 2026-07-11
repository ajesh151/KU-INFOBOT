#include "synonymmapper.h"

#include <fstream>
#include <sstream>
#include <algorithm>

SynonymMapper::SynonymMapper()
{}

std::string SynonymMapper::toLower(const std::string& str) const
{
    std::string result = str;

    std::transform(result.begin(), result.end(),
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

    std::ifstream file(filename);

    if (!file.is_open())
        return false;

    std::string line;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        if (line[0] == '#')
            continue;

        size_t equalPos = line.find('=');

        if (equalPos == std::string::npos)
            continue;

        std::string canonical = line.substr(0, equalPos);
        canonical = toLower(canonical);

        synonymMap[canonical] = canonical;

        std::string synonymList = line.substr(equalPos + 1);

        std::stringstream ss(synonymList);

        std::string synonym;

        while (getline(ss, synonym, ','))
        {
            synonym = toLower(synonym);

            if (!synonym.empty())
                synonymMap[synonym] = canonical;
        }
    }

    return true;
}

std::string SynonymMapper::normalizeWord(const std::string& word) const
{
    std::string lower = toLower(word);

    auto it = synonymMap.find(lower);

    if (it != synonymMap.end())
        return it->second;

    return lower;
}

std::string SynonymMapper::normalizeSentence(const std::string& sentence) const
{
    std::stringstream input(sentence);

    std::stringstream output;

    std::string word;

    bool first = true;

    while (input >> word)
    {
        if (!first)
            output << " ";

        output << normalizeWord(word);

        first = false;
    }

    return output.str();
}