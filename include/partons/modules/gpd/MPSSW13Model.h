#ifndef MPSSW13_MODEL_H
#define MPSSW13_MODEL_H

/**
 * @file VGGModel.h
 * @author Herve Moutarde (CEA/Irfu, Saclay)
 * @date November 11, 2014
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../GPDModule.h"
#include "../MathIntegratorModule.h"

class MSTWPDF;
namespace NumA {
class FunctionType1D;
} /* namespace NumA */

/**
 * @class MPSSW13Model
 *
 * For the reference see hep-ph/1301.3819
 *
 * Module based on the original code (TBDP class) received from H. Mutarde as a private communication
 */
class MPSSW13Model: public GPDModule, public MathIntegratorModule {

public:

    static const unsigned int classId; ///< ID assigned by BaseObjectRegistry

    /** Constructor
     @param className Name of this class
     */
    MPSSW13Model(const std::string &className);

    virtual ~MPSSW13Model();
    virtual MPSSW13Model* clone() const;
    virtual void resolveObjectDependencies();
    //TODO no configure
    //TODO no toString
    void setParameters(std::vector<double> Parameters);

    unsigned int getNbOfQuarkFlavor() const; ///< Get #m_NbOfQuarkFlavor
    unsigned int getNbOfColor() const; ///< Get #m_NbOfColor
    double getMx() const; ///< Get #m_Mx
    double getCA() const; ///< Get #m_CA
    double getCF() const; ///< Get #m_CF
    double getTF() const; ///< Get #m_TF
    double getF1d() const; ///< Get #m_F1u
    double getF1u() const; ///< Get #m_F1d
    double getFD() const; ///< Get #m_FD
    double getProfileShapeVal() const; ///< Get #m_ProfileShapeVal
    double getProfileShapeSea() const; ///< Get #m_ProfileShapeSea
    double getProfileShapeGlue() const; ///< Get #m_ProfileShapeGlue
    double getQuarkDTerm() const; ///< Get #m_QuarkDTerm
    double getGluonDTerm() const; ///< Get #m_GluonDTerm
    void setGluonDTerm(double gluonDTerm); //TODO why?

protected:

    /** Copy constructor
     @param other Object to be copied
     */
    MPSSW13Model(const MPSSW13Model& other);

    //TODO private isModuleWellConfigured?
    //TODO private initModule?
    //TODO private computeH?

private:

    virtual void isModuleWellConfigured();
    virtual void initModule();
    virtual PartonDistribution computeH();

    unsigned int m_NbOfQuarkFlavor; ///< TO BE REMOVED? (is it defined here?)
    unsigned int m_NbOfColor; ///< Number of colors

    double m_Mx; ///< Negative value of x

    double m_CA; ///< Number of active quark flavors
    double m_CF; ///< Color factor
    double m_TF; ///< SU(3) color factor

    double m_F1u; ///< Contribution of quarks u to Pauli n and p form factors
    double m_F1d; ///< Contribution of quarks d to Pauli n and p form factors
    double m_FD; ///< Dipole form factor

    double m_ProfileShapeVal; ///< %Double distribution profile parameter for valence quarks
    double m_ProfileShapeSea; ///< %Double distribution profile parameter for sea quarks
    double m_ProfileShapeGlue; ///< %Double distribution profile parameter for gluons

    double m_QuarkDTerm; ///< Value of D term for quarks
    double m_GluonDTerm; ///< Value of D term for gluons

    MSTWPDF* m_Forward; ///< Pointer to MSTW PDFs

    /** Compute D terms
     */
    void ComputeDTerms();

    /** Compute form factors
     */
    void ComputeFormFactors();

    /** %Double distribution profile function
     @param N Profile parameter
     @param alpha, beta %Double distribution variables
     */
    double Profile(double N, double alpha, double beta);

    /** %Double distribution ansatz for quarks uVal and x
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralHuVal(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks uVal and -x
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralHuValMx(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks uSea and x and |x| > xi
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxLargeHuSea(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks uSea and -x and |x| > xi
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxLargeHuSeaMx(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks uSea and x and |x| < xi (part 1)
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxSmall1HuSea(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks uSea and x and |x| < xi  (part 2)
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxSmall2HuSea(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks dVal and x
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralHdVal(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks dVal and -x
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralHdValMx(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks dSea and x and |x| > xi
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxLargeHdSea(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks dSea and -x and |x| > xi
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxLargeHdSeaMx(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks dSea and x and |x| < xi (part 1)
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxSmall1HdSea(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks dSea and x and |x| < xi (part 2)
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxSmall2HdSea(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks s and x and |x| > xi
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxLargeHsSea(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks s and -x and |x| > xi
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxLargeHsSeaMx(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks s and x and |x| < xi (part 1)
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxSmall1HsSea(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks s and x and |x| < xi (part 2)
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxSmall2HsSea(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks g and x and |x| > xi
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxLargeHg(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks g and -x and |x| > xi
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxLargeHgMx(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks g and x and |x| < xi (part 1)
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxSmall1Hg(double x, std::vector<double> Par);

    /** %Double distribution ansatz for quarks g and x and |x| < xi (part 2)
     @param x %Double distribution variable
     @param Par Additional parameters (not used)
     */
    double IntegralxSmall2Hg(double x, std::vector<double> Par);

    /** Evaluation of anomalous dimension matrix for qq
     @param nflavour Number of active flavors
     @param n Gegenbauer order
     */
    double GammaQQ(const unsigned int nflavour, const unsigned int n);

    /** Evaluation of anomalous dimension matrix for qg
     @param nflavour Number of active flavors
     @param n Gegenbauer order
     */
    double GammaQG(const unsigned int nflavour, const unsigned int n);

    /** Evaluation of anomalous dimension matrix for gq
     @param nflavour Number of active flavors
     @param n Gegenbauer order
     */
    double GammaGQ(const unsigned int nflavour, const unsigned int n);

    /** Evaluation of anomalous dimension matrix for gg
     @param nflavour Number of active flavors
     @param n Gegenbauer order
     */
    double GammaGG(const unsigned int nflavour, const unsigned int n);

    /** Throw exception if beta < 0 or beta > 1
     @param funcName Name of function where the error appeared
     @param betaValue Value of beta
     */
    void throwBetaException(const std::string &funcName, double betaValue);

    NumA::FunctionType1D* m_pIntegralHuVal; ///< Functor related to IntegralHuVal()
    NumA::FunctionType1D* m_pIntegralHdVal; ///< Functor related to IntegralHdVal()
    NumA::FunctionType1D* m_pIntegralHuValMx; ///< Functor related to IntegralHuValMx()
    NumA::FunctionType1D* m_pIntegralHdValMx; ///< Functor related to IntegralHdValMx()
    NumA::FunctionType1D* m_pIntegralxLargeHuSea; ///< Functor related to IntegralxLargeHuSea()
    NumA::FunctionType1D* m_pIntegralxLargeHdSea; ///< Functor related to IntegralxLargeHdSea()
    NumA::FunctionType1D* m_pIntegralxSmall1HuSea; ///< Functor related to IntegralxSmall1HuSea()
    NumA::FunctionType1D* m_pIntegralxSmall2HuSea; ///< Functor related to IntegralxSmall2HuSea()
    NumA::FunctionType1D* m_pIntegralxSmall1HdSea; ///< Functor related to IntegralxSmall1HdSea()
    NumA::FunctionType1D* m_pIntegralxSmall2HdSea; ///< Functor related to IntegralxSmall2HdSea()
    NumA::FunctionType1D* m_pIntegralxLargeHuSeaMx; ///< Functor related to IntegralxLargeHuSeaMx()
    NumA::FunctionType1D* m_pIntegralxLargeHdSeaMx; ///< Functor related to IntegralxLargeHdSeaMx()
    NumA::FunctionType1D* m_pIntegralxLargeHsSea; ///< Functor related to IntegralxLargeHsSea()
    NumA::FunctionType1D* m_pIntegralxSmall1HsSea; ///< Functor related to IntegralxSmall1HsSea()
    NumA::FunctionType1D* m_pIntegralxSmall2HsSea; ///< Functor related to IntegralxSmall2HsSea()
    NumA::FunctionType1D* m_pIntegralxLargeHsSeaMx; ///< Functor related to IntegralxLargeHsSeaMx()
    NumA::FunctionType1D* m_pIntegralxLargeHg; ///< Functor related to IntegralxLargeHg()
    NumA::FunctionType1D* m_pIntegralxSmall1Hg; ///< Functor related to IntegralxSmall1Hg()
    NumA::FunctionType1D* m_pIntegralxSmall2Hg; ///< Functor related to IntegralxSmall2Hg()
    NumA::FunctionType1D* m_pIntegralxLargeHgMx; ///< Functor related to IntegralxLargeHgMx()

    /** Initialize functors
     */
    void initFunctorsForIntegrations();
};

#endif /* MPSSW13_H_ */
