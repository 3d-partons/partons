#ifndef EXAMPLE_EVOL_QCD_MODEL_H
#define EXAMPLE_EVOL_QCD_MODEL_H

/**
 * @file ExampleEvolQCDModel.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 07 September 2015
 * @version 1.0
 *
 * @class ExampleEvolQCDModel
 *
 * @brief
 */

#include <string>

#include "../GPDEvolutionModule.h"

namespace PARTONS {

class ExampleEvolQCDModel: public GPDEvolutionModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    ExampleEvolQCDModel(const std::string &className);
    virtual ~ExampleEvolQCDModel();

    virtual ExampleEvolQCDModel* clone() const;

protected:
    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    ExampleEvolQCDModel(const ExampleEvolQCDModel& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual double integratedNonSingletMuFDerivative(const ActiveFlavorsThresholds &nfInterval);
    virtual double integratedSingletMuFDerivative(const ActiveFlavorsThresholds &nfInterval);
    virtual double integratedGluonMuFDerivative(const ActiveFlavorsThresholds &nfInterval);

private:
};

} /* namespace PARTONS */

#endif /* EXAMPLE_EVOL_QCD_MODEL_H */
