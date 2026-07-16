#ifndef SYNONYMMAPPER_H
#define SYNONYMMAPPER_H

#include <string>
#include <unordered_map>

class SynonymMapper
{
public:
    SynonymMapper();

    bool loadFromFile(const std::string& filename);

    std::string normalizeWord(const std::string& word) const;

    std::string normalizeSentence(const std::string& sentence) const;

private:
    std::unordered_map<std::string, std::string> synonymMap;
    std::unordered_map<std::string, std::string> phraseMap;

    std::string toLower(const std::string& str) const;
};

#endif