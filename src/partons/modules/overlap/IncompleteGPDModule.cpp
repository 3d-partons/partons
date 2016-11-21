#include "../../../../include/partons/modules/overlap/IncompleteGPDModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>

#include "../../../../include/partons/modules/GPDModule.h"

const std::string IncompleteGPDModule::DGLAP_REGION = "DGLAP";
const std::string IncompleteGPDModule::ERBL_REGION = "ERBL";

IncompleteGPDModule::IncompleteGPDModule(const std::string &className) :
        ModuleObject(className), m_x(0.), m_xi(0.), m_t(0.), m_MuF2(0.), m_MuR2(
                0.), m_gpdType(GPDType::ALL) {
}

IncompleteGPDModule::IncompleteGPDModule(const IncompleteGPDModule &other) :
        ModuleObject(other) {
    m_x = other.m_x;
    m_xi = other.m_xi;
    m_t = other.m_t;
    m_MuF2 = other.m_MuF2;
    m_MuR2 = other.m_MuR2;

    m_gpdType = other.m_gpdType;

    m_kinematicRegion = other.m_kinematicRegion;

    m_listGPDComputeTypeAvailable = other.m_listGPDComputeTypeAvailable;
    m_it = other.m_it;
}

IncompleteGPDModule::~IncompleteGPDModule() {
    //TODO Remove GPDEvolution pointer
}

void IncompleteGPDModule::configure(const ElemUtils::Parameters &parameters) {
    if (parameters.isAvailable(GPDModule::GPD_TYPE)) {
        m_gpdType =
                static_cast<GPDType::Type>(parameters.getLastAvailable().toUInt());
    }

    ModuleObject::configure(parameters);
}

//TODO implement
void IncompleteGPDModule::initModule() {
    debug(__func__, ElemUtils::Formatter() << "executed");
}

void IncompleteGPDModule::isModuleWellConfigured() {
    debug(__func__, ElemUtils::Formatter() << "executed");

    // Test variable range

    if (!isInKinematicRegion(m_x, m_xi)) {
        warn(__func__,
                ElemUtils::Formatter()
                        << "x and xi are not in the kinematic region "
                        << getKinematicRegion() << " described by the module "
                        << getClassName());
    }

    if (m_t > 0.) {
        warn(__func__, "Nucleon momentum transfer should be <= 0.");
    }

    if (m_MuF2 < 0.) {
        warn(__func__, "Square of factorization scale should be >= 0.");
    }
}

void IncompleteGPDModule::preCompute(double x, double xi, double t, double MuF,
        double MuR, GPDType::Type gpdType) {
    m_x = x;
    m_xi = xi;
    m_t = t;
    m_MuF2 = MuF;
    m_MuR2 = MuR;
    m_gpdType = gpdType;

    debug(__func__,
            ElemUtils::Formatter() << "x = " << m_x << "    xi = " << m_xi
                    << "    t = " << m_t << " GeV2    MuF = " << m_MuF2
                    << " GeV    MuR = " << m_MuR2 << " GeV");

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();
}

PartonDistribution IncompleteGPDModule::compute(const GPDKinematic &kinematic,
        GPDType gpdType) {
    return compute(kinematic.getX(), kinematic.getXi(), kinematic.getT(),
            kinematic.getMuF2(), kinematic.getMuR2(), gpdType.getType());
}

PartonDistribution IncompleteGPDModule::compute(double x, double xi, double t,
        double MuF2, double MuR2, GPDType::Type gpdType) {

    preCompute(x, xi, t, MuF2, MuR2, gpdType);

    PartonDistribution partonDistribution;
    m_it = m_listGPDComputeTypeAvailable.find(m_gpdType);
    if (m_it != m_listGPDComputeTypeAvailable.end()) {
        partonDistribution = ((*this).*(m_it->second))();
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "GPD("
                        << GPDType(m_gpdType).toString()
                        << ") is not available for this GPD model");
    }

    debug(__func__, ElemUtils::Formatter() << partonDistribution.toString());

    return partonDistribution;
}

//TODO implement
PartonDistribution IncompleteGPDModule::computeH() {
    throw std::runtime_error("");
}

PartonDistribution IncompleteGPDModule::computeE() {
    throw std::runtime_error("");
}

PartonDistribution IncompleteGPDModule::computeHt() {
    throw std::runtime_error("");
}

PartonDistribution IncompleteGPDModule::computeEt() {
    throw std::runtime_error("");
}

std::string IncompleteGPDModule::toString() {
    return ModuleObject::toString();
}

double IncompleteGPDModule::getMuF2() const {
    return m_MuF2;
}

void IncompleteGPDModule::setMuF2(double muF2) {
    m_MuF2 = muF2;
}

double IncompleteGPDModule::getMuR2() const {
    return m_MuR2;
}

void IncompleteGPDModule::setMuR2(double muR2) {
    m_MuR2 = muR2;
}

double IncompleteGPDModule::getT() const {
    return m_t;
}

void IncompleteGPDModule::setT(double t) {
    m_t = t;
}

double IncompleteGPDModule::getX() const {
    return m_x;
}

void IncompleteGPDModule::setX(double x) {
    m_x = x;
}

double IncompleteGPDModule::getXi() const {
    return m_xi;
}

void IncompleteGPDModule::setXi(double xi) {
    m_xi = xi;
}

List<GPDType> IncompleteGPDModule::getListOfAvailableGPDTypeForComputation() const {
    std::map<GPDType::Type, PartonDistribution (IncompleteGPDModule::*)()>::const_iterator it;
    List<GPDType> listOfAvailableGPDTypeForComputation;

    for (it = m_listGPDComputeTypeAvailable.begin();
            it != m_listGPDComputeTypeAvailable.end(); it++) {
        listOfAvailableGPDTypeForComputation.add(it->first);
    }

    return listOfAvailableGPDTypeForComputation;
}

const std::string& IncompleteGPDModule::getKinematicRegion() const {
    return m_kinematicRegion;
}

void IncompleteGPDModule::setKinematicRegion(
        const std::string& kinematicRegion) {
    m_kinematicRegion = kinematicRegion;
}

void IncompleteGPDModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    ModuleObject::prepareSubModules(subModulesData);
}
