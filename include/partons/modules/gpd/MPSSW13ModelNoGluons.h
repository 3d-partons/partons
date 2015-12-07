/*
 * MPSSW13ModelNoGluons.h
 *
 *  Created on: Oct 5, 2015
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MPSSW13MODELNOGLUONS_H_
#define MPSSW13MODELNOGLUONS_H_

#include <string>

//#include <src/modules/gpd/MPSSW13Model.h>
//#include <src/services/GPDService.h>
#include "../GPDModule.h"

class GPDService;

class MPSSW13ModelNoGluons: public GPDModule {

public:

    static const unsigned int classId;

    MPSSW13ModelNoGluons(const std::string &className);

    virtual ~MPSSW13ModelNoGluons();

    virtual MPSSW13ModelNoGluons* clone() const;

    virtual void configure(ParameterList parameters);

    virtual std::string toString();

protected:

    MPSSW13ModelNoGluons(const MPSSW13ModelNoGluons& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PartonDistribution computeH();

private:

    GPDService* pGPDService;
    GPDModule* MPSSW13model;
};

#endif /* MPSSW13MODELNOGLUONS_H_ */
