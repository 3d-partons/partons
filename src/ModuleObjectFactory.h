#ifndef MODULE_OBJECT_FACTORY_H
#define MODULE_OBJECT_FACTORY_H

/**
 * @file ModuleObjectFactory.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 02 July 2015
 * @version 1.0
 *
 * @class ModuleObjectFactory
 *
 * @brief
 */

#include <string>

class DoubleDistributionModule;
class DVCSConvolCoeffFunctionModule;
class DVCSModule;
class EvolQCDModule;
class GPDModule;
class ObservableModule;
class RunningAlphaStrongModule;

class ModuleObjectFactory {
public:

    static DoubleDistributionModule* newDoubleDistributionModule(
            unsigned int classId);
    static DoubleDistributionModule* newDoubleDistributionModule(
            const std::string & className);

    static EvolQCDModule* newEvolQCDModule(unsigned int classId);
    static EvolQCDModule* newEvolQCDModule(const std::string & className);

    static GPDModule* newGPDModule(unsigned int classId);
    static GPDModule* newGPDModule(const std::string &className);

    static DVCSConvolCoeffFunctionModule* newDVCSConvolCoeffFunctionModule(
            unsigned int classId);
    static DVCSConvolCoeffFunctionModule* newDVCSConvolCoeffFunctionModule(
            const std::string &className);

    static DVCSModule* newDVCSModule(unsigned int classId);
    static DVCSModule* newDVCSModule(const std::string & className);

    static RunningAlphaStrongModule* newRunningAlphaStrongModule(
            unsigned int classId);
    static RunningAlphaStrongModule* newRunningAlphaStrongModule(
            const std::string & className);

    static ObservableModule* newObservableModule(unsigned int classId);
    static ObservableModule* newObservableModule(const std::string &className);
};

#endif /* MODULE_OBJECT_FACTORY_H */
