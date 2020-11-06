#ifndef ALGEBRAICTOYMODEL_H
#define ALGEBRAICTOYMODEL_H


/**
 * @file algebraicToyModel.h
 * @author José Manuel Morgado Chavez (University Of Huelva)
 * @author Cédric Mezrag (CEA Saclay)
 * @date 2020
 * @version 1.0
 */


#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../GPDModule.h"

namespace PARTONS {

class algebraicToyModel: public PARTONS::GPDModule {
public:
    /**
     * ID assigned by BaseObjectRegistry.
     */
    static const unsigned int classId;
    /**
     * Default constructor.
     * @param className Name of class.
     */
    algebraicToyModel(const std::string& className);
    /**
     * Destructor.
     */
    virtual ~algebraicToyModel();
    virtual algebraicToyModel* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
protected:
    /**
     * Copy constructor.
     * @param other Object to be copied.
     * */

    algebraicToyModel(const algebraicToyModel& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();
    
    virtual PARTONS::PartonDistribution computeH();

private:

};


} //end of namespace

# endif