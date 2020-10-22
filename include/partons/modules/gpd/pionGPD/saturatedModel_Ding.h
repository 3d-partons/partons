#ifndef PIONGPD_DING
#define PIONGPD_DING


/**
 * @file saturatedModel_Ding.h
 * @author José Manuel Morgado Chavez (University Of Huelva)
 * @author Cédric Mezrag (CEA Saclay)
 * @date 2020
 * @version 1.0
 */


#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "GPDModule.h"

namespace PARTONS {

class saturatedModel_Ding: public PARTONS::GPDModule {
public:
    /**
     * ID assigned by BaseObjectRegistry.
     */
    static const unsigned int classId;
    /**
     * Default constructor.
     * @param className Name of class.
     */
    saturatedModel_Ding(const std::string& className);
    /**
     * Destructor.
     */
    virtual ~saturatedModel_Ding();
    virtual saturatedModel_Ding* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
protected:
    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    saturatedModel_Ding(const saturatedModel_Ding& other);
    virtual void isModuleWellConfigured();
    virtual void initModule();
    virtual PARTONS::PartonDistribution computeH();

};


} //end of namespace
