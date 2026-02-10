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

namespace PARTONS {

/**
 * @class BaseObjectData
 *
 * @brief Container to store data to be used by base objects.
 *
 * This class is a container to store data used by base objects in the automation process.
 * An object of this class is intended to be created by a XML parser during running PARTONS runtime from a specific XML file, which can be local or can be retrieved from a database.
 * Therefore, this class may be seen as a representation of a set of parameters encoded in a XML file, like:
 \code{.py}
 <kinematics type="GPDKinematic">
     <param name="x" value="0.1" />
     <param name="xi" value="0.05" />
     <param name="t" value="-0.3" />
     <param name="MuF2" value="8." />
     <param name="MuR2" value="8." />
 </kinematics>
 \endcode
 */
class BaseObjectData: public BaseObject {

public:

    /**
     * Default constructor.
     */
    BaseObjectData();

    /**
     * Assignment constructor.
     * @param moduleType Type of module intended to use parameters stored in this class.
     * @param moduleClassName Name of module intended to use parameters stored in this class.
     */
    BaseObjectData(const std::string &moduleType,
            const std::string &moduleClassName);

    /**
     * Destructor.
     */
    virtual ~BaseObjectData();

    /**
     * Add submodule, i.e. indicate that this BaseObjectData object has a tree-like structure.
     * @param moduleType Type of module intended to use parameters stored in this class.
     * @param moduleClassName Name of module intended to use parameters stored in this class.
     * @return Reference to added BaseObjectData object.
     */
    BaseObjectData& addSubModule(const std::string &moduleType,
            const std::string &moduleClassName);

    /**
     * Add submodule, i.e. indicate that this BaseObjectData object has a tree-like structure.
     * @param baseObjectData BaseObjectData object.
     */
    void addSubModule(const BaseObjectData& baseObjectData);

    /**
     * Add a single parameter.
     * @param parameter Parameter to be added.
     */
    void addParameter(const ElemUtils::Parameter &parameter);

    /**
     * Add a set of parameters.
     * @param parameters Parameters to be added.
     */
    void addParameters(const ElemUtils::Parameters &parameters);

    /**
     * Check if parameters of submodule of a given type are available.
     * @param moduleClassType Requested type of submodule.
     * @return True if parameters are available, otherwise false.
     */
    bool isAvailableSubModule(const std::string &moduleClassType) const;

    /**
     * Get BaseObjectData object indicated by the last call of BaseObjectData::isAvailableSubModule() function.
     * @return Reference to object marked by the last call of BaseObjectData::isAvailableSubModule() function.
     */
    const BaseObjectData& getLastAvailable() const; // Care ! Use it after isAvailable() to ensure that the iterator is in the map range.

    virtual std::string toString() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type of module intended to use parameters stored in this class.
     */
    const std::string& getModuleType() const;

    /**
     * Set type of module intended to use parameters stored in this class.
     */
    void setModuleType(const std::string& moduleType);

    /**
     * Get name of module intended to use parameters stored in this class.
     */
    const std::string& getModuleClassName() const;

    /**
     * Set name of module intended to use parameters stored in this class.
     */
    void setModuleClassName(const std::string& moduleClassName);

    /**
     * Get parameters stored in this class.
     */
    const ElemUtils::Parameters& getParameters() const;

    /**
     * Set parameters stored in this class.
     */
    void setParameters(const ElemUtils::Parameters& parameters);

    /**
     * Get map to build a tree-like structure of BaseObjectData objects to be used if submodules are used. The keys in the map indicate type of modules intended to use parameters stored in corresponding BaseObjectData objects.
     */
    const std::map<std::string, BaseObjectData>& getSubModules() const;

private:

    /**
     * Type of module intended to use parameters stored in this class.
     */
    std::string m_moduleType;

    /**
     * Name of module intended to use parameters stored in this class.
     */
    std::string m_moduleClassName;

    /**
     * Parameters stored in this class.
     */
    ElemUtils::Parameters m_parameters;

    /**
     * Map to build a tree-like structure of BaseObjectData objects to be used if submodules are used. The keys in the map indicate type of modules intended to use parameters stored in corresponding BaseObjectData objects.
     */
    std::map<std::string, BaseObjectData> m_subModules;

    //Mutable keyword is use too indicate that member can be modify in const function. In this case useful with iterator m_it & isAvailable() function.
    /**
     * Iterator of BaseObjectData::m_subModules map.
     */
    mutable std::map<std::string, BaseObjectData>::const_iterator m_it;
};

} /* namespace PARTONS */

#endif /* BASE_OBJECT_DATA_H */
