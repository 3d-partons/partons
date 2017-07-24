#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSCFFConstant.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/Parameter.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <utility>
#include <vector>

#include "../../../../../include/partons/BaseObjectRegistry.h"

namespace PARTONS {


const unsigned int DVCSCFFConstant::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFConstant("DVCSCFFConstant"));

// set key names
const std::string DVCSCFFConstant::PARAMETER_NAME_CFF_VALUES = "cff_values";

const std::string DVCSCFFConstant::PARAMETER_NAME_CFF_H_Re = "cff_value_H_Re";
const std::string DVCSCFFConstant::PARAMETER_NAME_CFF_H_Im = "cff_value_H_Im";
const std::string DVCSCFFConstant::PARAMETER_NAME_CFF_E_Re = "cff_value_E_Re";
const std::string DVCSCFFConstant::PARAMETER_NAME_CFF_E_Im = "cff_value_E_Im";
const std::string DVCSCFFConstant::PARAMETER_NAME_CFF_Ht_Re = "cff_value_Ht_Re";
const std::string DVCSCFFConstant::PARAMETER_NAME_CFF_Ht_Im = "cff_value_Ht_Im";
const std::string DVCSCFFConstant::PARAMETER_NAME_CFF_Et_Re = "cff_value_Et_Re";
const std::string DVCSCFFConstant::PARAMETER_NAME_CFF_Et_Im = "cff_value_Et_Im";

DVCSCFFConstant::DVCSCFFConstant(const std::string &className) :
        DVCSConvolCoeffFunctionModule(className) {
    setIsGPDModuleDependent(false);

    //initialize
    m_CFFs.clear();

    //insert and assign
    for (int i = static_cast<int>(GPDType::H);
            i < static_cast<int>(GPDType::END); i++) {

        //insert to map
        m_CFFs.insert(
                std::make_pair(static_cast<GPDType::Type>(i),
                        std::complex<double>(0., 0.)));

        //assign function
        m_listOfCFFComputeFunctionAvailable.insert(
                std::make_pair(static_cast<GPDType::Type>(i),
                        &DVCSConvolCoeffFunctionModule::computeCFF));
    }
}

DVCSCFFConstant::DVCSCFFConstant(const DVCSCFFConstant &other) :
        DVCSConvolCoeffFunctionModule(other) {
    m_CFFs = other.m_CFFs;
}

DVCSCFFConstant* DVCSCFFConstant::clone() const {
    return new DVCSCFFConstant(*this);
}

//TODO comment gérer le cycle de vie des modules membres
DVCSCFFConstant::~DVCSCFFConstant() {
}

void DVCSCFFConstant::initModule() {

    // initialize parent module before
    DVCSConvolCoeffFunctionModule::initModule();
}

void DVCSCFFConstant::isModuleWellConfigured() {

    // check parent module before
    DVCSConvolCoeffFunctionModule::isModuleWellConfigured();
}

std::complex<double> DVCSCFFConstant::computeCFF() {

    std::map<GPDType::Type, std::complex<double> >::iterator it = m_CFFs.find(
            m_currentGPDComputeType);

    if (it == m_CFFs.end()) {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "CFF of type "
                        << GPDType(m_currentGPDComputeType).toString()
                        << " is not supported by this class");

        return std::complex<double>(0., 0.);

    } else {
        return it->second;
    }
}

void DVCSCFFConstant::configure(const ElemUtils::Parameters &parameters) {

    //clear
    for (std::map<GPDType::Type, std::complex<double> >::iterator it =
            m_CFFs.begin(); it != m_CFFs.end(); it++) {
        (it->second) = std::complex<double>(0., 0.);
    }

    //check if set via all
    bool isSetViaAll = false;

    // several CFFs
    if (parameters.isAvailable(DVCSCFFConstant::PARAMETER_NAME_CFF_VALUES)) {

        //is set via all
        isSetViaAll = true;

        // to string
        std::string temp_str = parameters.getLastAvailable().getString();

        // check if not empty
        if (!temp_str.empty()) {

            // split string
            std::vector<std::string> CFFValues = ElemUtils::StringUtils::split(
                    temp_str, '|');

            // iterator
            std::map<GPDType::Type, std::complex<double> >::iterator it =
                    m_CFFs.begin();

            // assign
            for (int i = 0; i < CFFValues.size(); i++) {

                //check if not end
                if (it == m_CFFs.end()) {

                    warn(__func__,
                            ElemUtils::Formatter()
                                    << "Too many elements for key "
                                    << DVCSCFFConstant::PARAMETER_NAME_CFF_VALUES
                                    << ", additional elements will be ignored");
                    break;
                }

                //assign and increment
                if (i % 2 == 0) {

                    (it->second).real(
                            ElemUtils::GenericType(CFFValues.at(i)).toDouble());

                    info(__func__,
                            ElemUtils::Formatter() << "Real part of CFF type "
                                    << GPDType(it->first).toString()
                                    << " configured with value = "
                                    << (it->second).real());
                } else {

                    (it->second).imag(
                            ElemUtils::GenericType(CFFValues.at(i)).toDouble());

                    info(__func__,
                            ElemUtils::Formatter()
                                    << "Imaginary part of CFF type "
                                    << GPDType(it->first).toString()
                                    << " configured with value = "
                                    << (it->second).imag());

                    it++;
                }
            }

        } else {
            warn(__func__,
                    ElemUtils::Formatter() << "String for "
                            << DVCSCFFConstant::PARAMETER_NAME_CFF_VALUES << " is empty");
        }
    }

    // single CFFs
    if (parameters.isAvailable(DVCSCFFConstant::PARAMETER_NAME_CFF_H_Re)) {
        if (isSetViaAll)
            printErrorInConfigure(DVCSCFFConstant::PARAMETER_NAME_CFF_H_Re);
        configureValue(GPDType::H, 1, parameters.getLastAvailable());
    }
    if (parameters.isAvailable(DVCSCFFConstant::PARAMETER_NAME_CFF_H_Im)) {
        if (isSetViaAll)
            printErrorInConfigure(DVCSCFFConstant::PARAMETER_NAME_CFF_H_Im);
        configureValue(GPDType::H, 0, parameters.getLastAvailable());
    }
    if (parameters.isAvailable(DVCSCFFConstant::PARAMETER_NAME_CFF_E_Re)) {
        if (isSetViaAll)
            printErrorInConfigure(DVCSCFFConstant::PARAMETER_NAME_CFF_E_Re);
        configureValue(GPDType::E, 1, parameters.getLastAvailable());
    }
    if (parameters.isAvailable(DVCSCFFConstant::PARAMETER_NAME_CFF_E_Im)) {
        if (isSetViaAll)
            printErrorInConfigure(DVCSCFFConstant::PARAMETER_NAME_CFF_E_Im);
        configureValue(GPDType::E, 0, parameters.getLastAvailable());
    }
    if (parameters.isAvailable(DVCSCFFConstant::PARAMETER_NAME_CFF_Ht_Re)) {
        if (isSetViaAll)
            printErrorInConfigure(DVCSCFFConstant::PARAMETER_NAME_CFF_Ht_Re);
        configureValue(GPDType::Ht, 1, parameters.getLastAvailable());
    }
    if (parameters.isAvailable(DVCSCFFConstant::PARAMETER_NAME_CFF_Ht_Im)) {
        if (isSetViaAll)
            printErrorInConfigure(DVCSCFFConstant::PARAMETER_NAME_CFF_Ht_Im);
        configureValue(GPDType::Ht, 0, parameters.getLastAvailable());
    }
    if (parameters.isAvailable(DVCSCFFConstant::PARAMETER_NAME_CFF_Et_Re)) {
        if (isSetViaAll)
            printErrorInConfigure(DVCSCFFConstant::PARAMETER_NAME_CFF_Et_Re);
        configureValue(GPDType::Et, 1, parameters.getLastAvailable());
    }
    if (parameters.isAvailable(DVCSCFFConstant::PARAMETER_NAME_CFF_Et_Im)) {
        if (isSetViaAll)
            printErrorInConfigure(DVCSCFFConstant::PARAMETER_NAME_CFF_Et_Im);
        configureValue(GPDType::Et, 0, parameters.getLastAvailable());
    }

    //configure mother class
    DVCSConvolCoeffFunctionModule::configure(parameters);
}

void DVCSCFFConstant::printErrorInConfigure(const std::string& key) const {
    throw ElemUtils::CustomException(getClassName(), __func__,
            ElemUtils::Formatter() << "Ambiguous way of setting CFF values via "
                    << DVCSCFFConstant::PARAMETER_NAME_CFF_VALUES << " and " << key
                    << " keys");
}

void DVCSCFFConstant::configureValue(GPDType::Type gpdType,
        bool isRealPart, const ElemUtils::GenericType& value) {

    //iterator
    std::map<GPDType::Type, std::complex<double> >::iterator it = m_CFFs.find(
            gpdType);

    //check if not end
    if (it == m_CFFs.end()) {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "CFF of type "
                        << GPDType(gpdType).toString()
                        << " is not supported by this class");
    }

    //value
    double valueDouble = value.toDouble();

    //set
    if (isRealPart) {
        (it->second).real(valueDouble);
    } else {
        (it->second).imag(valueDouble);
    }

    //status
    info(__func__,
            ElemUtils::Formatter() << ((isRealPart) ? ("Real") : ("Imaginary"))
                    << " part of CFF type " << GPDType(gpdType).toString()
                    << " configured with value = " << valueDouble);
}

const std::map<GPDType::Type, std::complex<double> >& DVCSCFFConstant::getCFFs() const {
    return m_CFFs;
}

void DVCSCFFConstant::setCFFs(
        const std::map<GPDType::Type, std::complex<double> >& cffs) {
    m_CFFs = cffs;
}

const std::complex<double>& DVCSCFFConstant::getCFF(
        GPDType::Type gpdType) const {

    //iterator
    std::map<GPDType::Type, std::complex<double> >::const_iterator it =
            m_CFFs.find(gpdType);

    //check if not end
    if (it == m_CFFs.end()) {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "CFF of type "
                        << GPDType(gpdType).toString()
                        << " is not supported by this class");
    }

    //return
    return it->second;
}

void DVCSCFFConstant::setCFF(GPDType::Type gpdType,
        const std::complex<double>& cff) {

    //iterator
    std::map<GPDType::Type, std::complex<double> >::iterator it = m_CFFs.find(
            gpdType);

    //check if not end
    if (it == m_CFFs.end()) {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "CFF of type "
                        << GPDType(gpdType).toString()
                        << " is not supported by this class");
    }

    //set
    it->second = cff;
}

} /* namespace PARTONS */
