#ifndef X_SPACE_GPD_EVOLUTION_MODULE_H
#define X_SPACE_GPD_EVOLUTION_MODULE_H

/**
 * @file xSpaceGPDEvolutionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 11 September 2015
 * @version 1.0
 *
 * @class xSpaceGPDEvolutionModule
 *
 * @brief
 */

#include <string>

#include "../MathIntegratorModule.h"
#include "GPDEvolutionModule.h"

class xSpaceGPDEvolutionModule: public GPDEvolutionModule,
        public MathIntegratorModule {
public:
    xSpaceGPDEvolutionModule(const std::string &className);
    virtual ~xSpaceGPDEvolutionModule();
    virtual xSpaceGPDEvolutionModule* clone() const = 0;

protected:
    /**
     * Copy constructor
     */
    xSpaceGPDEvolutionModule(const xSpaceGPDEvolutionModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual double nonSingletMuFDerivative(const NfInterval &nfInterval);
    virtual double singletMuFDerivative(const NfInterval &nfInterval);
    virtual double gluonMuFDerivative(const NfInterval &nfInterval);

    virtual double kernelNonSinglet(double y) = 0;

    virtual double kernelSingletqq(double y) = 0;
    virtual double kernelSingletqg(double y) = 0;
    virtual double kernekSingletgq(double y) = 0;
    virtual double kernelSingletgg(double y) = 0;

private:
    double integrateNonSinglet(double* variables, double* parameters);
    double integrateSinglet(double *variables, double *parameters);
    double integrateGluon(double *variables, double *parameters);
};

#endif /* X_SPACE_GPD_EVOLUTION_MODULE_H */
