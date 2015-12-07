/*
 * SortingMode.h
 *
 *  Created on: Dec 7, 2015
 *      Author: debian
 */

#ifndef SORTING_MODE_H
#define SORTING_MODE_H

#include <string>

class SortingMode {
public:

    enum Type {
        UNDEFINED = 0, ASCENDING = 1, DESCENDING = 2
    };

    SortingMode();

    SortingMode(Type type);

    operator Type() const;

    std::string toString();

    std::string getShortName();

    SortingMode::Type getType() const;

    void setType(Type type);

private:
    SortingMode::Type m_type;
};

#endif /* SORTING_MODE_H */
