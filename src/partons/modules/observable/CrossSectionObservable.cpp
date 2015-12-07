#include "../../../../include/partons/modules/observable/CrossSectionObservable.h"

#include <NumA/linear_algebra/vector/Vector3D.h>
#include <vector>

#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/ProcessModule.h"
#include "../../../../include/partons/utils/GenericType.h"
#include "../../../../include/partons/utils/ParameterList.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"
#include "../../../../include/partons/utils/stringUtils/StringUtils.h"

const std::string CrossSectionObservable::PARAMETER_NAME_BEAM_HELICITY =
        "beam_helicity";
const std::string CrossSectionObservable::PARAMETER_NAME_BEAM_CHARGE =
        "beam_charge";
const std::string CrossSectionObservable::PARAMETER_NAME_TARGET_POLARIZATION =
        "target_polarization";

// Initialise [class]::classId with a unique name.
const unsigned int CrossSectionObservable::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new CrossSectionObservable("CrossSectionObservable"));

CrossSectionObservable::CrossSectionObservable(const std::string &className) :
        Observable(className) {
}

CrossSectionObservable::CrossSectionObservable(
        const CrossSectionObservable& other) :
        Observable(other) {
}

CrossSectionObservable::~CrossSectionObservable() {
}

CrossSectionObservable* CrossSectionObservable::clone() const {
    return new CrossSectionObservable(*this);
}

double CrossSectionObservable::compute(ProcessModule* pDVCSModule, double phi) {
//    double result = pDVCSModule->computeCrossSection(+1, -1,
//            Vector3D(0., 1., 0.), phi);
//    return result;

    double result = pDVCSModule->computeCrossSection(m_beamHelicity,
            m_beamCharge, m_targetPolarization, phi);
    return result;
}

void CrossSectionObservable::configure(ParameterList parameters) {
    if (parameters.isAvailable(
            CrossSectionObservable::PARAMETER_NAME_BEAM_HELICITY)) {
        m_beamHelicity = parameters.getLastAvailable().toDouble();

        info(__func__,
                Formatter()
                        << CrossSectionObservable::PARAMETER_NAME_BEAM_HELICITY
                        << " configured with value = " << m_beamHelicity);
    }
    if (parameters.isAvailable(
            CrossSectionObservable::PARAMETER_NAME_BEAM_CHARGE)) {
        m_beamCharge = parameters.getLastAvailable().toDouble();

        info(__func__,
                Formatter()
                        << CrossSectionObservable::PARAMETER_NAME_BEAM_CHARGE
                        << " configured with value = " << m_beamCharge);
    }
    if (parameters.isAvailable(
            CrossSectionObservable::PARAMETER_NAME_TARGET_POLARIZATION)) {

        std::string temp_str = parameters.getLastAvailable().toString();
        if (!temp_str.empty()) {
            std::vector<std::string> vectorPoints = StringUtils::split(
                    parameters.getLastAvailable().toString(), '|');

            if (vectorPoints.size() == 3) {
                m_targetPolarization = NumA::Vector3D(
                        StringUtils::fromStringToDouble(vectorPoints[0]),
                        StringUtils::fromStringToDouble(vectorPoints[1]),
                        StringUtils::fromStringToDouble(vectorPoints[2]));

                info(__func__,
                        Formatter()
                                << CrossSectionObservable::PARAMETER_NAME_TARGET_POLARIZATION
                                << " configured with value = "
                                << m_targetPolarization.toString());
            } else {
                //TODO exception missing point
            }
        }
    }
}
