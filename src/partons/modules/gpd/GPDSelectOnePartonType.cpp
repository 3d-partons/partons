#include "../../../../include/partons/modules/gpd/GPDSelectOnePartonType.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <iterator>
#include <utility>
#include <vector>

#include "../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int GPDSelectOnePartonType::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDSelectOnePartonType("GPDSelectOnePartonType"));

const std::string GPDSelectOnePartonType::PARAMETER_NAME_SELECTONEPARTONTYPE_PARTONTYPE =
        "parton_type";

GPDSelectOnePartonType::GPDSelectOnePartonType(const std::string &className) :
        GPDModule(className), m_pGPDModule(0) {

    for (int i = static_cast<int>(GPDType::H);
            i < static_cast<int>(GPDType::END); i++) {

        m_listGPDComputeTypeAvailable.insert(
                std::make_pair(static_cast<GPDType::Type>(i),
                        &GPDModule::computeH));
    }

    m_MuF2_ref = 1.; //reference scale (not used but should be set to avoid warning)
}

GPDSelectOnePartonType::GPDSelectOnePartonType(
        const GPDSelectOnePartonType& other) :
        GPDModule(other) {

    m_partonType = other.m_partonType;

    if (other.m_pGPDModule != 0) {
        m_pGPDModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pGPDModule);
    } else {
        m_pGPDModule = 0;
    }
}

GPDSelectOnePartonType::~GPDSelectOnePartonType() {

    if (m_pGPDModule) {
        setGPDModule(0);
        m_pGPDModule = 0;
    }
}

GPDSelectOnePartonType* GPDSelectOnePartonType::clone() const {
    return new GPDSelectOnePartonType(*this);
}

void GPDSelectOnePartonType::configure(
        const ElemUtils::Parameters &parameters) {

    GPDModule::configure(parameters);

    if (parameters.isAvailable(
            GPDSelectOnePartonType::PARAMETER_NAME_SELECTONEPARTONTYPE_PARTONTYPE)) {

        setPartonType(parameters.getLastAvailable().getString());

        info(__func__,
                ElemUtils::Formatter() << "Parameter "
                        << GPDSelectOnePartonType::PARAMETER_NAME_SELECTONEPARTONTYPE_PARTONTYPE
                        << " set to "
                        << parameters.getLastAvailable().getString());
    }
}

void GPDSelectOnePartonType::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    GPDModule::prepareSubModules(subModulesData);

    std::map<std::string, BaseObjectData>::const_iterator it;

    it = subModulesData.find(GPDModule::GPD_MODULE_CLASS_NAME);

    if (it != subModulesData.end()) {

        if (m_pGPDModule) {

            setGPDModule(0);
            m_pGPDModule = 0;
        }

        if (!m_pGPDModule) {

            m_pGPDModule =
                    Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                            (it->second).getModuleClassName());

            info(__func__,
                    ElemUtils::Formatter() << "Configure with GPDModule = "
                            << m_pGPDModule->getClassName());

            m_pGPDModule->configure((it->second).getParameters());
            m_pGPDModule->prepareSubModules((it->second).getSubModules());
        }
    }
}

PartonDistribution GPDSelectOnePartonType::computeH() {

    //check if GPD module set
    if (!m_pGPDModule) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "GPD module not set");
    }

    //check if parton type selected
    if (m_partonType.empty()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Parton type not selected");
    }

    //evaluate
    PartonDistribution result = m_pGPDModule->compute(
            GPDKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    //new result
    PartonDistribution newResult;

    //get list of computed flavors
    std::vector<QuarkFlavor::Type> flavorList = result.listTypeOfQuarkFlavor();

    //iterator
    std::vector<QuarkFlavor::Type>::const_iterator it;

    //parton type
    if (m_partonType == "g") {

        newResult.setGluonDistribution(result.getGluonDistribution());

        for (it = flavorList.begin(); it != flavorList.end(); it++) {
            newResult.addQuarkDistribution(QuarkDistribution(*it, 0., 0., 0.));
        }

    } else {

        QuarkFlavor::Type quarkFlavor = QuarkFlavor().fromString(m_partonType);

        if (quarkFlavor == QuarkFlavor::UNDEFINED) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Parton type is undefined or conversion from string faild: "
                            << m_partonType);
        }

        newResult.setGluonDistribution(GluonDistribution(0.));

        for (it = flavorList.begin(); it != flavorList.end(); it++) {

            if (*it == quarkFlavor) {
                newResult.addQuarkDistribution(
                        result.getQuarkDistribution(*it));
            } else {
                newResult.addQuarkDistribution(
                        QuarkDistribution(*it, 0., 0., 0.));
            }
        }

    }

    //return
    return newResult;
}

const GPDModule* GPDSelectOnePartonType::getGPDModule() const {
    return m_pGPDModule;
}

void GPDSelectOnePartonType::setGPDModule(GPDModule* pGPDModule) {
    m_pModuleObjectFactory->updateModulePointerReference(m_pGPDModule,
            pGPDModule);
    m_pGPDModule = pGPDModule;
}

const std::string& GPDSelectOnePartonType::getPartonType() const {
    return m_partonType;
}

void GPDSelectOnePartonType::setPartonType(const std::string& partonType) {
    m_partonType = partonType;
}

} /* namespace PARTONS */
