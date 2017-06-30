/*
 * GPDGK11NoGluons.h
 *
 *  Created on: Oct 5, 2015
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef GPDGK11NOGLUONS_H
#define GPDGK11NOGLUONS_H

#include <string>

#include "../GPDModule.h"

class GPDService;

class GPDGK11NoGluons: public GPDModule {

public:

    static const unsigned int classId;

    GPDGK11NoGluons(const std::string &className);

    virtual ~GPDGK11NoGluons();

    virtual GPDGK11NoGluons* clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual std::string toString();

protected:

    GPDGK11NoGluons(const GPDGK11NoGluons& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();
    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeEt();

private:

    GPDService* pGPDService;
    GPDModule* GKmodel;
};

#endif /* GPDGK11NOGLUONS_H */
