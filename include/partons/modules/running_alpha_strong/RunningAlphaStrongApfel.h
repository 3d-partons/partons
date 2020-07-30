/**
 * @file RunningAlphaStrongApfel.h
 * @author Valerio Bertone (CEA Saclay)
 * @date June 2nd, 2020.
 * @version 1.0
 */

#ifndef RUNNING_ALPHA_STRONG_APFEL_H
#define RUNNING_ALPHA_STRONG_APFEL_H

#include "RunningAlphaStrongModule.h"
#include "../../beans/PerturbativeQCDOrderType.h"

#include <vector>

namespace PARTONS {

/*
 * \class RunningAlphaStrongApfel
 * 
 * \brief Evaluation of the strong running coupling constant in the
 * \f$\overline{MS}\f$ scheme through APFEL++.
 *
 * This module evaluates the strong running coupling constant in the
 * \f$\overline{MS}\f$ scheme. It takes the value of \f$Q_{\rm ref}\f$
 * and \f$\alpha(Q_{\rm ref})\f$, and quark masses as inputs and
 * calls APFEL++ to compute the evolution.
 *
 */
class RunningAlphaStrongApfel: public RunningAlphaStrongModule {

public:

    /**
     * Settable parameters
     */
    static const std::string PARAM_NAME_ALPHAS_REF;
    static const std::string PARAM_NAME_MU_REF;

    static const std::string PARAM_NAME_THRESHOLDS;
    static const std::string PARAM_NAME_MASSES;

    static const std::string PARAM_NAME_TAB_NODES;
    static const std::string PARAM_NAME_TAB_LOWER_BOUND;
    static const std::string PARAM_NAME_TAB_UPPER_BOUND;
    static const std::string PARAM_NAME_TAB_INTER_DEGREE;

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    RunningAlphaStrongApfel(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~RunningAlphaStrongApfel();
    virtual RunningAlphaStrongApfel* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double compute();

    // ##### GETTERS & SETTERS #####

    void setPertOrder(const PerturbativeQCDOrderType::Type& pertOrder);
    void setAlphasRef(const double& alphasRef);
    void setMuRef(const double& muRef);

    void setThresholds(const std::vector<double>& thresholds);
    void setMasses(const std::vector<double>& masses);

    void setTabNodes(const int& tabNodes);
    void setTabLowerBound(const double& tabLowerBound);
    void setTabUpperBound(const double& tabUpperBound);
    void setTabInterDegree(const int& tabInterDegree);

    PerturbativeQCDOrderType::Type getPertOrder() const;
    double getAlphasRef() const;
    double getMuRef() const;

    std::vector<double> getThresholds() const;
    std::vector<double> getMasses() const;

    int getTabNodes() const;
    double getTabLowerBound() const;
    double getTabUpperBound() const;
    int getTabInterDegree() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    RunningAlphaStrongApfel(const RunningAlphaStrongApfel &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    PerturbativeQCDOrderType::Type m_pertOrder;
    double m_alphasRef;
    double m_muRef;
    std::vector<double> m_thresholds;
    std::vector<double> m_masses;
    int m_tabNodes;
    double m_tabLowerBound;
    double m_tabUpperBound;
    int m_tabInterDegree;

    void setAlphasFunc(const std::function<double(double const&)>& alphasFunc);
    std::function<double(double const&)> getAlphasFunc() const;
    std::function<double(double const&)> m_alphasFunc;
};

} /* namespace PARTONS */

#endif /* RUNNING_ALPHA_STRONG_APFEL_H */

