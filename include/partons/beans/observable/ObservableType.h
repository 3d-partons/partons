#ifndef OBSERVABLE_TYPE_H
#define OBSERVABLE_TYPE_H

/**
 * @file ObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 September 2015
 * @version 1.0
 *
 * @class ObservableResult
 *
 * @brief
 */

#include <string>

class ObservableType {
public:
    enum Type {
        UNDEFINED = 0, PHI = 1, FOURIER = 2
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

#endif /* OBSERVABLE_TYPE_H */
