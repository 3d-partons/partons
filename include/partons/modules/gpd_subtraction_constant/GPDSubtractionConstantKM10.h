/*
 * GPDSubtractionConstantKM10.h
 *
 *  Created on: Oct 18, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#ifndef GPD_SUBTRACTION_CONSTANT_KM10_H
#define GPD_SUBTRACTION_CONSTANT_KM10_H

#include <string>

#include "GPDSubtractionConstantModule.h"

/**
 * @class GPDSubtractionConstantKM10
 *
 * @brief Implementation of KM subtraction constant.
 *
 * This module implements the subtraction constant as proposed by Kumericki and Mueller in Ref. @cite Kumericki:2009uq.
 *
 * For an example of usage of this module see the abstract class documentation.
 */
class GPDSubtractionConstantKM10: public GPDSubtractionConstantModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of this class.
     */
    GPDSubtractionConstantKM10(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~GPDSubtractionConstantKM10();

    virtual GPDSubtractionConstantKM10* clone() const;

protected:

    /** Copy constructor.
     * @param other Object to be copied.
     */
    GPDSubtractionConstantKM10(const GPDSubtractionConstantKM10& other);

    virtual double computeH();
    virtual double computeE();

private:

    double m_par_H_C;       ///< H_C parameter
    double m_par_H_M_sub;   ///< H_M_sub parameter
};

#endif /* GPD_SUBTRACTION_CONSTANT_KM10_H */
