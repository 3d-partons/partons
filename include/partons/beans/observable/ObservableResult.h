#ifndef OBSERVABLE_RESULT_H
#define OBSERVABLE_RESULT_H

/**
 * @file ObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 21, 2015
 * @version 1.0
 */

#include <ElementaryUtils/string_utils/StringUtils.h>
#include <string>

#include "../../utils/compare/ComparisonReport.h"
#include "../../utils/math/ErrorBar.h"
#include "../Result.h"
#include "ObservableKinematic.h"
#include "ObservableType.h"

namespace PARTONS {

class ComparisonReport;

/**
 * @class ObservableResult
 *
 * @brief Class representing single result of DVCS observable computation.
 *
 * This class is used to store a result of a single DVCS observable computation. This is illustrated by the following example:
 \code{.cpp}
 //evaluate exemplary observable result

 //retrieve observable service
 ObservableService* pObservableService = Partons::getInstance()->getServiceObjectRegistry()->getObservableService();

 //load GPD module with the BaseModuleFactory
 GPDModule* pGPDModel = Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MMS13Model::classId);

 //load CFF module with the BaseModuleFactory
 DVCSConvolCoeffFunctionModule* pDVCSCFFModule = Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(DVCSCFFModel::classId);

 //configure CFF module
 ElemUtils::Parameters parameters(PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, PerturbativeQCDOrderType::LO);
 pDVCSCFFModule->configure(parameters);

 pDVCSCFFModule->setGPDModule(pGPDModel);

 //load process module with BaseModuleFactory
 DVCSModule* pDVCSModule = Partons::getInstance()->getModuleObjectFactory()->newProcessModule(GV2008Model::classId);

 //configure process module
 pDVCSModule->setConvolCoeffFunctionModule(pDVCSCFFModule);

 //load observable module with BaseModuleFactory
 Observable* pObservable = Partons::getInstance()->getModuleObjectFactory()->newObservable(CrossSectionObservable::classId);

 //onfigure observable module
 pObservable->setProcessModule(pDVCSModule);

 //define observable kinematics used in computation
 ObservableKinematic observableKinematic(0.17, -0.13, 1.36, 5.77, 10.);

 //evaluate
 ObservableResult observableResult = pObservableService->computeObservable(observableKinematic, pObservable);

 //get value
 double result = observableResult.getValue();
 \endcode
 */
class ObservableResult: public Result {
public:

    /**
     * Parameter name to set observable value via configuration methods.
     */
    static const std::string PARAMETER_NAME_OBSERVABLE_VALUE;

    /**
     * Parameter name to set total uncertainty via configuration methods.
     */
    static const std::string PARAMETER_NAME_TOTAL_ERROR;

    /**
     * Default constructor.
     */
    ObservableResult();

    /**
     * Assignment constructor.
     * @param observableName Name of observable.
     * @param value Value.
     */
    ObservableResult(const std::string &observableName, double value);

    /**
     * Destructor.
     */
    virtual ~ObservableResult();

    virtual std::string toString() const;

    /**
     * Get string containing information on stored data.
     * @return String with returned information.
     */
    virtual std::string getObjectInfo() const;

    /**
     * Compare to other DVCSConvolCoeffFunctionResult object and store comparison result in given comparison report.
     * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
     * @param referenceObject Reference to object to be compared.
     * @param parentObjectInfo Addition information coming from the parent object (if needed).
     */
    void compare(ComparisonReport &rootComparisonReport,
            const ObservableResult &referenceObject,
            std::string parentObjectInfo = ElemUtils::StringUtils::EMPTY) const;

    /**
     * Relation operator that checks if the value of left operand is less than the value of right operand (in this case returned is this->m_kinematic < other.m_kinematic).
     * Used by std::sort function.
     * @param other Right hand value.
     * @return True if the value of left operand is less than the value of right operand, otherwise false.
     */
    bool operator <(const ObservableResult &other) const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get name of observable associated to this result.
     */
    const std::string& getObservableName() const;

    /**
     * Get value of result.
     */
    double getValue() const;

    /**
     * Set value of result.
     */
    void setValue(double value);

    /**
     * Get total uncertainty associated to this result.
     */
    double getTotalError() const;

    /**
     * Set total uncertainty associated to this result.
     */
    void setTotalError(double totalError);

    /**
     * Get reference to statistical uncertainty associated to this result.
     */
    const ErrorBar& getStatError() const;

    /**
     * Set statistical uncertainty associated to this result.
     */
    void setStatError(const ErrorBar& statError);

    /**
     * Get reference to systematic uncertainty associated to this result.
     */
    const ErrorBar& getSystError() const;

    /**
     * Set systematic uncertainty associated to this result.
     */
    void setSystError(const ErrorBar& systError);

    /**
     * Get reference to DVCS observable kinematics associated to this result.
     */
    const ObservableKinematic& getKinematic() const;

    /**
     * Set DVCS observable kinematics associated to this result.
     */
    void setKinematic(const ObservableKinematic &kinematic);

    /**
     * Get type of observable associated to this result.
     */
    ObservableType::Type getObservableType() const;

    /**
     * Set type of observable associated to this result.
     */
    void setObservableType(ObservableType::Type observableType);

private:

    /**
     * Name of observable associated to this result.
     */
    std::string m_observableName;

    /**
     * Value of result.
     */
    double m_value;

    /**
     * Total uncertainty associated to this result.
     */
    double m_totalError;

    /**
     * Statistical uncertainty associated to this result.
     */
    ErrorBar m_statError;

    /**
     * Systematic uncertainty associated to this result.
     */
    ErrorBar m_systError;

    //TODO add a proxy to retrieve it from database.
    /**
     * DVCS observable kinematics associated to this result.
     */
    ObservableKinematic m_kinematic;

    /**
     * Type of observable associated to this result.
     */
    ObservableType::Type m_observableType;
};

} /* namespace PARTONS */

#endif /* OBSERVABLE_RESULT_H */
