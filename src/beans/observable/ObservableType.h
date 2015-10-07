/*
 * ObservableType.h
 *
 *  Created on: Sep 28, 2015
 *      Author: debian
 */

#ifndef OBSERVABLETYPE_H_
#define OBSERVABLETYPE_H_

#include <string>

class ObservableType {
public:
    enum Type {
        UNDEFINED = 0, PHI, FOURIER
    };

    ObservableType();

    ObservableType(Type type);

    operator Type() const;

    std::string toString();

    std::string getShortName();

    ObservableType::Type getType() const;

    void setType(Type type);

private:
    ObservableType::Type m_type;
};

#endif /* OBSERVABLETYPE_H_ */
