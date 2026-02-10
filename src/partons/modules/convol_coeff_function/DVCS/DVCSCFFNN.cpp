#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSCFFNN.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/neural_network/beans/ActivationFunctionType.h>
#include <NumA/neural_network/beans/CombinationFunctionType.h>
#include <NumA/neural_network/beans/Data.h>
#include <NumA/neural_network/beans/NeuralNetworkCellType.h>
#include <NumA/neural_network/beans/ScalingFunctionType.h>
#include <NumA/neural_network/neural_network_cell/Perceptron.h>
#include <NumA/neural_network/neural_network_cell/ScalingCell.h>
#include <NumA/neural_network/neural_network_layer/NeuralNetworkLayer.h>
#include <NumA/neural_network/neural_network_neuron/NeuralNetworkNeuron.h>
#include <cmath>
#include <iterator>

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/modules/convol_coeff_function/ConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSCFFNNReplicas.h"

namespace PARTONS {

const std::string DVCSCFFNN::PARAMETER_NAME_REPLICA = "replica";

const unsigned int DVCSCFFNN::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFNN("DVCSCFFNN"));

DVCSCFFNN::DVCSCFFNN(const std::string &className) :
        DVCSConvolCoeffFunctionModule(className) {

    //mark as GPDModule independent
    setIsGPDModuleDependent(false);

    //availible CFFs
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &DVCSConvolCoeffFunctionModule::computeCFF));

    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::E,
                    &DVCSConvolCoeffFunctionModule::computeCFF));

    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Ht,
                    &DVCSConvolCoeffFunctionModule::computeCFF));

    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Et,
                    &DVCSConvolCoeffFunctionModule::computeCFF));

    NumA::NeuralNetwork* nullNN = 0;

    m_neuralNetworks.insert(
            std::make_pair(GPDType::H, std::make_pair(nullNN, nullNN)));
    m_neuralNetworks.insert(
            std::make_pair(GPDType::E, std::make_pair(nullNN, nullNN)));
    m_neuralNetworks.insert(
            std::make_pair(GPDType::Ht, std::make_pair(nullNN, nullNN)));
    m_neuralNetworks.insert(
            std::make_pair(GPDType::Et, std::make_pair(nullNN, nullNN)));

    m_rangeLog10Xi = std::make_pair(-6., 1.);
    m_rangeT = std::make_pair(-1., 0.5);
    m_rangeLog10Q2 = std::make_pair(-1., 2.);

    m_rangeXiReCFF.insert(std::make_pair(GPDType::H, std::make_pair(-1., 1.5)));
    m_rangeXiReCFF.insert(std::make_pair(GPDType::E, std::make_pair(-1.5, 1.)));
    m_rangeXiReCFF.insert(
            std::make_pair(GPDType::Ht, std::make_pair(-0.6, 0.9)));
    m_rangeXiReCFF.insert(
            std::make_pair(GPDType::Et, std::make_pair(-120., 180.)));
    m_rangeXiImCFF.insert(std::make_pair(GPDType::H, std::make_pair(-4., 6.)));
    m_rangeXiImCFF.insert(std::make_pair(GPDType::E, std::make_pair(-6., 4.)));
    m_rangeXiImCFF.insert(
            std::make_pair(GPDType::Ht, std::make_pair(-1., 1.5)));
    m_rangeXiImCFF.insert(
            std::make_pair(GPDType::Et, std::make_pair(-8., 12.)));

    m_replica = 0;

    buildNeuralNetworks();
    loadParameters(m_replica, false);
}

DVCSCFFNN::DVCSCFFNN(const DVCSCFFNN &other) :
        DVCSConvolCoeffFunctionModule(other) {

    NumA::NeuralNetwork* nullNN = 0;

    std::map<GPDType::Type,
            std::pair<NumA::NeuralNetwork*, NumA::NeuralNetwork*> >::const_iterator it;

    for (it = other.m_neuralNetworks.begin();
            it != other.m_neuralNetworks.end(); it++) {
        if (it->second.first && it->second.second) {
            m_neuralNetworks.insert(
                    std::make_pair(it->first,
                            std::make_pair((it->second.first)->clone(),
                                    (it->second.second)->clone())));
        } else {
            m_neuralNetworks.insert(
                    std::make_pair(it->first, std::make_pair(nullNN, nullNN)));
        }
    }

    m_rangeLog10Xi = other.m_rangeLog10Xi;
    m_rangeT = other.m_rangeT;
    m_rangeLog10Q2 = other.m_rangeLog10Q2;
    m_rangeXiReCFF = other.m_rangeXiReCFF;
    m_rangeXiImCFF = other.m_rangeXiImCFF;

    m_replica = other.m_replica;
}

DVCSCFFNN* DVCSCFFNN::clone() const {
    return new DVCSCFFNN(*this);
}

DVCSCFFNN::~DVCSCFFNN() {

    std::map<GPDType::Type,
            std::pair<NumA::NeuralNetwork*, NumA::NeuralNetwork*> >::iterator it;

    for (it = m_neuralNetworks.begin(); it != m_neuralNetworks.end(); it++) {

        if (it->second.first) {
            delete it->second.first;
            it->second.first = 0;
        }

        if (it->second.second) {
            delete it->second.second;
            it->second.second = 0;
        }
    }
}

void DVCSCFFNN::configure(const ElemUtils::Parameters &parameters) {

    DVCSConvolCoeffFunctionModule::configure(parameters);

    if (parameters.isAvailable(PARAMETER_NAME_REPLICA)) {
        loadParameters(parameters.getLastAvailable().toUInt());
    }
}

void DVCSCFFNN::resolveObjectDependencies() {
    DVCSConvolCoeffFunctionModule::resolveObjectDependencies();
}

void DVCSCFFNN::buildNeuralNetworks() {

    //iterator
    std::map<GPDType::Type,
            std::pair<NumA::NeuralNetwork*, NumA::NeuralNetwork*> >::iterator it;

    //delete if exist and create
    for (it = m_neuralNetworks.begin(); it != m_neuralNetworks.end(); it++) {

        //real part
        if (it->second.first != 0) {
            delete it->second.first;
            it->second.first = 0;
        }

        it->second.first = buildAndConfigureSingleNeuralNetwork(it->first, 1);

        //real part
        if (it->second.second != 0) {
            delete it->second.second;
            it->second.second = 0;
        }

        it->second.second = buildAndConfigureSingleNeuralNetwork(it->first, 0);
    }
}

NumA::NeuralNetwork* DVCSCFFNN::buildAndConfigureSingleNeuralNetwork(
        GPDType::Type gpdType, bool isReal) {

    //pointer to NN
    NumA::NeuralNetwork* pNN;

    //definition of neural nets
    std::vector<unsigned int> perceptronLayers;
    perceptronLayers.push_back(6);

    //allocate
    pNN = new NumA::NeuralNetwork(3, 1, perceptronLayers,
            NumA::ActivationFunctionType::Hyperbolic,
            NumA::CombinationFunctionType::ScalarProduct,
            NumA::ActivationFunctionType::Linear,
            NumA::CombinationFunctionType::ScalarProduct,
            NumA::ScalingFunctionType::MinMax);

    //scaling
    NumA::NeuralNetworkLayer* pNNLayer;
    NumA::ScalingCell* pNNCell;

    //input
    pNNLayer = pNN->getNeuralNetworkLayers().at(1);

    //xi
    if (pNNLayer->getCells().at(0)->getType()
            != NumA::NeuralNetworkCellType::ScalingCell) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "No scaling cell type");
    }
    pNNCell = static_cast<NumA::ScalingCell*>(pNNLayer->getCells().at(0));
    pNNCell->setScalingParameters(m_rangeLog10Xi);

    //t
    if (pNNLayer->getCells().at(1)->getType()
            != NumA::NeuralNetworkCellType::ScalingCell) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "No scaling cell type");
    }
    pNNCell = static_cast<NumA::ScalingCell*>(pNNLayer->getCells().at(1));
    pNNCell->setScalingParameters(m_rangeT);

    //Q2
    if (pNNLayer->getCells().at(2)->getType()
            != NumA::NeuralNetworkCellType::ScalingCell) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "No scaling cell type");
    }
    pNNCell = static_cast<NumA::ScalingCell*>(pNNLayer->getCells().at(2));
    pNNCell->setScalingParameters(m_rangeLog10Q2);

    //output
    pNNLayer = pNN->getNeuralNetworkLayers().at(
            pNN->getNeuralNetworkLayers().size() - 2);

    //Re or Im
    if (pNNLayer->getCells().at(0)->getType()
            != NumA::NeuralNetworkCellType::ScalingCell) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "No scaling cell type");
    }
    pNNCell = static_cast<NumA::ScalingCell*>(pNNLayer->getCells().at(0));

    if (isReal) {
        pNNCell->setScalingParameters(m_rangeXiReCFF.find(gpdType)->second);
    } else {
        pNNCell->setScalingParameters(m_rangeXiImCFF.find(gpdType)->second);
    }

    //return
    return pNN;
}

void DVCSCFFNN::loadParameters(size_t replica, bool printInfo) {

    if (replica >= c_nDVCSCFFNNReplicas) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Illegal replica " << replica);
    }

    std::vector<double> params(c_nDVCSCFFNNParameters);

    for (size_t i = 0; i < c_nDVCSCFFNNParameters; i++) {
        params[i] = c_DVCSCFFNNReplicas[replica][i];
    }

    setParameters(params);

    if (printInfo) {
        info(__func__,
                ElemUtils::Formatter() << "Parameters set for replica "
                        << replica);
    }
}

void DVCSCFFNN::setParameters(const std::vector<double>& params) {

    std::vector<NumA::NeuralNetworkCell*> cells;
    std::vector<NumA::NeuralNetworkNeuron*> neurons;

    std::vector<NumA::NeuralNetworkCell*>::const_iterator cell;
    std::vector<NumA::NeuralNetworkNeuron*>::const_iterator neuron;
    std::map<GPDType::Type,
            std::pair<NumA::NeuralNetwork*, NumA::NeuralNetwork*> >::iterator it;

    for (it = m_neuralNetworks.begin(); it != m_neuralNetworks.end(); it++) {

        //real and imaginary parts
        for (size_t i = 0; i < 2; i++) {

            //get pointer to nn
            NumA::NeuralNetwork* nn =
                    (i == 0) ? ((it->second).first) : ((it->second).second);

            //get perceptrons
            for (cell = (nn->getNeuralNetworkCells()).begin();
                    cell != (nn->getNeuralNetworkCells()).end(); cell++) {
                if ((*cell)->getType()
                        == NumA::NeuralNetworkCellType::Perceptron) {
                    cells.push_back(*cell);
                }
            }

            //get neurons
            for (neuron = (nn->getNeuralNetworkNeurons()).begin();
                    neuron != (nn->getNeuralNetworkNeurons()).end(); neuron++) {
                if ((*neuron)->getNeuralNetworkCellOut()->getType()
                        == NumA::NeuralNetworkCellType::Perceptron) {
                    neurons.push_back(*neuron);
                }
            }

        }
    }

    size_t parameter = 0;

    for (cell = cells.begin(); cell != cells.end(); cell++) {

        (static_cast<NumA::Perceptron*>(*cell))->setBias(
                params.at(parameter));

        parameter++;

        if (parameter > params.size()) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Illegal parameter index "
                            << parameter);
        }
    }

    for (neuron = neurons.begin(); neuron != neurons.end(); neuron++) {

        (*neuron)->setWeight(params.at(parameter));

        parameter++;

        if (parameter > params.size()) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Illegal parameter index "
                            << parameter);
        }
    }
}

void DVCSCFFNN::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    DVCSConvolCoeffFunctionModule::prepareSubModules(subModulesData);
}

void DVCSCFFNN::initModule() {
    DVCSConvolCoeffFunctionModule::initModule();
}

void DVCSCFFNN::isModuleWellConfigured() {
    DVCSConvolCoeffFunctionModule::isModuleWellConfigured();
}

std::complex<double> DVCSCFFNN::computeCFF() {

    std::map<GPDType::Type,
            std::pair<NumA::NeuralNetwork*, NumA::NeuralNetwork*> >::iterator it =
            m_neuralNetworks.find(m_currentGPDComputeType);

    if (it == m_neuralNetworks.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Calculation for GPD type "
                        << GPDType(m_currentGPDComputeType).toString()
                        << " unsupported");
    }

    if ((!it->second.first) || (!it->second.second)) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Null pointer to NN representing CFF for GPD "
                        << GPDType(m_currentGPDComputeType).toString());
    }

    NumA::Data dataInput(3);

    std::vector<double> dataSingle;
    dataSingle.push_back(log10(m_xi));
    dataSingle.push_back(m_t);
    dataSingle.push_back(log10(m_Q2));
    dataInput.addData(dataSingle);

    NumA::Data dataOutputRe = (it->second.first)->evaluate(dataInput);
    NumA::Data dataOutputIm = (it->second.second)->evaluate(dataInput);

    double r_part = ((dataOutputRe.getData().find(0))->second).at(0) / m_xi;
    double i_part = ((dataOutputIm.getData().find(0))->second).at(0) / m_xi;

    return std::complex<double>(r_part, i_part);
}

const std::map<GPDType::Type,
        std::pair<NumA::NeuralNetwork*, NumA::NeuralNetwork*> >& DVCSCFFNN::getNeuralNetworks() const {
    return m_neuralNetworks;
}

double DVCSCFFNN::getMean(const std::vector<double>& v) const {

    if (v.size() == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "vector size is 0");
    }

    double mean = 0.;

    for (int i = 0; i < v.size(); i++) {
        mean += v.at(i);
    }

    return mean / v.size();
}

double DVCSCFFNN::getSigma(const std::vector<double>& v) const {

    if (v.size() == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "vector size is 0");
    }

    double mean = getMean(v);

    double sigma = 0.;

    for (int i = 0; i < v.size(); i++) {
        sigma += pow(mean - v.at(i), 2);
    }

    return sqrt(sigma / double(v.size()));
}

size_t DVCSCFFNN::removeOutliers(std::vector<double>& v) const {

    if (v.size() == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "vector size is 0");
    }

    double meanData = getMean(v);
    double sigmaData = getSigma(v);

    if (sigmaData == 0.) {

        warn(__func__, "sigma size is 0");
        return 0;
    }

    std::vector<double> result;
    std::vector<double>::iterator it;
    size_t nRemoved = 0;

    for (it = v.begin(); it != v.end(); it++) {

        if (fabs((*it) - meanData) / sigmaData > 3.) {
            nRemoved++;
        } else {
            result.push_back(*it);
        }
    }

    v = result;

    if (nRemoved != 0)
        nRemoved += removeOutliers(v);

    return nRemoved;
}

void DVCSCFFNN::getMeanAndUncertainty(const std::vector<double>& v,
        double& mean, double& unc) const {

    std::vector<double> vOutlierFree = v;
    removeOutliers(vOutlierFree);

    mean = getMean(vOutlierFree);
    unc = getSigma(vOutlierFree);
}

}
