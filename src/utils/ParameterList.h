#ifndef PARAMETER_LIST_H
#define PARAMETER_LIST_H

/**
 * @file ParameterList.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 03 July 2014
 * @version 1.0
 *
 * @class ParameterList
 *
 * @brief
 */

#include <stddef.h>
#include <map>
#include <string>
#include <utility>

#include "../BaseObject.h"
#include "GenericType.h"

class ParameterList: public BaseObject {
public:
    ParameterList();

    template<class T>
    ParameterList(const std::string &parameterName, const T &parameterValue) :
            BaseObject("ParameterList") {
        add(parameterName, parameterValue);
    }

    virtual ~ParameterList();

    template<class T>
    void add(const std::string &parameterName, const T &parameterValue) {
        m_parameters.insert(
                std::make_pair(parameterName, GenericType(parameterValue)));
    }
    GenericType get(const std::string &parameterName) const;
    GenericType getLastAvailable() const;

    size_t size();

    bool isAvailable(const std::string &parameterName);

    virtual std::string toString() const;

private:
    // std::string : parameter name
    // GenericType : value of the parameter
    std::map<std::string, GenericType> m_parameters;

    std::map<std::string, GenericType>::const_iterator m_it;
};

#endif /* PARAMETER_LIST_H */
