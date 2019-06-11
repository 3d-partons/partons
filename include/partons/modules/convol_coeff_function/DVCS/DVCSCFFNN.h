#ifndef DVCS_CFF_NN_H
#define DVCS_CFF_NN_H

#include <ElementaryUtils/parameters/Parameters.h>
#include <NumA/neural_network/neural_network/NeuralNetwork.h>
#include <stddef.h>
#include <complex>
#include <map>
#include <string>
#include <utility>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/gpd/GPDType.h"
#include "DVCSConvolCoeffFunctionModule.h"

namespace PARTONS {

class DVCSCFFNN: public DVCSConvolCoeffFunctionModule {

public:

    static const std::string PARAMETER_NAME_REPLICA;

    static const unsigned int classId;

    DVCSCFFNN(const std::string &className);
    virtual DVCSCFFNN* clone() const;
    virtual ~DVCSCFFNN();

    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void resolveObjectDependencies();

    virtual void prepareSubModules(
            const std::map<std::string, PARTONS::BaseObjectData>& subModulesData);

    virtual std::complex<double> computeCFF();

    void loadParameters(size_t replica);

    const std::map<PARTONS::GPDType::Type,
            std::pair<NumA::NeuralNetwork*, NumA::NeuralNetwork*> >& getNeuralNetworks() const;

protected:

    DVCSCFFNN(const DVCSCFFNN &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    void buildNeuralNetworks();
    NumA::NeuralNetwork* buildAndConfigureSingleNeuralNetwork(
            PARTONS::GPDType::Type gpdType, bool isReal);

    std::map<PARTONS::GPDType::Type,
            std::pair<NumA::NeuralNetwork*, NumA::NeuralNetwork*> > m_neuralNetworks;

    std::pair<double, double> m_rangeLog10Xi;
    std::pair<double, double> m_rangeT;
    std::pair<double, double> m_rangeQ2;
    std::map<PARTONS::GPDType::Type, std::pair<double, double> > m_rangeXiReCFF;
    std::map<PARTONS::GPDType::Type, std::pair<double, double> > m_rangeXiImCFF;

    size_t m_replica;
};

}

#endif /* DVCS_CFF_NN_H */
