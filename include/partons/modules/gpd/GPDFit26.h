#ifndef GPD_FIT26_H
#define GPD_FIT26_H

/**
 * @file GPDFit26.h
 * @author Pawel Sznajder (NCBJ)
 * @date January 29, 2026
 * @version 1.0
 */

#include "GPDModule.h"

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>
#include <LHAPDF/LHAPDF.h>

namespace PARTONS {

/**
 * @class GPDFit26
 *
 * Fit to data, 2006.
 */
class GPDFit26: public GPDModule {

public:

    static const std::string PARAMETER_NAME_FIT26MODEL_UNPOLPDFSET; ///< Name of parameter to set unpolarized PDF set.
    static const std::string PARAMETER_NAME_FIT26MODEL_UNPOLPDFREPLICA; ///< Name of parameter to set unpolarized PDF set replica.
    static const std::string PARAMETER_NAME_FIT26MODEL_POLPDFSET; ///< Name of parameter to set polarized PDF.
    static const std::string PARAMETER_NAME_FIT26MODEL_POLPDFREPLICA; ///< Name of parameter to set polarized PDF replica.

    static const unsigned int classId;    ///< ID assigned by BaseObjectRegistry

    /**
     * Partons.
     */
    enum Parton {
        g = 0,
        uVal = 1, uSea = 2,
        dVal = 3, dSea = 4,
        s = 5
    };

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GPDFit26(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~GPDFit26();
    virtual GPDFit26* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

    /**
     * Set parameters.
     */
    void setParameters(const std::vector<double>& params = std::vector<double>());

protected:

    /** Copy constructor.
     @param other Object to be copied.
     */
    GPDFit26(const GPDFit26& other);

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();
    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeEt();

private:

    std::string m_unpolPDFSetName; ///< Name of PDF set (unpolarized).
    int m_unpolPDFReplica; ///< Member of PDF set (unpolarized).
    LHAPDF::PDF* m_pUnpolPDF; ///< Pointer to PDF set (unpolarized).

    std::string m_polPDFSetName; ///< Name of PDF set (polarized).
    int m_polPDFReplica; ///< Member of PDF set (polarized).
    LHAPDF::PDF* m_pPolPDF;  ///< Pointer to PDF set (polarized).

    double m_par_H_uval_AmC_E_uval_A;
    double m_par_H_uval_B;
    double m_par_H_dval_AmC_E_dval_A;
    double m_par_H_dval_B;

    double m_par_E_uvaldval_alpha;
    double m_par_E_uval_B;
    double m_par_E_uval_C;
    double m_par_E_dval_B;
    double m_par_E_dval_C;

    /**
     * Load PDFs.
     */
    void loadPDFs();
};

} /* namespace PARTONS */

#endif /* GPD_FIT26_H */
