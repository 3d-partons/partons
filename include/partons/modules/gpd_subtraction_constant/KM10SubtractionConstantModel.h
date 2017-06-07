/*
 * KM10SubtractionConstantModel.h
 *
 *  Created on: Oct 18, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#ifndef KM10SUBTRACTIONCONSTANTMODEL_H_
#define KM10SUBTRACTIONCONSTANTMODEL_H_

#include <string>

#include "../GPDSubtractionConstantModule.h"

/**
 * @class KM10SubtractionConstantModel
 *
 * @brief Implementation of KM subtraction constant.
 *
 * This module implements the subtraction constant as proposed by Kumericki and Mueller in Ref. @cite Kumericki:2009uq.
 *
 * For an example of usage of this module see the abstract class documentation.
 */
class KM10SubtractionConstantModel: public GPDSubtractionConstantModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of this class.
     */
    KM10SubtractionConstantModel(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~KM10SubtractionConstantModel();

    virtual KM10SubtractionConstantModel* clone() const;

protected:

    /** Copy constructor.
     * @param other Object to be copied.
     */
    KM10SubtractionConstantModel(const KM10SubtractionConstantModel& other);

    virtual double computeH();
    virtual double computeE();

private:

    double m_par_H_C;       ///< H_C parameter
    double m_par_H_M_sub;   ///< H_M_sub parameter
};

#endif /* KM10SUBTRACTIONCONSTANTMODEL_H_ */
