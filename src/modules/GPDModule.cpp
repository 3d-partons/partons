#include "GPDModule.h"

#include "EvolQCDModule.h"

GPDModule::GPDModule(const std::string &moduleID) :
		ModuleObject(moduleID), m_pEvolQCDModule(0) {
}

GPDModule::GPDModule(const GPDModule &other) :
		ModuleObject(other) {
	m_listGPDComputeTypeAvailable = other.m_listGPDComputeTypeAvailable;
	m_it = other.m_it;

	m_x = other.m_x;
	m_xi = other.m_xi;
	m_t = other.m_t;
	m_MuF = other.m_MuF;
	m_MuR = other.m_MuR;

	m_MuF_ref = other.m_MuF_ref;

	if (other.m_pEvolQCDModule != 0) {
		m_pEvolQCDModule = other.m_pEvolQCDModule->clone();
	}
	else
	{
	    m_pEvolQCDModule = 0;
	}

//		if (other.m_pPDFModule != 0) {
//			m_pPDFModule = other.m_pPDFModule->clone();
//		}
}

GPDModule::~GPDModule() {
}

double GPDModule::getNf() const {
	return m_nf;
}

void GPDModule::setNf(double nf) {
	m_nf = nf;
}

const EvolQCDModule* GPDModule::getEvolQcdModule() const {
	return m_pEvolQCDModule;
}

//TODO est-il nécessaire de tester le pointeur null ?
void GPDModule::setEvolQcdModule(EvolQCDModule* pEvolQcdModule) {
	m_pEvolQCDModule = pEvolQcdModule;
	if (m_pEvolQCDModule != 0)
		m_pEvolQCDModule->setGpdModule(this);
}

double GPDModule::getMuFRef() const {
	return m_MuF_ref;
}

std::string GPDModule::toString()
{
    return ModuleObject::toString();
}
