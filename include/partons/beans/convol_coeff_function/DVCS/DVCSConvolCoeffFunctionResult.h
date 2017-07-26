#ifndef DVCS_CONVOL_COEFF_FUNCTION_RESULT_H
#define DVCS_CONVOL_COEFF_FUNCTION_RESULT_H

/**
 * @file DVCSConvolCoeffFunctionResult.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2015
 * @version 1.0
 */

#include <ElementaryUtils/string_utils/StringUtils.h>
#include <complex>
#include <map>
#include <string>
#include <vector>

#include "../../../utils/compare/ComparisonReport.h"
#include "../../gpd/GPDType.h"
#include "../../observable/ObservableChannel.h"
#include "../../Result.h"
#include "DVCSConvolCoeffFunctionKinematic.h"

namespace PARTONS {

class ComparisonReport;

/**
 * @class DVCSConvolCoeffFunctionResult
 *
 * @brief Class representing single result of DVCS CFF computation.
 *
 * This class is used to store results of a single DVCS Compton Form Factor (CFF) computation. In other words, the class stores a collection of complex numbers, with each of them representing a part of the result for a specific GPD type. This is illustrated by the following example:
 \code{.cpp}
 //evaluate exemplary CFF result

 //retrieve CFF service
 ConvolCoeffFunctionService* pDVCSConvolCoeffFunctionService = Partons::getInstance()->getServiceObjectRegistry()->getConvolCoeffFunctionService();

 //load GPD module with the BaseModuleFactory
 GPDModule* pGPDModel = Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MMS13Model::classId);

 //load CFF module with the BaseModuleFactory
 DVCSConvolCoeffFunctionModule* pDVCSCFFModule = Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(DVCSCFFModel::classId);

 //configure CFF module
 ElemUtils::Parameters parameters(PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, PerturbativeQCDOrderType::LO);
 pDVCSCFFModule->configure(parameters);

 pDVCSCFFModule->setGPDModule(pGPDModel);

 //define CFF kinematics used in computation
 DVCSConvolCoeffFunctionKinematic cffKinematic(0.05, -0.1, 4., 4., 4.);

 //evaluate
 DVCSConvolCoeffFunctionResult cffResult = pDVCSConvolCoeffFunctionService->computeForOneCCFModel(cffKinematic, pDVCSCFFModule);

 //retrieve value of CFF for GPD H:
 std::complex<double> CFFH = cffResult.getResult(GPDType::H);

 //retrieve the imaginary part of CFF for GPD E
 double CFFEimaginary = cffResult.getResult(GPDType::H).imag();
 \endcode
 */
class DVCSConvolCoeffFunctionResult: public Result {
public:

    /**
     * Default constructor.
     */
    DVCSConvolCoeffFunctionResult();

    /**
     * Assignment constructor.
     * @param kinematic DVCS CFF kinematics to be assigned.
     */
    DVCSConvolCoeffFunctionResult(DVCSConvolCoeffFunctionKinematic kinematic);

    /**
     * Destructor.
     */
    virtual ~DVCSConvolCoeffFunctionResult();

    /**
     * Add result for given GPD type.
     * @param gpdComputeType GPD type associated to given result.
     * @param result Result to be added.
     */
    void add(GPDType::Type gpdComputeType, std::complex<double> result);

    /**
     * Get result for given GPD type.
     * @param gpdComputeType GPD type associated to requested result.
     * @return Requested result.
     */
    std::complex<double> get(GPDType::Type gpdComputeType);

    /**
     * Check if the object stores result associated to given GPD type.
     * @param gpdType GPD type to be checked.
     * @return True if the object stores result associated to given GPD type, otherwise false.
     * @see DVCSConvolCoeffFunctionResult::getLastAvailable()
     */
    bool isAvailable(GPDType::Type gpdType);

    /**
     * Get reference to result marked by the last call of DVCSConvolCoeffFunctionResult::isAvailible() function.
     * @return Reference to selected result.
     * @see DVCSConvolCoeffFunctionResult::isAvailible()
     */
    std::complex<double> getLastAvailable() const;

    /**
     * Get reference to result for given GPD type.
     * @param gpdType GPD type associated to requested result.
     * @return Reference to requested result.
     */
    const std::complex<double>& getResult(GPDType::Type gpdType) const;

    /**
     * Compare to other DVCSConvolCoeffFunctionResult object and store comparison result in given comparison report.
     * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
     * @param referenceObject Reference to object to be compared.
     * @param parentObjectInfo Addition information coming from the parent object (if needed).
     */
    void compare(ComparisonReport &rootComparisonReport,
            const DVCSConvolCoeffFunctionResult &referenceObject,
            std::string parentObjectInfo = ElemUtils::StringUtils::EMPTY) const;

    virtual std::string toString() const;

    /**
     * Relation operator that checks if the value of left operand is less than the value of right operand (in this case returned is this->m_kinematic < other.m_kinematic).
     * Used by std::sort function.
     * @param other Right hand value.
     * @return True if the value of left operand is less than the value of right operand, otherwise false.
     */
    bool operator <(const DVCSConvolCoeffFunctionResult &other) const;

    /**
     * Get list of GPD types associated to stored parton distributions.
     * @return Vector of associated types.
     */
    std::vector<GPDType> listGPDTypeComputed();

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type of channel associated to this class.
     */
    const ObservableChannel& getChannel() const;

    /**
     * Set type of channel associated to this class.
     */
    void setChannel(const ObservableChannel& channel);

    /**
     * Get reference to DVCS CFF kinematics associated to this result.
     */
    const DVCSConvolCoeffFunctionKinematic& getKinematic() const;

    /**
     * Set DVCS CFF kinematics associated to this result.
     */
    void setKinematic(const DVCSConvolCoeffFunctionKinematic& kinematic);

    /**
     * Get reference to map containing stored results distinguished by associated GPD types.
     */
    const std::map<GPDType::Type, std::complex<double> >& getResultsByGpdType() const;

    /**
     * Set map containing stored results distinguished by associated GPD types.
     */
    void setResultsByGpdType(
            const std::map<GPDType::Type, std::complex<double> >& resultsByGpdType);

private:

    /**
     * Type of channel associated to this class.
     */
    ObservableChannel m_channel;

    /**
     * DVCS CFF kinematics associated to this result.
     */
    DVCSConvolCoeffFunctionKinematic m_kinematic;

    /**
     * Map containing stored results distinguished by associated GPD types.
     */
    std::map<GPDType::Type, std::complex<double> > m_resultsByGPDType;

    /**
     * Iterator used to mark a specific entry in DVCSConvolCoeffFunctionResult::m_resultsByGPDType.
     */
    std::map<GPDType::Type, std::complex<double> >::const_iterator m_it;
};

} /* namespace PARTONS */

#endif /* DVCS_CONVOL_COEFF_FUNCTION_RESULT_H */
