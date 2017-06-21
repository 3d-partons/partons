#include "../../../../../../include/partons/modules/observable/DVCS/cross_section/DVCSCrossSection.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <vector>

#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/ProcessModule.h"

const std::string DVCSCrossSection::PARAMETER_NAME_BEAM_HELICITY =
        "beam_helicity";
const std::string DVCSCrossSection::PARAMETER_NAME_BEAM_CHARGE =
        "beam_charge";
const std::string DVCSCrossSection::PARAMETER_NAME_TARGET_POLARIZATION =
        "target_polarization";

// Initialise [class]::classId with a unique name.
const unsigned int DVCSCrossSection::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCrossSection("DVCSCrossSection"));

DVCSCrossSection::DVCSCrossSection(const std::string &className) :
        Observable(className) {
}

DVCSCrossSection::DVCSCrossSection(
        const DVCSCrossSection& other) :
        Observable(other) {
}

DVCSCrossSection::~DVCSCrossSection() {
}

DVCSCrossSection* DVCSCrossSection::clone() const {
    return new DVCSCrossSection(*this);
}

double DVCSCrossSection::computePhiObservable(double phi) {
//    double result = pDVCSModule->computeCrossSection(+1, -1,
//            Vector3D(0., 1., 0.), phi);
//    return result;

    double result = m_pProcessModule->computeCrossSection(m_beamHelicity,
            m_beamCharge, m_targetPolarization, phi);
    return result;
}

void DVCSCrossSection::configure(
        const ElemUtils::Parameters &parameters) {
    if (parameters.isAvailable(
            DVCSCrossSection::PARAMETER_NAME_BEAM_HELICITY)) {
        m_beamHelicity = parameters.getLastAvailable().toDouble();

        info(__func__,
                ElemUtils::Formatter()
                        << DVCSCrossSection::PARAMETER_NAME_BEAM_HELICITY
                        << " configured with value = " << m_beamHelicity);
    }
    if (parameters.isAvailable(
            DVCSCrossSection::PARAMETER_NAME_BEAM_CHARGE)) {
        m_beamCharge = parameters.getLastAvailable().toDouble();

        info(__func__,
                ElemUtils::Formatter()
                        << DVCSCrossSection::PARAMETER_NAME_BEAM_CHARGE
                        << " configured with value = " << m_beamCharge);
    }
    if (parameters.isAvailable(
            DVCSCrossSection::PARAMETER_NAME_TARGET_POLARIZATION)) {

        std::string temp_str = parameters.getLastAvailable().getString();
        if (!temp_str.empty()) {
            std::vector<std::string> vectorPoints =
                    ElemUtils::StringUtils::split(
                            parameters.getLastAvailable().getString(), '|');

            if (vectorPoints.size() == 3) {
                m_targetPolarization = NumA::Vector3D(
                        ElemUtils::GenericType(vectorPoints[0]).toDouble(),
                        ElemUtils::GenericType(vectorPoints[1]).toDouble(),
                        ElemUtils::GenericType(vectorPoints[2]).toDouble());

                info(__func__,
                        ElemUtils::Formatter()
                                << DVCSCrossSection::PARAMETER_NAME_TARGET_POLARIZATION
                                << " configured with value = "
                                << m_targetPolarization.toString());
            } else {
                //TODO exception missing point
            }
        }
    }

    Observable::configure(parameters);
}
