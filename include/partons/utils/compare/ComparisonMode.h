/*
 * ComparisonMode.h
 *
 *  Created on: Nov 30, 2015
 *      Author: debian
 */

#ifndef COMPARISON_MODE_H
#define COMPARISON_MODE_H

/**
 * @file ComparisonMode.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 November 2015
 * @version 1.0
 *
 * @class ComparisonMode
 *
 * @brief
 */

#include <string>

class ComparisonMode {
public:
    enum Type {
        UNDEFINED = 0, EQUAL = 1, INTERSECTION = 2

    };

    ComparisonMode();

    ComparisonMode(Type type);

    operator Type() const;

    std::string toString() const;

    std::string getShortName() const;

    ComparisonMode::Type getType() const;

    void setType(Type type);

private:
    ComparisonMode::Type m_type;
};

#endif /* COMPARISONMODE_H_ */
