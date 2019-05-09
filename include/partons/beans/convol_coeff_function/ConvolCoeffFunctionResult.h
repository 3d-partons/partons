#ifndef CONVOL_COEFF_FUNCTION_RESULT_H
#define CONVOL_COEFF_FUNCTION_RESULT_H

/**
 * @file ConvolCoeffFunctionResult.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2015
 * @version 1.0
 */

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <complex>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "../../utils/compare/CompareUtils.h"
#include "../../utils/compare/ComparisonData.h"
#include "../../utils/compare/ComparisonReport.h"
#include "../channel/ChannelType.h"
#include "../gpd/GPDType.h"
#include "../Result.h"

namespace PARTONS {

/**
 * @class ConvolCoeffFunctionResult
 *
 * @brief Abstract class representing a single result of CCF computation.
 *
 * This abstract class is used to store results of a single CCF computation.
 */
template<typename KinematicType>
class ConvolCoeffFunctionResult: public Result<KinematicType> {

public:

    /**
     * Destructor.
     */
    virtual ~ConvolCoeffFunctionResult() {
    }

    virtual std::string toString() const {

        ElemUtils::Formatter formatter;

        formatter << "\n" << Result<KinematicType>::toString() << "\n\n";
        formatter << "Result: " << '\n';

        std::map<GPDType::Type, std::complex<double> >::const_iterator it;

        for (it = m_resultsByGPDType.begin(); it != m_resultsByGPDType.end();
                it++) {
            formatter << "CFF " << GPDType(it->first).toString() << ": Re: "
                    << (it->second).real() << " Im: " << (it->second).imag()
                    << '\n';
        }

        return formatter.str();
    }

    /**
     * Add result for given GPD type.
     * @param gpdComputeType GPD type associated to given result.
     * @param result Result to be added.
     */
    void addResult(GPDType::Type gpdType, const std::complex<double>& result) {

        //get iterator
        std::map<GPDType::Type, std::complex<double> >::const_iterator it =
                m_resultsByGPDType.find(gpdType);

        //throw exception
        if (it != m_resultsByGPDType.end()) {
            throw ElemUtils::CustomException(this->getClassName(), __func__,
                    ElemUtils::Formatter() << "Result for GPDType = "
                            << GPDType(gpdType).toString() << " exists");
        }

        //insert
        m_resultsByGPDType.insert(std::make_pair(gpdType, result));
    }

    /**
     * Get reference to result for given GPD type.
     * @param gpdType GPD type associated to requested result.
     * @return Reference to requested result.
     */
    const std::complex<double>& getResult(GPDType::Type gpdType) const {

        //get iterator
        std::map<GPDType::Type, std::complex<double> >::const_iterator it =
                m_resultsByGPDType.find(gpdType);

        //throw exception
        if (it == m_resultsByGPDType.end()) {
            throw ElemUtils::CustomException(this->getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Cannot find result for GPDType = "
                            << GPDType(gpdType).toString());
        }

        //return
        return (it->second);
    }

    /**
     * Check if the object stores result associated to given GPD type.
     * @param gpdType GPD type to be checked.
     * @return True if the object stores result associated to given GPD type, otherwise false.
     * @see ConvolCoeffFunctionResult::getLastAvailable()
     */
    bool isAvailable(GPDType::Type gpdType) {

        //get iterator
        m_it = m_resultsByGPDType.find(gpdType);

        //return
        return (m_it != m_resultsByGPDType.end());
    }

    /**
     * Get reference to result marked by the last call of ConvolCoeffFunctionResult::isAvailible() function.
     * @return Reference to selected result.
     * @see ConvolCoeffFunctionResult::isAvailible()
     */
    std::complex<double> getLastAvailable() const {
        return m_it->second;
    }

    /**
     * Compare to other ConvolCoeffFunctionResult object and store comparison result in given comparison report.
     * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
     * @param referenceObject Reference to object to be compared.
     * @param parentObjectInfo Addition information coming from the parent object (if needed).
     */
    void compare(ComparisonReport &rootComparisonReport,
            const ConvolCoeffFunctionResult &referenceObject,
            std::string parentObjectInfo = ElemUtils::StringUtils::EMPTY) const {

        //TODO faire un test pour valider la cinématique associée

        //TODO tester la taille des listes avant de faire le test
        for (std::map<GPDType::Type, std::complex<double> >::const_iterator it =
                m_resultsByGPDType.begin(); it != m_resultsByGPDType.end();
                it++) {

            ComparisonData comparisonData = CompareUtils::compareComplex("",
                    it->second, referenceObject.getResult(it->first),
                    rootComparisonReport.getTolerances(),
                    ElemUtils::Formatter() << parentObjectInfo << " "
                            << GPDType(it->first).toString());
            rootComparisonReport.addComparisonData(comparisonData);
        }
    }

    /**
     * Get list of GPD types associated to stored parton distributions.
     * @return Vector of associated types.
     */
    std::vector<GPDType> listGPDTypeComputed() const {

        //result
        std::vector<GPDType> list;

        //iterator
        std::map<GPDType::Type, std::complex<double> >::const_iterator it;

        //fill vector
        if (m_resultsByGPDType.size() != 0) {

            for (it = m_resultsByGPDType.begin();
                    it != m_resultsByGPDType.end(); ++it) {
                list.push_back(it->first);
            }
        }

        //return
        return list;
    }

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get reference to map containing stored results distinguished by associated GPD types.
     */
    const std::map<GPDType::Type, std::complex<double> >& getResultsByGpdType() const {
        return m_resultsByGPDType;
    }

    /**
     * Set map containing stored results distinguished by associated GPD types.
     */
    void setResultsByGpdType(
            const std::map<GPDType::Type, std::complex<double> >& resultsByGpdType) {
        m_resultsByGPDType = resultsByGpdType;
    }

protected:

    /**
     * Default constructor.
     */
    ConvolCoeffFunctionResult(const std::string &className,
            ChannelType::Type channelType) :
            Result<KinematicType>(className, channelType) {
    }

    /**
     * Assignment constructor.
     * @param kinematic CCF kinematics to be assigned.
     */
    ConvolCoeffFunctionResult(const std::string &className,
            ChannelType::Type channelType, const KinematicType& kinematic) :
            Result<KinematicType>(className, channelType, kinematic) {
    }

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ConvolCoeffFunctionResult(const ConvolCoeffFunctionResult& other) :
            Result<KinematicType>(other) {
        m_resultsByGPDType = other.m_resultsByGPDType;
    }

    /**
     * Map containing stored results distinguished by associated GPD types.
     */
    std::map<GPDType::Type, std::complex<double> > m_resultsByGPDType;

    /**
     * Iterator used to mark a specific entry in ConvolCoeffFunctionResult::m_resultsByGPDType.
     */
    std::map<GPDType::Type, std::complex<double> >::const_iterator m_it;
};

} /* namespace PARTONS */

#endif /* CONVOL_COEFF_FUNCTION_RESULT_H */
