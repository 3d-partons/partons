#ifndef BASE_TYPE_H
#define BASE_TYPE_H

/**
 * @file BaseType.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 19 January 2016
 * @version 1.0
 *
 * @class BaseType
 *
 * @brief
 */

#include <string>

class BaseType {
public:
    BaseType();
    BaseType(bool initialized);

    virtual ~BaseType();

    bool isInitialized() const;
    void setInitialized(bool initialized);
    const std::string& getUnit() const;
    void setUnit(const std::string& unit);

private:
    bool m_initialized;
    std::string m_unit;
};

#endif /* BASE_TYPE_H */
