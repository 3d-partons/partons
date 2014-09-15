#include "StringUtils.h"

#include <stddef.h>
#include <algorithm> // pour transform#include <cctype>    // pour tolower et toupper#include <cstdlib>#include <cctype>#include <cstdlib>#include <cctype>#include <cstdlib>#include <cctype>#include <cstdlib>#include <iterator>#include <cctype>#include <cstdlib>#include <cctype>#include <cstdlib>#include <iterator>#include <cctype>
#include <cstdlib>
#include <iterator>
#include <sstream>   // pour ostringstreamstd::string StringUtils::EMPTY = "";bool StringUtils::is_empty(std::string* chaine) {return (chaine == NULL || *chaine == EMPTY) ? true : false;}

bool StringUtils::equals(std::string* chaine, std::string* chaine2) {
return (&chaine == &chaine2) ? true : false;
}

bool StringUtils::equalsIgnoreCase(const std::string &chaine,
    const std::string &chaine2, const bool &toUpperCase) {
std::string tempChaine = chaine;
std::string tempChaine2 = chaine2;

if (toUpperCase) {
    std::transform(tempChaine.begin(), tempChaine.end(), tempChaine.begin(),
            toupper);
    std::transform(tempChaine2.begin(), tempChaine2.end(), tempChaine2.begin(),
            toupper);
} else {
    std::transform(tempChaine.begin(), tempChaine.end(), tempChaine.begin(),
            tolower);
    std::transform(tempChaine2.begin(), tempChaine2.end(), tempChaine2.begin(),
            tolower);
}

return (tempChaine == tempChaine2) ? true : false;
}

void StringUtils::to_upperCase(std::string &chaine) {
std::transform(chaine.begin(), chaine.end(), chaine.begin(), toupper);
}

void StringUtils::to_lowerCase(std::string* chaine) {
std::transform(chaine->begin(), chaine->end(), chaine->begin(), tolower);
}

std::string StringUtils::fromIntToString(int number) {
std::ostringstream oss;
oss << number;

return oss.str();
}

int StringUtils::fromStringToInt(std::string chaine) {
return atoi(chaine.c_str());
}

float StringUtils::fromStringToFloat(std::string chaine) {
return static_cast<float>(atof(chaine.c_str()));
}

template<typename T>
std::string StringUtils::toString(T valeur) {
std::ostringstream oss;
oss << valeur;

return oss.str();
}

bool StringUtils::fromStringToBool(std::string chaine) {
to_upperCase(chaine);
return (chaine == "TRUE") ? true : false;
}

bool StringUtils::contains(std::string* chaine, std::string* searchString) {
return (chaine->find(*searchString) != chaine->length()) ? true : false;
}

bool StringUtils::containsIgnoreCase(std::string* chaine,
    std::string* searchString, bool toUpperCase) {
std::string tempChaine = *chaine;
std::string tempSearchString = *searchString;

if (toUpperCase) {
    std::transform(tempChaine.begin(), tempChaine.end(), tempChaine.begin(),
            toupper);
    std::transform(tempSearchString.begin(), tempSearchString.end(),
            tempSearchString.begin(), toupper);
} else {
    std::transform(tempChaine.begin(), tempChaine.end(), tempChaine.begin(),
            tolower);
    std::transform(tempSearchString.begin(), tempSearchString.end(),
            tempSearchString.begin(), tolower);
}

return (tempChaine.find(tempSearchString) != chaine->length()) ? true : false;
}

void StringUtils::replaceAll(std::string* chaine, std::string* searchString,
    std::string* replaceString) {
std::string newChaine = EMPTY;
size_t index = -1;

if (!is_empty(chaine)) {
    while (contains(chaine, searchString)) {
        index = chaine->find(*searchString);

        newChaine.append(*chaine, 0, index);
        newChaine.append(*replaceString);
        newChaine.append(*chaine, index + searchString->length(),
                chaine->length());

        chaine->erase(0, index + searchString->length());
    }

    newChaine.append(*chaine);
}

*chaine = newChaine;
}

void StringUtils::replaceAllIgnoreCase(std::string* chaine,
    std::string* searchString, std::string* replaceString, bool toUpperCase) {
/*
 std::string newChaine = EMPTY;

 std::string modifiedChaine = _chaine;
 std::string modifiedSearchString = _searchString;

 size_t index = -1;

 if( !is_empty(_chaine) )
 {

 if( toUpperCase )
 {
 std::transform(modifiedChaine.begin(),modifiedChaine.end(),modifiedChaine.begin(),std::toupper);
 std::transform(modifiedSearchString.begin(),modifiedSearchString.end(),modifiedSearchString.begin(),std::toupper);
 }
 else
 {
 std::transform(modifiedChaine.begin(),modifiedChaine.end(),modifiedChaine.begin(),std::tolower);
 std::transform(modifiedSearchString.begin(),modifiedSearchString.end(),modifiedSearchString.begin(),std::tolower);
 }

 while( contains(modifiedChaine, modifiedSearchString) )
 {
 index = modifiedChaine.find(modifiedSearchString);

 newChaine.append(_chaine, 0, index);
 newChaine.append(_replaceString );
 newChaine.append(_chaine, index + _searchString.length(), _chaine.length());

 _chaine.erase( 0, index + _searchString.length() );
 }

 newChaine.append(_chaine);
 }

 return newChaine;
 */
}

void StringUtils::trim(std::string &str, const std::string& whitespace) {
size_t strBegin = str.find_first_not_of(whitespace);
if (strBegin == std::string::npos) {
    str = ""; // no content
} else {
    size_t strEnd = str.find_last_not_of(whitespace);
    size_t strRange = strEnd - strBegin + 1;

    str = str.substr(strBegin, strRange);
}
}

int StringUtils::count(std::string* chaine, std::string* searchString) {
int resultat = 0;
size_t index = 0;

if (!is_empty(chaine)) {
    while (contains(chaine, searchString)) {
        ++resultat;

        index = chaine->find(*searchString);
        chaine->erase(0, index + searchString->length());
    }
}

return resultat;
}

std::vector<std::string> StringUtils::split(std::string chaine,
    const char & splitCharacter) {
std::vector<std::string> strings;
int index = 0;

for (unsigned int pos = 0; pos != chaine.size(); ++pos) {
    if (chaine.at(pos) == splitCharacter) {
        /*
         std::cout << "index = " << index << std::endl;
         std::cout << "pos = " << pos << std::endl;
         */

        strings.push_back(chaine.substr(index, pos - index));
        index = pos + 1;
    }
}
strings.push_back(chaine.substr(index, chaine.size() - index));

return strings;
}

std::string StringUtils::vectorToString(std::vector<std::string>* vector) {
std::string res = EMPTY;

for (unsigned int i = 0; i != vector->size(); ++i) {
    res.append(vector->at(i)).append(" ");
}

return res;
}
