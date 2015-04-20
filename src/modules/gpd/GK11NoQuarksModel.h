#ifndef GK11_NO_QUARKS_MODEL_H
#define GK11_NO_QUARKS_MODEL_H

/**
 * @file GK11NoQuarksModel.h
 * @author Jakub
 * @date 13 February 2015
 * @version 1.0
 *
 * @class GK11NoQuarksModel
 *
 * @brief
 */

#include <string>

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

#endif /* GK11_NO_QUARKS_MODEL_H */
