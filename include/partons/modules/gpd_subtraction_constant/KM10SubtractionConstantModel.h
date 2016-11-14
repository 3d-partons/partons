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
 * Subtraction constant model used in Kumericki-Mueller fit
 * see Nucl. Phys. B841, 1 (2010)
 */
class KM10SubtractionConstantModel: public GPDSubtractionConstantModule {

public:

    static const unsigned int classId;    ///< ID assigned by BaseObjectRegistry

    /**
     * Default constructor
     * @param className Class name
     */
    KM10SubtractionConstantModel(const std::string &className);

    /**
     * Destructor
     */
    virtual ~KM10SubtractionConstantModel();

    virtual KM10SubtractionConstantModel* clone() const;

    /**
     * Get H_C parameter
     */
    double getParHC() const;

    /**
     * Set H_C parameter
     * @param parHC Value of H_C parameter to be set
     */
    void setParHC(double parHC);

    /**
     * Get H_M_sub parameter
    */
    double getParHMSub() const;

    /**
     * Set H_M_sub parameter
     * @param parHMSub Value of H_M_sub parameter to be set
     */
    void setParHMSub(double parHMSub);

protected:

    /** Copy constructor
     @param other Object to be copied
     */
    KM10SubtractionConstantModel(const KM10SubtractionConstantModel& other);

    virtual double computeH();
    virtual double computeE();

private:

    double m_par_H_C;   ///< H_C parameter
    double m_par_H_M_sub;   ///< H_M_sub parameter
};

#endif /* KM10SUBTRACTIONCONSTANTMODEL_H_ */
