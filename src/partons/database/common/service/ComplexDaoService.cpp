#include "../../../../../include/partons/database/common/service/ComplexDaoService.h"

ComplexDaoService::ComplexDaoService() :
        BaseObject("ComplexDaoService") {
}

ComplexDaoService::~ComplexDaoService() {
}

int ComplexDaoService::insert(const std::complex<double>& complex) const {
    return m_complexDao.insert(complex.real(), complex.imag());
}

std::complex<double> ComplexDaoService::getComplexById(const int id) const {
    return m_complexDao.getComplexById(id);
}

int ComplexDaoService::getComplexId(const std::complex<double>& complex) const {
    return m_complexDao.select(complex.real(), complex.imag());
}
