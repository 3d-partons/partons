/*
 * GPDMPSSW13NoGluons.h
 *
 *  Created on: Oct 5, 2015
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef GPDMPSSW13NOGLUONS_H
#define GPDMPSSW13NOGLUONS_H

#include <string>

#include "../GPDModule.h"

class GPDService;

class GPDMPSSW13NoGluons: public GPDModule {

public:

    static const unsigned int classId;

    GPDMPSSW13NoGluons(const std::string &className);

    virtual ~GPDMPSSW13NoGluons();

    virtual GPDMPSSW13NoGluons* clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual std::string toString();

protected:

    GPDMPSSW13NoGluons(const GPDMPSSW13NoGluons& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PartonDistribution computeH();

private:

    GPDService* pGPDService;
    GPDModule* MPSSW13model;
};

#endif /* GPDMPSSW13NOGLUONS_H */
