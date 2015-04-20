/*
 * StringUtils2.h
 *
 *  Created on: 2 févr. 2015
 *      Author: bryan
 */

#ifndef STRINGUTILS2_H_
#define STRINGUTILS2_H_

#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

class StringUtils2 {
public:
    /**
     * Share a unique pointer of this class
     */
    static StringUtils2* getInstance();

    virtual ~StringUtils2();

    double convertFromStringToDouble(const std::string &string);

    template<class T>
    bool fromString(T& t, const std::string& s,
            std::ios_base& (*f)(std::ios_base&))
    {
        std::istringstream iss(s);
            return !(iss >> f >> t).fail();
    }
private:

    /**
     * Private pointer of this class for a unique instance
     */
    static StringUtils2* m_pInstance;

    /**
     * Private default constructor for a unique instance
     */
    StringUtils2();

    std::map<char, double> m_charToDoubleConversionTable;
    std::map<char, double>::iterator m_it;

    void createConversionMapFromCharToDouble();

    static std::pair<std::string, std::vector<char> > fromStringToArrayOfChar(
            const std::string &string);
};

#endif /* STRINGUTILS2_H_ */
