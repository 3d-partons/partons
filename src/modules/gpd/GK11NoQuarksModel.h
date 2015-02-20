/*
 * GK11NoQuarksModel.h
 *
 *  Created on: Feb 13, 2015
 *      Author: kuba
 */

#ifndef GK11NOQUARKSMODEL_H_
#define GK11NOQUARKSMODEL_H_

#include "GK11Model.h"

class GK11NoQuarksModel: public GK11Model {
public:

    static const std::string moduleID; ///< Unique ID to automatically register the module in the factory.

	GK11NoQuarksModel(const std::string &className);

	virtual ~GK11NoQuarksModel();

	virtual GK11NoQuarksModel* clone() const;

protected:
	GK11NoQuarksModel(const GK11NoQuarksModel& other);

//    virtual void isModuleWellConfigured();
//    virtual void initModule();

    //GPDResultData compute(GPDComputeType gpdComputeType);

    virtual GPDResultData computeH(); ///< Compute GPD H at considered kinematics
    virtual GPDResultData computeE(); ///< Compute GPD E at considered kinematics
    virtual GPDResultData computeHt(); ///< Compute GPD Ht at considered kinematics
    virtual GPDResultData computeEt(); ///< Compute GPD Et at considered kinematics

//    double computeSinglet(const GPDQuarkFlavorData &quark_u,
//            const GPDQuarkFlavorData &quark_d,
//            const GPDQuarkFlavorData &quark_s);
};

#endif /* GK11NOQUARKSMODEL_H_ */
