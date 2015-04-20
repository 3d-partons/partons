#include "StringUtils.h"

#include <stddef.h>
#include <algorithm> // pour transform#include <cctype>    // pour tolower et toupper#include <cstdlib>#include <cctype>#include <cstdlib>#include <cctype>#include <cstdlib>#include <cctype>#include <cstdlib>#include <iterator>#include <cctype>#include <cstdlib>#include <cctype>#include <cstdlib>#include <iterator>#include <cctype>#include <cctype>#include <cstdlib>#include <cctype>#include <cstdlib>#include <cctype>#include <cstdlib>#include <cctype>#include <cctype>#include <cstdlib>#include <cctype>#include <cctype>#include <cctype>#include <cstdlib>#include <iterator>#include <cctype>#include <cstdlib>#include <cctype>#include <cctype>#include <cstdlib>#include <cctype>#include <cctype>#include <cmath>#include <cctype>#include <cstdlib>#include <cctype>#include <cstdlib>#include <cctype>#include <cstdlib>
#include <iterator>

//#include <utility>

//#include "Formatter.h"

std::string StringUtils::EMPTY = "";

bool StringUtils::isEmpty(const std::string & str) {

    return (str == EMPTY) ? true : false;
}

bool StringUtils::equals(const std::string & str, const std::string & str2) {
    return (str == str2) ? true : false;
}

bool StringUtils::equalsIgnoreCase(const std::string &chaine,
        const std::string &chaine2, const bool &toUpperCase) {
    std::string tempChaine = chaine;
    std::string tempChaine2 = chaine2;

    if (toUpperCase) {
        std::transform(tempChaine.begin(), tempChaine.end(), tempChaine.begin(),
                toupper);
        std::transform(tempChaine2.begin(), tempChaine2.end(),
                tempChaine2.begin(), toupper);
    } else {
        std::transform(tempChaine.begin(), tempChaine.end(), tempChaine.begin(),
                tolower);
        std::transform(tempChaine2.begin(), tempChaine2.end(),
                tempChaine2.begin(), tolower);
    }

    return (tempChaine == tempChaine2) ? true : false;
}

void StringUtils::to_upperCase(std::string &chaine) {
    std::transform(chaine.begin(), chaine.end(), chaine.begin(), toupper);
}

void StringUtils::to_lowerCase(std::string* chaine) {
    std::transform(chaine->begin(), chaine->end(), chaine->begin(), tolower);
}

//std::string StringUtils::fromIntToString(int number) {
//    std::ostringstream oss;
//    oss << number;
//
//    return oss.str();
//}

int StringUtils::fromStringToInt(std::string chaine) {
    return atoi(chaine.c_str());
}

//unsigned int StringUtils::fromStringToUnsignedInt(const std::string & chaine) {
//    return stoul(chaine.c_str());
//}

float StringUtils::fromStringToFloat(std::string chaine) {
    return static_cast<float>(atof(chaine.c_str()));
}

double StringUtils::fromStringToDouble(const std::string & str) {
    //return *reinterpret_cast<double*>(str);

    //    std::stringstream stream;
//    stream << str;
//    double d = 0.;
//    stream >> d;
//    return d;

//TODO http://stackoverflow.com/questions/22543349/what-is-wrong-with-the-following-code-it-converts-double-to-string-without-usin

// deprecated
// return atof(str.c_str());

    return strtod(str.c_str(), NULL);
}

std::pair<std::string, std::vector<char> > StringUtils::fromStringToArrayOfChar(
        const std::string &string) {

    std::pair<std::string, std::vector<char> > result;
    std::vector<char> floatingPointVector;

    std::vector<std::string> doubleStringSplittedByPoint = StringUtils::split(
            string, '.');

    std::string realPart = "";
    std::string floatingPart = "";

    unsigned int vectorSize = doubleStringSplittedByPoint.size();

    if (vectorSize == 1) {
        realPart = doubleStringSplittedByPoint[0];
    }
    if (vectorSize == 2) {
        realPart = doubleStringSplittedByPoint[0];
        floatingPart = doubleStringSplittedByPoint[1];
    }

    for (unsigned int i = 0; i != floatingPart.size(); i++) {
        floatingPointVector.push_back(floatingPart.at(i));
    }

    result = std::make_pair(realPart, floatingPointVector);

    return result;
}

bool StringUtils::fromStringToBool(std::string chaine) {
    to_upperCase(chaine);
    return (chaine == "TRUE") ? true : false;
}

bool StringUtils::contains(const std::string & str,
        const std::string & searchString) {
    return (str.find(searchString) != std::string::npos) ? true : false;
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

    return (tempChaine.find(tempSearchString) != std::string::npos) ?
            true : false;
}

void StringUtils::replaceAll(std::string & str,
        const std::string & searchString, const std::string & replaceString) {
    std::string newChaine = EMPTY;
    size_t index = -1;

    if (!isEmpty(str)) {
        while (contains(str, searchString)) {
            index = str.find(searchString);

            newChaine.append(str, 0, index);
            newChaine.append(replaceString);
            newChaine.append(str, index + searchString.length(), str.length());

            str.erase(0, index + searchString.length());
        }

        newChaine.append(str);
    }

    str = newChaine;
}

void StringUtils::replaceAllIgnoreCase(std::string* chaine,
        std::string* searchString, std::string* replaceString,
        bool toUpperCase) {
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

unsigned int StringUtils::count(std::string str,
        const std::string & searchString) {
    unsigned int resultat = 0;
    size_t index = 0;

    if (!isEmpty(str)) {
        while (contains(str, searchString)) {
            ++resultat;

            index = str.find(searchString);
            str.erase(0, index + searchString.length());
        }
    }

    return resultat;
}

std::vector<std::string> StringUtils::split(const std::string &chaine,
        const char splitCharacter) {
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
