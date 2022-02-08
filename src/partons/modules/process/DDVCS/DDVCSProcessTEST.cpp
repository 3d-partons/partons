#include "../../../../../include/partons/modules/process/DDVCS/DDVCSProcessTEST.h"

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DDVCSProcessTEST::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSProcessTEST("DDVCSProcessTEST"));

DDVCSProcessTEST::DDVCSProcessTEST(const std::string &className) :
        DDVCSProcessModule(className) {
}

DDVCSProcessTEST::DDVCSProcessTEST(const DDVCSProcessTEST& other) :
        DDVCSProcessModule(other) {
}

DDVCSProcessTEST::~DDVCSProcessTEST() {
}

DDVCSProcessTEST* DDVCSProcessTEST::clone() const {
    return new DDVCSProcessTEST(*this);
}

void DDVCSProcessTEST::initModule() {

    // Init mother class
    DDVCSProcessModule::initModule();
}

void DDVCSProcessTEST::isModuleWellConfigured() {

    //check mother class
    DDVCSProcessModule::isModuleWellConfigured();
}

PhysicalType<double> DDVCSProcessTEST::CrossSectionBH() {

    //availible variables
    m_xB;
    m_t;
    m_Q2;
    m_Q2Prim;
    m_E;
    m_phi;
    m_theta;

    m_dvcsConvolCoeffFunctionResult;

    m_beamHelicity;
    m_beamCharge;
    m_targetPolarization;

    //additionally (if we decide to keep them)
    m_tmin;
    m_tmax;
    m_xBmin;
    m_y;
    m_epsilon;

    return PhysicalType<double>(1., PhysicalUnit::GEVm2);
}

PhysicalType<double> DDVCSProcessTEST::CrossSectionVCS() {
    return PhysicalType<double>(2., PhysicalUnit::GEVm2);
}

PhysicalType<double> DDVCSProcessTEST::CrossSectionInterf() {
    return PhysicalType<double>(3., PhysicalUnit::GEVm2);
}

} /* namespace PARTONS */
