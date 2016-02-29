/*
 * ComplexDao.h
 *
 *  Created on: Nov 24, 2015
 *      Author: debian
 */

#ifndef COMPLEX_DAO_H
#define COMPLEX_DAO_H

#include <complex>
#include <QtSql/qsqlquery.h>

#include "../../../BaseObject.h"

class ComplexDao: public BaseObject {
public:
    ComplexDao();
    virtual ~ComplexDao();

    int insert(const double realPart, const double imgPart) const;
    int select(const double realPart, const double imgPart) const;

    std::complex<double> getComplexById(const int id) const;

private:

    std::complex<double> getComplexFromQuery(QSqlQuery &query) const;
};

#endif /* COMPLEX_DAO_H */
