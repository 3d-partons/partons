/*
 * KM10BorderFunctionModel.h
 *
 *  Created on: Oct 17, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#ifndef KM10BORDERFUNCTIONMODEL_H_
#define KM10BORDERFUNCTIONMODEL_H_

#include <string>

#include "../GPDBorderFunctionModule.h"

/**
 * GPD border function model used in Kumericki-Mueller fit
 * see Nucl. Phys. B841, 1 (2010)
 */
class KM10BorderFunctionModel: public GPDBorderFunctionModule {

public:

    static const unsigned int classId; ///< ID assigned by BaseObjectRegistry

    /** Constructor
     @param className Name of this class
     */
    KM10BorderFunctionModel(const std::string &className);

    /**
     * Destructor
     */
    virtual ~KM10BorderFunctionModel();

    virtual KM10BorderFunctionModel* clone() const;
    virtual void resolveObjectDependencies();
    void virtual configure(const ElemUtils::Parameters &parameters);
    virtual std::string toString();

    virtual PartonDistribution computeH();
};

#endif /* KM10BORDERFUNCTIONMODEL_H_ */
