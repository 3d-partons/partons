/*
 * KM10BorderFunctionModel.h
 *
 *  Created on: Oct 17, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#ifndef KM10BORDERFUNCTIONMODEL_H_
#define KM10BORDERFUNCTIONMODEL_H_

#include <string>

#include "../GPDBorderFunctionModule.h"

/**
 * @class KM10BorderFunctionModel
 *
 * @brief Implementation of KM GPD H border function.
 *
 * This module implements the border function for GPD H as proposed by Kumericki and Mueller in Ref. @cite Kumericki:2009uq.
 *
 * For an example of usage of this module see the abstract class documentation.
 */
class KM10BorderFunctionModel: public GPDBorderFunctionModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of this class.
     */
    KM10BorderFunctionModel(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~KM10BorderFunctionModel();

    virtual KM10BorderFunctionModel* clone() const;
    virtual void resolveObjectDependencies();
    void virtual configure(const ElemUtils::Parameters &parameters);
    virtual std::string toString() const;

    virtual PartonDistribution computeH();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    KM10BorderFunctionModel(const KM10BorderFunctionModel& other);

private:

    /**
     * Evaluate diagonal GPD. According to Eq. (107) in Ref. @cite Kumericki:2009uq.
     */
    double getDiagonalGPD(double xi, double n, double r, double alpha, double b, double M,
            double p) const;

    double m_par_H_b_val;   ///< Parameter H_b_val
    double m_par_H_r_val;   ///< Parameter H_r_val
    double m_par_H_M_val;   ///< Parameter H_M_val

    double m_par_H_b_sea;   ///< Parameter H_b_sea
    double m_par_H_r_sea;   ///< Parameter H_r_sea
    double m_par_H_M_sea;   ///< Parameter H_M_sea

    double m_par_H_n_val;       ///< Parameter n_val
    double m_par_H_delta_val;   ///< Parameter delta_val
    double m_par_H_alpha_val;   ///< Parameter alpha_val
    double m_par_H_p_val;       ///< Parameter p_val

    double m_par_H_n_sea;       ///< Parameter n_sea
    double m_par_H_delta_sea;   ///< Parameter delta_sea
    double m_par_H_alpha_sea;   ///< Parameter alpha_sea
    double m_par_H_p_sea;       ///< Parameter p_sea
};

#endif /* KM10BORDERFUNCTIONMODEL_H_ */
