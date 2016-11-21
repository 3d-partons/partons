#ifndef BASE_OBJECT_DATA_H
#define BASE_OBJECT_DATA_H

/**
 * @file BaseObjectData.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../BaseObject.h"

namespace ElemUtils {
class Parameter;
} /* namespace ElemUtils */

/**
 * @class BaseObjectData
 * @brief
 */
class BaseObjectData: public BaseObject {
public:
    BaseObjectData();
    BaseObjectData(const std::string &moduleType,
            const std::string &moduleClassName);
    virtual ~BaseObjectData();

    BaseObjectData& addSubModule(const std::string &moduleType,
            const std::string &moduleClassName);
    void addParameter(const ElemUtils::Parameter &parameter);
    void addParameters(const ElemUtils::Parameters &parameters);

    bool isAvailableSubModule(const std::string &moduleClassType) const;
    const BaseObjectData& getLastAvailable() const; /// Care ! Use it after isAvailable() to ensure that the iterator is in the map range.

    virtual std::string toString() const;

    const std::string& getModuleClassName() const;
    void setModuleClassName(const std::string& moduleClassName);
    const std::string& getModuleType() const;
    void setModuleType(const std::string& moduleType);
    const ElemUtils::Parameters& getParameters() const;
    void setParameters(const ElemUtils::Parameters& parameters);
    const std::map<std::string, BaseObjectData>& getSubModules() const;

private:
    std::string m_moduleType;
    std::string m_moduleClassName;

    ElemUtils::Parameters m_parameters;

    std::map<std::string, BaseObjectData> m_subModules;

    // mutable keyword is use too indicate that member can be modify in const function. In this case useful with iterator m_it & isAvailable() function.
    mutable std::map<std::string, BaseObjectData>::const_iterator m_it;
};

#endif /* BASE_OBJECT_DATA_H */
