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
 * GPD border function model used in Kumericki-Mueller fit
 * see Nucl. Phys. B841, 1 (2010)
 */
class KM10BorderFunctionModel: public GPDBorderFunctionModule {

public:

    static const unsigned int classId; ///< ID assigned by BaseObjectRegistry

    /** Constructor
     @param className Name of this class
     */
    KM10BorderFunctionModel(const std::string &className);

    /**
     * Destructor
     */
    virtual ~KM10BorderFunctionModel();

    virtual KM10BorderFunctionModel* clone() const;
    virtual void resolveObjectDependencies();
    void virtual configure(const ElemUtils::Parameters &parameters);
    virtual std::string toString();

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();
    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeEt();

    double getParEBVal() const;
    void setParEBVal(double parEBVal);
    double getParEMVal() const;
    void setParEMVal(double parEMVal);
    double getParERVal() const;
    void setParERVal(double parERVal);
    double getParHBSea() const;
    void setParHBSea(double parHBSea);
    double getParHBVal() const;
    void setParHBVal(double parHBVal);
    double getParHMSea() const;
    void setParHMSea(double parHMSea);
    double getParHMVal() const;
    void setParHMVal(double parHMVal);
    double getParHRSea() const;
    void setParHRSea(double parHRSea);
    double getParHRVal() const;
    void setParHRVal(double parHRVal);
    double getParHtBVal() const;
    void setParHtBVal(double parHtBVal);
    double getParHtMVal() const;
    void setParHtMVal(double parHtMVal);
    double getParHtRVal() const;
    void setParHtRVal(double parHtRVal);

private:

    /** Get diagonal GPD according to Eq. (107) in  http://arxiv.org/pdf/0904.0458v2.pdf
     */
    double getDiagonalGPD(double xi, double r, double alpha, double b, double M,
            double p) const;

    GPDModule* m_pGK11Module;   ///< Module to evaluate Et contribution

    double m_par_H_b_val;   ///< Parameter H_b_val
    double m_par_H_r_val;   ///< Parameter H_r_val
    double m_par_H_M_val;   ///< Parameter H_M_val

    double m_par_H_b_sea;   ///< Parameter H_b_sea
    double m_par_H_r_sea;   ///< Parameter H_r_sea
    double m_par_H_M_sea;   ///< Parameter H_M_sea

    double m_par_E_b_val;      ///< Parameter E_b_val
    double m_par_E_r_val;      ///< Parameter E_r_val
    double m_par_E_M_val;      ///< Parameter E_M_val

    double m_par_Ht_b_val;      ///< Parameter Ht_b_val
    double m_par_Ht_r_val;      ///< Parameter Ht_r_val
    double m_par_Ht_M_val;      ///< Parameter Ht_M_val
};

#endif /* KM10BORDERFUNCTIONMODEL_H_ */
