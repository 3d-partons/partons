/*
 * HadronStructureUtils.cpp
 *
 *  Created on: 30 juin 2015
 *      Author: debian
 */

#include "HadronStructureUtils.h"

#include "GPDResultReport.h"
#include "PartonDistributionReport.h"

GPDResultReport HadronStructureUtils::compareGPDResults(
        GPDResult* p_gpdResultLeft, GPDResult* p_gpdResultRight,
        double absoluteTolerance, double relativeTolerance) {
//    HadronStructureUtils::comparePartonDistributions();
    return GPDResultReport();
}

PartonDistributionReport HadronStructureUtils::comparePartonDistributions(
        PartonDistribution* p_partonDistributionLeft,
        PartonDistribution* p_partonDistributionRight, double absoluteTolerance,
        double relativeTolerance) {
    return PartonDistributionReport();
}
