#ifndef DVCS_CFF_NN_H
#define DVCS_CFF_NN_H

#include <ElementaryUtils/parameters/Parameters.h>
#include <NumA/neural_network/neural_network/NeuralNetwork.h>
#include <stddef.h>
#include <complex>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/gpd/GPDType.h"
#include "DVCSConvolCoeffFunctionModule.h"

namespace PARTONS {

/**
 * @class DVCSCFFNN
 *
 * @brief DVCS CFFs based on neural network analysis.
 *
 * This module returns CFFs as estimated from this fit to world data: Eur.Phys.J. C79 (2019) no.7, 614
 *
 * Mean values and uncertainties should be estimated from a set of values returned by 101 replicas. To run the evaluation for
 * a given replica one should use either DVCSCFFNN::loadParameters() function, or DVCSCFFNN::configure() with
 * DVCSCFFNN::PARAMETER_NAME_REPLICA parameter (to be used also in xml).
 *
 * Mean values and uncertainties can be evaluated from a set of values using DVCSCFFNN::getMeanAndUncertainty().
 * Please note the procedure to remove the outliers.
 */
class DVCSCFFNN: public DVCSConvolCoeffFunctionModule {

public:

    static const std::string PARAMETER_NAME_REPLICA; ///< Name of parameter to set replica index via xml file.

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    DVCSCFFNN(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSCFFNN();

    virtual DVCSCFFNN* clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void resolveObjectDependencies();

    virtual void prepareSubModules(
            const std::map<std::string, PARTONS::BaseObjectData>& subModulesData);

    virtual std::complex<double> computeCFF();

    /**
     * Load parameters for a given replica index.
     */
    void loadParameters(size_t replica, bool printInfo = true);

    /**
     * Get neural networks.
     */
    const std::map<PARTONS::GPDType::Type,
            std::pair<NumA::NeuralNetwork*, NumA::NeuralNetwork*> >& getNeuralNetworks() const;

    /**
     * Evaluate mean and uncertainty for a given vector of numbers. The procedure include removing of outliers.
     */
    void getMeanAndUncertainty(const std::vector<double>& v, double& mean,
            double& unc) const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSCFFNN(const DVCSCFFNN &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    /**
     * Build all neural networks.
     */
    void buildNeuralNetworks();

    /**
     * Build single neural network for a given GPD type and CFF RE or Im.
     */
    NumA::NeuralNetwork* buildAndConfigureSingleNeuralNetwork(
            PARTONS::GPDType::Type gpdType, bool isReal);

    /**
     * Evaluate mean from a given vector.
     */
    double getMean(const std::vector<double>& v) const;

    /**
     * Evaluate sigma from a given vector.
     */
    double getSigma(const std::vector<double>& v) const;

    /**
     * Remove outliers from a given vector using 3sigma rule.
     */
    size_t removeOutliers(std::vector<double>& v) const;

    std::map<PARTONS::GPDType::Type,
            std::pair<NumA::NeuralNetwork*, NumA::NeuralNetwork*> > m_neuralNetworks; ///< Neural networks

    std::pair<double, double> m_rangeLog10Xi; ///< Normalization range: log10xi
    std::pair<double, double> m_rangeT; ///< Normalization range: t
    std::pair<double, double> m_rangeLog10Q2; ///< Normalization range: log10Q2
    std::map<PARTONS::GPDType::Type, std::pair<double, double> > m_rangeXiReCFF; ///< Normalization range: xi*ReCFF
    std::map<PARTONS::GPDType::Type, std::pair<double, double> > m_rangeXiImCFF; ///< Normalization range: xi*ImCFF

    size_t m_replica; ///< Replica index.
};

}

#endif /* DVCS_CFF_NN_H */
