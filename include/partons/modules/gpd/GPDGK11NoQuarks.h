#ifndef GPDGK11NOQUARKS_H
#define GPDGK11NOQUARKS_H

/**
 * @file GPDGK11NoQuarks.h
 * @author Jakub
 * @date 13 February 2015
 * @version 1.0
 *
 * @class GK11NoQuarksModel
 *
 * @brief
 */

#include <string>

#include "GPDGK11.h"

class GPDGK11NoQuarks: public GPDGK11 {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    GPDGK11NoQuarks(const std::string &className);

    virtual ~GPDGK11NoQuarks();

    virtual GPDGK11NoQuarks* clone() const;

protected:
    GPDGK11NoQuarks(const GPDGK11NoQuarks& other);

//    virtual void isModuleWellConfigured();
//    virtual void initModule();

    //GPDResultData compute(GPDComputeType gpdComputeType);

    virtual PartonDistribution computeH(); ///< Compute GPD H at considered kinematics
    virtual PartonDistribution computeE(); ///< Compute GPD E at considered kinematics
    virtual PartonDistribution computeHt(); ///< Compute GPD Ht at considered kinematics
    virtual PartonDistribution computeEt(); ///< Compute GPD Et at considered kinematics

//    double computeSinglet(const GPDQuarkFlavorData &quark_u,
//            const GPDQuarkFlavorData &quark_d,
//            const GPDQuarkFlavorData &quark_s);
};

#endif /* GPDGK11NOQUARKS_H */
