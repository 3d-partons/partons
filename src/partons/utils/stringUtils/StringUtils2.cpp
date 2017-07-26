///*
// * StringUtils2.cpp
// *
// *  Created on: 2 févr. 2015
// *      Author: bryan
// */
//
//#include "StringUtils2.h"
//
//#include <cmath>
//#include <cstdlib>
//
//#include "StringUtils.h"

namespace PARTONS {

//
//// Global static pointer used to ensure a single instance of the class.
//StringUtils2* StringUtils2::m_pInstance = 0;
//
//StringUtils2::StringUtils2() {
//    createConversionMapFromCharToDouble();
//}
//
//StringUtils2* StringUtils2::getInstance() {
//    // Only allow one instance of class to be generated.
//    if (!m_pInstance) {
//        m_pInstance = new StringUtils2();
//    }
//
//    return m_pInstance;
//}
//
//StringUtils2::~StringUtils2() {
//    if (m_pInstance != 0) {
//        delete m_pInstance;
//        m_pInstance = 0;
//    }
//}
//
//void StringUtils2::createConversionMapFromCharToDouble() {
//    std::map<char, double> conversionMap;
//    m_charToDoubleConversionTable['0'] = 0.;
//    m_charToDoubleConversionTable['1'] = 1.;
//    m_charToDoubleConversionTable['2'] = 2.;
//    m_charToDoubleConversionTable['3'] = 3.;
//    m_charToDoubleConversionTable['4'] = 4.;
//    m_charToDoubleConversionTable['5'] = 5.;
//    m_charToDoubleConversionTable['6'] = 6.;
//    m_charToDoubleConversionTable['7'] = 7.;
//    m_charToDoubleConversionTable['8'] = 8.;
//    m_charToDoubleConversionTable['9'] = 9.;
//}
//
//double StringUtils2::convertFromStringToDouble(const std::string &string) {
//    double result = 0.;
//
//    std::vector<char> floatingPointVector;
//
//    std::vector<std::string> doubleStringSplittedByPoint = StringUtils::split(
//            string, '.');
//
//    bool isMinus = false;
//    std::string realPart = "";
//    std::string floatingPart = "";
//
//    unsigned int vectorSize = doubleStringSplittedByPoint.size();
//
//    if (vectorSize == 1) {
//        realPart = doubleStringSplittedByPoint[0];
//
//        if (realPart.size() > 0) {
//            if (realPart.at(0) == '-') {
//                isMinus = true;
//            }
//        }
//    }
//    if (vectorSize == 2) {
//        realPart = doubleStringSplittedByPoint[0];
//        floatingPart = doubleStringSplittedByPoint[1];
//
//        if (realPart.size() > 0) {
//            if (realPart.at(0) == '-') {
//                isMinus = true;
//            }
//        }
//    }
//
//    for (unsigned int i = 0; i != floatingPart.size(); i++) {
//        floatingPointVector.push_back(floatingPart.at(i));
//    }
//
//    result += strtod(realPart.c_str(), NULL);
//
//    for (unsigned int i = 0; i != floatingPointVector.size(); i++) {
//
//        result += m_charToDoubleConversionTable[floatingPointVector[i]]
//                / (pow10(i + 1));
//    }
//
//    if (isMinus) {
//        result *= -1.;
//    }
//
//    return result;
//}

} /* namespace PARTONS */
