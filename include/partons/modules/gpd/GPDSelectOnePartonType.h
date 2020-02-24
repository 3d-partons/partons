#ifndef GPDSELECTONEPARTONTYPE_H
#define GPDSELECTONEPARTONTYPE_H

/**
 * @file GPDSelectOnePartonType.h
 * @author Pawel Sznajder (IPNO)
 * @date March 29, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "GPDModule.h"

namespace PARTONS {

/**
 * @class GPDSelectOnePartonType
 *
 * @brief This model allows to select a single parton type.
 */
class GPDSelectOnePartonType: public GPDModule {

public:

    static const std::string PARAMETER_NAME_SELECTONEPARTONTYPE_PARTONTYPE; ///< Name of parameter to set parton type via configure()

    static const unsigned int classId;    ///< ID assigned by BaseObjectRegistry

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GPDSelectOnePartonType(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~GPDSelectOnePartonType();

    virtual GPDSelectOnePartonType* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    /**
     * Set underlying GPD module.
     */
    const GPDModule* getGPDModule() const;

    /**
     * Get  underlying GPD module.
     */
    void setGPDModule(GPDModule* pGPDModule);

    /**
     * Get parton type.
     */
    const std::string& getPartonType() const;

    /**
     * Set parton type.
     */
    void setPartonType(const std::string& partonType);

protected:

    /** Copy constructor.
     @param other Object to be copied.
     */
    GPDSelectOnePartonType(const GPDSelectOnePartonType& other);

    virtual PartonDistribution computeH();

private:

    GPDModule* m_pGPDModule; ///< GPD module.
    std::string m_partonType; ///< Parton type.
};

} /* namespace PARTONS */

#endif /* GPDSELECTONEPARTONTYPE_H */
