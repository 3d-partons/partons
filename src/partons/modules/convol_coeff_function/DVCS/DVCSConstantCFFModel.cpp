#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConstantCFFModel.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <utility>
#include <vector>

#include "../../../../../include/partons/BaseObjectRegistry.h"

// initialize [class]::classId with a unique name.
const unsigned int DVCSConstantCFFModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSConstantCFFModel("DVCSConstantCFFModel"));

// set key names
const std::string DVCSConstantCFFModel::CFF_VALUES = "cff_values";

const std::string DVCSConstantCFFModel::CFF_H_Re = "cff_value_H_Re";
const std::string DVCSConstantCFFModel::CFF_H_Im = "cff_value_H_Im";
const std::string DVCSConstantCFFModel::CFF_E_Re = "cff_value_E_Re";
const std::string DVCSConstantCFFModel::CFF_E_Im = "cff_value_E_Im";
const std::string DVCSConstantCFFModel::CFF_Ht_Re = "cff_value_Ht_Re";
const std::string DVCSConstantCFFModel::CFF_Ht_Im = "cff_value_Ht_Im";
const std::string DVCSConstantCFFModel::CFF_Et_Re = "cff_value_Et_Re";
const std::string DVCSConstantCFFModel::CFF_Et_Im = "cff_value_Et_Im";

DVCSConstantCFFModel::DVCSConstantCFFModel(const std::string &className) :
        DVCSConvolCoeffFunctionModule(className) {
    setIsGPDModuleDependent(false);

    //initialize
    m_CFF.clear();

    //insert and assign
    for (int i = static_cast<int>(GPDType::H);
            i < static_cast<int>(GPDType::END); i++) {

        //insert to map
        m_CFF.insert(std::make_pair(static_cast<GPDType::Type>(i), std::complex<double>(0., 0.)));

        //assign function
        m_listOfCFFComputeFunctionAvailable.insert(
                std::make_pair(static_cast<GPDType::Type>(i),
                        &DVCSConvolCoeffFunctionModule::computeCFF));
    }
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

    // initialize parent module before
    DVCSConvolCoeffFunctionModule::initModule();
}

void DVCSConstantCFFModel::isModuleWellConfigured() {

    // check parent module before
    DVCSConvolCoeffFunctionModule::isModuleWellConfigured();
}

std::complex<double> DVCSConstantCFFModel::computeCFF() {

    std::map<GPDType::Type, std::complex<double> >::iterator it = m_CFF.find(
            m_currentGPDComputeType);

    if (it == m_CFF.end()) {

        warn(__func__,
                ElemUtils::Formatter() << "CFF of type "
                        << GPDType(m_currentGPDComputeType).toString()
                        << " is not supported by this class");

        return std::complex<double>(0., 0.);

    } else {
        return it->second;
    }
}

void DVCSConstantCFFModel::configure(const ElemUtils::Parameters &parameters) {

    //clear
    for (std::map<GPDType::Type, std::complex<double> >::iterator it =
            m_CFF.begin(); it != m_CFF.end(); it++) {
        (it->second) = std::complex<double>(0., 0.);
    }

    // several CFFs
    if (parameters.isAvailable(DVCSConstantCFFModel::CFF_VALUES)) {

        // to string
        std::string temp_str = parameters.getLastAvailable().toString();

        // check if not empty
        if (!temp_str.empty()) {

            // split string
            std::vector<std::string> CFFValues = ElemUtils::StringUtils::split(
                    temp_str, '|');

            // iterator
            std::map<GPDType::Type, std::complex<double> >::iterator it =
                    m_CFF.begin();

            // assign
            for (int i = 0; i < CFFValues.size(); i++) {

                //check if not end
                if (it == m_CFF.end()) {

                    warn(__func__,
                            ElemUtils::Formatter()
                                    << "Too many elements for key "
                                    << DVCSConstantCFFModel::CFF_VALUES
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
                            << DVCSConstantCFFModel::CFF_VALUES << " is empty");
        }
    }

    // single CFFs
    if (parameters.isAvailable(DVCSConstantCFFModel::CFF_H_Re)) {
        configureValue(GPDType::H, 1, parameters.getLastAvailable());
    }
    if (parameters.isAvailable(DVCSConstantCFFModel::CFF_H_Im)) {
        configureValue(GPDType::H, 0, parameters.getLastAvailable());
    }
    if (parameters.isAvailable(DVCSConstantCFFModel::CFF_E_Re)) {
        configureValue(GPDType::E, 1, parameters.getLastAvailable());
    }
    if (parameters.isAvailable(DVCSConstantCFFModel::CFF_E_Im)) {
        configureValue(GPDType::E, 0, parameters.getLastAvailable());
    }
    if (parameters.isAvailable(DVCSConstantCFFModel::CFF_Ht_Re)) {
        configureValue(GPDType::Ht, 1, parameters.getLastAvailable());
    }
    if (parameters.isAvailable(DVCSConstantCFFModel::CFF_Ht_Im)) {
        configureValue(GPDType::Ht, 0, parameters.getLastAvailable());
    }
    if (parameters.isAvailable(DVCSConstantCFFModel::CFF_Et_Re)) {
        configureValue(GPDType::Et, 1, parameters.getLastAvailable());
    }
    if (parameters.isAvailable(DVCSConstantCFFModel::CFF_Et_Im)) {
        configureValue(GPDType::Et, 0, parameters.getLastAvailable());
    }

    //configure mother class
    DVCSConvolCoeffFunctionModule::configure(parameters);
}

void DVCSConstantCFFModel::configureValue(GPDType::Type gpdType,
        bool isRealPart, const ElemUtils::GenericType& value) {

    //iterator
    std::map<GPDType::Type, std::complex<double> >::iterator it = m_CFF.find(
            gpdType);

    //check if not end
    if (it == m_CFF.end()) {

        warn(__func__,
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
            ElemUtils::Formatter() << ((isRealPart) ?
                    ("Real") :
                    ("Imaginary")) << " part of CFF type "
                            << GPDType(gpdType).toString()
                            << " configured with value = " << valueDouble);
}
