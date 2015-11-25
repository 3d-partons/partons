/*
 * ComplexDaoService.h
 *
 *  Created on: Nov 24, 2015
 *      Author: debian
 */

#ifndef COMPLEX_DAO_SERVICE_H
#define COMPLEX_DAO_SERVICE_H

#include <complex>

#include "../dao/ComplexDao.h"

class ComplexDaoService: public BaseObject {
public:
    ComplexDaoService();
    virtual ~ComplexDaoService();

    int insert(const std::complex<double> &complex) const;
    std::complex<double> getComplexById(const int id) const;
    int getComplexId(const std::complex<double> &complex) const;

private:
    ComplexDao m_complexDao;
};

#endif /* COMPLEX_DAO_SERVICE_H */
