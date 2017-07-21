/*
 * GK11ModelNoGluons.h
 *
 *  Created on: Oct 5, 2015
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef GK11MODELNOGLUONS_H_
#define GK11MODELNOGLUONS_H_

#include <string>

#include "../GPDModule.h"

class GPDService;

class GK11ModelNoGluons: public GPDModule {

public:

    static const unsigned int classId;

    GK11ModelNoGluons(const std::string &className);

    virtual ~GK11ModelNoGluons();

    virtual GK11ModelNoGluons* clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual std::string toString() const;

protected:

    GK11ModelNoGluons(const GK11ModelNoGluons& other);

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

#endif /* GK11MODELNOGLUONS_H_ */
