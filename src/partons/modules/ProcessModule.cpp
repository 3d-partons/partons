#include "../../../include/partons/modules/ProcessModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>

#include "../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../include/partons/modules/scale/ScaleModule.h"
#include "../../../include/partons/modules/xb_to_xi/XiConverterModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"

const std::string ProcessModule::PROCESS_MODULE_CLASS_NAME = "ProcessModule";

ProcessModule::ProcessModule(const std::string &className) :
        ModuleObject(className), m_isCCFModuleDependent(true), m_phi(0.), m_phiS(
                0.), m_phie(0.), m_xB(0.), m_t(0.), m_Q2(0.), m_E(0.), m_pScaleModule(
                0), m_pXiConverterModule(0), m_pConvolCoeffFunctionModule(0), m_channel(
                ObservableChannel::UNDEFINED) {
}

ProcessModule::ProcessModule(const ProcessModule &other) :
        ModuleObject(other) {
    m_isCCFModuleDependent = other.m_isCCFModuleDependent;

    m_channel = other.m_channel;

    m_xB = other.m_xB;
    m_t = other.m_t;
    m_Q2 = other.m_Q2;

    m_E = other.m_E;

    m_phi = other.m_phi;
    m_phiS = other.m_phiS;
    m_phie = other.m_phie;

    if (other.m_pScaleModule != 0) {
        m_pScaleModule = other.m_pScaleModule->clone();
    } else {
        m_pScaleModule = 0;
    }

    if (other.m_pXiConverterModule != 0) {
        m_pXiConverterModule = other.m_pXiConverterModule->clone();
    } else {
        m_pXiConverterModule = 0;
    }

    if (other.m_pConvolCoeffFunctionModule != 0) {
        m_pConvolCoeffFunctionModule =
                other.m_pConvolCoeffFunctionModule->clone();
    } else {
        m_pConvolCoeffFunctionModule = 0;
    }
}

ProcessModule::~ProcessModule() {
    if (m_pConvolCoeffFunctionModule != 0) {
        setConvolCoeffFunctionModule(0);
        m_pConvolCoeffFunctionModule = 0;
    }

    if (m_pScaleModule != 0) {
        setScaleModule(0);
        m_pScaleModule = 0;
    }

    if (m_pXiConverterModule != 0) {
        setXiConverterModule(0);
        m_pXiConverterModule = 0;
    }
}

void ProcessModule::configure(const ElemUtils::Parameters &parameters) {
    ModuleObject::configure(parameters);
}

//TODO vérifier que la précision du double ne fausse pas le résultat
bool ProcessModule::isPreviousKinematicsDifferent(double xB, double t,
        double Q2) {
    bool result = false;
    if (xB != m_xB || t != m_t || Q2 != m_Q2) {
        result = true;
    }
    return result;
}

void ProcessModule::resetPreviousKinematics() {
    m_xB = 0.;
    m_t = 0.;
    m_Q2 = 0.;
}

void ProcessModule::setConvolCoeffFunctionModule(
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule) {

    m_pModuleObjectFactory->updateModulePointerReference(
            m_pConvolCoeffFunctionModule, pConvolCoeffFunctionModule);
    m_pConvolCoeffFunctionModule = pConvolCoeffFunctionModule;

    if (m_pConvolCoeffFunctionModule != 0) {
        info(__func__,
                ElemUtils::Formatter()
                        << "ConvolCoeffFunctionModule is set to : "
                        << pConvolCoeffFunctionModule->getClassName());
    } else {
        info(__func__, "ConvolCoeffFunctionModule is set to : 0");
    }

    resetPreviousKinematics();
}

void ProcessModule::setScaleModule(ScaleModule* pScaleModule) {
    m_pScaleModule = pScaleModule;
}

void ProcessModule::setXiConverterModule(
        XiConverterModule* pXiConverterModule) {
    m_pXiConverterModule = pXiConverterModule;
}

bool ProcessModule::isCCFModuleDependent() const {
    return m_isCCFModuleDependent;
}

void ProcessModule::isCCFModuleDependent(bool isCcfModuleDependent) {
    m_isCCFModuleDependent = isCcfModuleDependent;
}

ConvolCoeffFunctionModule* ProcessModule::getConvolCoeffFunctionModule() const {
    return m_pConvolCoeffFunctionModule;
}

void ProcessModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    ModuleObject::prepareSubModules(subModulesData);

    std::map<std::string, BaseObjectData>::const_iterator it;

    it = subModulesData.find(ScaleModule::SCALE_MODULE_CLASS_NAME);

    if (it != subModulesData.end()) {
        if (m_pScaleModule) {
            setScaleModule(0);
            m_pScaleModule = 0;
        }
        if (!m_pScaleModule) {
            m_pScaleModule =
                    Partons::getInstance()->getModuleObjectFactory()->newScaleModule(
                            (it->second).getModuleClassName());

            info(__func__,
                    ElemUtils::Formatter() << "Configured with ScaleModule = "
                            << m_pScaleModule->getClassName());

            m_pScaleModule->configure((it->second).getParameters());

            m_pScaleModule->prepareSubModules((it->second).getSubModules());
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << getClassName()
                        << " is ScaleModule dependent and you have not provided one");
    }

    it = subModulesData.find(XiConverterModule::XI_CONVERTER_MODULE_CLASS_NAME);

    if (it != subModulesData.end()) {
        if (m_pXiConverterModule) {
            setXiConverterModule(0);
            m_pXiConverterModule = 0;
        }
        if (!m_pXiConverterModule) {
            m_pXiConverterModule =
                    Partons::getInstance()->getModuleObjectFactory()->newXiConverterModule(
                            (it->second).getModuleClassName());

            info(__func__,
                    ElemUtils::Formatter()
                            << "Configured with XiConverterModule = "
                            << m_pXiConverterModule->getClassName());

            m_pXiConverterModule->configure((it->second).getParameters());

            m_pXiConverterModule->prepareSubModules(
                    (it->second).getSubModules());
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << getClassName()
                        << " is XiConverterModule dependent and you have not provided one");
    }

    if (isCCFModuleDependent()) {
        it =
                subModulesData.find(
                        ConvolCoeffFunctionModule::CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME);

        if (it != subModulesData.end()) {
            if (m_pConvolCoeffFunctionModule) {
                setConvolCoeffFunctionModule(0);
                m_pConvolCoeffFunctionModule = 0;
            }
            if (!m_pConvolCoeffFunctionModule) {
                m_pConvolCoeffFunctionModule =
                        Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                                (it->second).getModuleClassName());

                info(__func__,
                        ElemUtils::Formatter()
                                << "Configured with ConvolCoeffFunctionModule = "
                                << m_pConvolCoeffFunctionModule->getClassName());

                m_pConvolCoeffFunctionModule->configure(
                        (it->second).getParameters());

                m_pConvolCoeffFunctionModule->prepareSubModules(
                        (it->second).getSubModules());
            }
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << getClassName()
                            << " is ConvolCoeffFunctionModule dependent and you have not provided one");
        }
    }
}
