#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConstantCFFModel.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <map>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"

//#include "../../../../../include/partons/modules/active_flavors/NfFunctionExample.h"
//#include "../../../../../include/partons/modules/alphaS/RunningAlphaStrong.h"
//#include "../../../../../include/partons/ModuleObjectFactory.h"
//#include "../../../../../include/partons/Partons.h"

// Initialise [class]::classId with a unique name.
const unsigned int DVCSConstantCFFModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSConstantCFFModel("DVCSConstantCFFModel"));

const std::string DVCSConstantCFFModel::CFF_VALUES = "cff_values";

DVCSConstantCFFModel::DVCSConstantCFFModel(const std::string &className) :
        DVCSConvolCoeffFunctionModule(className) {
    setIsGPDModuleDependent(false);

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

void DVCSConstantCFFModel::configure(const ElemUtils::Parameters &parameters) {
    if (parameters.isAvailable(DVCSConstantCFFModel::CFF_VALUES)) {

        std::string temp_str = parameters.getLastAvailable().toString();
        if (!temp_str.empty()) {
            std::vector<std::string> CFFValues = ElemUtils::StringUtils::split(
                    parameters.getLastAvailable().toString(), '|');

            for (int i = 0;
                    i < CFFValues.size()
                            and i
                                    < 2
                                            * (static_cast<int>(GPDType::END)
                                                    - static_cast<int>(GPDType::H));
                    i = i + 2) {
                int j = static_cast<int>(GPDType::H) + i / 2;
                m_CFF[j] =
                        std::complex<double>(
                                ElemUtils::GenericType(CFFValues[i]).toDouble(),
                                i + 1 < CFFValues.size() ?
                                        ElemUtils::GenericType(CFFValues[i + 1]).toDouble() :
                                        0.);

                info(__func__,
                        ElemUtils::Formatter()
                                << DVCSConstantCFFModel::CFF_VALUES
                                << " of type "
                                << GPDType(static_cast<GPDType::Type>(j)).toString()
                                << " configured with value = "
                                << m_CFF[j].real() << " + i*"
                                << m_CFF[j].imag());
            }
        }
    }

    DVCSConvolCoeffFunctionModule::configure(parameters);
}
