#include "DVCSConstantCFFModel.h"

#include <map>
#include <utility>

#include "../../../beans/gpd/GPDType.h"
#include "../../../BaseObjectRegistry.h"
#include "../../../ModuleObjectFactory.h"
#include "../../../utils/GenericType.h"
#include "../../../utils/ParameterList.h"
#include "../../../utils/stringUtils/Formatter.h"
#include "../../../utils/stringUtils/StringUtils.h"
#include "../../alphaS/RunningAlphaStrong.h"
#include "../../evolution/gpd/ExampleEvolQCDModel.h"

// Initialise [class]::classId with a unique name.
const unsigned int DVCSConstantCFFModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSConstantCFFModel("DVCSConstantCFFModel"));

const std::string DVCSConstantCFFModel::CFF_VALUES = "cff_values";

DVCSConstantCFFModel::DVCSConstantCFFModel(const std::string &className) :
        DVCSConvolCoeffFunctionModule(className) {
    m_CFF.assign(GPDType::END, 0.);

    for (int i = static_cast<int>(GPDType::H);
            i < static_cast<int>(GPDType::END); i++) {
        m_listOfCFFComputeFunctionAvailable.insert(
                std::make_pair(static_cast<GPDType::Type>(i),
                        &DVCSConvolCoeffFunctionModule::computeCFF));
    }
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::H,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::E,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::Ht,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::Et,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::HTrans,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::ETrans,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::HtTrans,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::EtTrans,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::H3p,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::E3p,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::Ht3p,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::Et3p,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::H3m,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::E3m,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::Ht3m,
//                    &DVCSConstantCFFModel::computeCFF));
//    m_listOfCFFComputeFunctionAvailable.insert(
//            std::make_pair(GPDType::Et3m,
//                    &DVCSConstantCFFModel::computeCFF));
}

DVCSConstantCFFModel::DVCSConstantCFFModel(const DVCSConstantCFFModel &other) :
        DVCSConvolCoeffFunctionModule(other) {
    m_CFF = other.m_CFF;
}

DVCSConstantCFFModel* DVCSConstantCFFModel::clone() const {
    return new DVCSConstantCFFModel(*this);
}

//TODO comment gérer le cycle de vie des modules membres
DVCSConstantCFFModel::~DVCSConstantCFFModel() {
}

//TODO comment gérer la dépendance aux autres modules quand on en a pas besoin ? (voir erreur isWellConfigured)
void DVCSConstantCFFModel::init() {

    m_pRunningAlphaStrongModule =
            ModuleObjectFactory::newRunningAlphaStrongModule(
                    RunningAlphaStrong::classId);

    m_pNfConvolCoeffFunction = ModuleObjectFactory::newActiveFlavorsModule(
            ExampleEvolQCDModel::classId);
}

void DVCSConstantCFFModel::initModule() {
    // init parent module before
    DVCSConvolCoeffFunctionModule::initModule();
}

void DVCSConstantCFFModel::isModuleWellConfigured() {
    // check parent module before
    DVCSConvolCoeffFunctionModule::isModuleWellConfigured();
}

std::complex<double> DVCSConstantCFFModel::computeCFF() {
    return m_CFF[m_currentGPDComputeType];
}

void DVCSConstantCFFModel::configure(ParameterList parameters) {
    DVCSConvolCoeffFunctionModule::configure(parameters);

    if (parameters.isAvailable(DVCSConstantCFFModel::CFF_VALUES)) {

        std::string temp_str = parameters.getLastAvailable().toString();
        if (!temp_str.empty()) {
            std::vector<std::string> CFFValues = StringUtils::split(
                    parameters.getLastAvailable().toString(), '|');

            for (int i = 0;
                    i < CFFValues.size()
                            and i
                                    < 2
                                            * (static_cast<int>(GPDType::END)
                                                    - static_cast<int>(GPDType::H));
                    i = i + 2) {
                int j = static_cast<int>(GPDType::H) + i / 2;
                m_CFF[j] = std::complex<double>(
                        StringUtils::fromStringToDouble(CFFValues[i]),
                        StringUtils::fromStringToDouble(CFFValues[i + 1]));

                info(__func__,
                        Formatter() << DVCSConstantCFFModel::CFF_VALUES
                                << " of type "
                                << GPDType(static_cast<GPDType::Type>(j)).toString()
                                << " configured with value = "
                                << m_CFF[j].real() << " + i*"
                                << m_CFF[j].imag());
            }
        }
    }
}
