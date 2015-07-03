/*
 * ModuleObjectFactory.h
 *
 *  Created on: Jul 2, 2015
 *      Author: Bryan BERTHOU
 */

#ifndef MODULEOBJECTFACTORY_H_
#define MODULEOBJECTFACTORY_H_

#include <string>

class BaseObjectFactory;
class CoefficientFunctionModule;
class DVCSModule;
class EvolQCDModule;
class GPDModule;
class MathIntegratorModule;
class ObservableModule;
class RunningAlphaStrongModule;

/*
 *
 */
class ModuleObjectFactory {
public:

    static GPDModule* newGPDModule(unsigned int classId);
    static GPDModule* newGPDModule(const std::string &className);

    static CoefficientFunctionModule* newCoefficientFunctionModule(
            unsigned int classId);
    static CoefficientFunctionModule* newCoefficientFunctionModule(
            const std::string &className);

    static EvolQCDModule* newEvolQCDModule(unsigned int classId);
    static EvolQCDModule* newEvolQCDModule(const std::string & className);

    static DVCSModule* newDVCSModule(unsigned int classId);
    static DVCSModule* newDVCSModule(const std::string & className);

    static MathIntegratorModule* newMathIntegratorModule(unsigned int classId);
    static MathIntegratorModule* newMathIntegratorModule(
            const std::string & className);

    static RunningAlphaStrongModule* newRunningAlphaStrongModule(
            unsigned int classId);
    static RunningAlphaStrongModule* newRunningAlphaStrongModule(
            const std::string & className);

    static ObservableModule* newObservableModule(unsigned int classId);
    static ObservableModule* newObservableModule(const std::string &className);

private:

    static BaseObjectFactory* m_pBaseObjectFactory;
};

#endif /* MODULEOBJECTFACTORY_H_ */
