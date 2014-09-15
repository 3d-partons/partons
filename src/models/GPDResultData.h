#ifndef GPD_RESULT_DATA_H
#define GPD_RESULT_DATA_H

/**
 * @file GPDResultData.h
 * @brief
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 24/07/2014
 * @version 1.0
 */

#include <map>
#include <string>
#include <vector>

#include "GenericData.h"
#include "GPDQuarkFlavorData.h"
#include "QuarkFlavor.h"

struct GPDComputeType;

class GPDResultData: public GenericData {
    std::map<QuarkFlavor::Type, GPDQuarkFlavorData*> gpdQuarkFlavorData;

    GPDComputeType* pGPDComputeType;
    // double squareChargeAveraged;
    double gluon;
    double singlet;

public:

    GPDResultData(GPDComputeType* _pGPDComputeType);
    ~GPDResultData();

    void addGPDQuarkFlavorData(GPDQuarkFlavorData* _pGPDQuarkFlavorData);
    GPDQuarkFlavorData* getGPDQuarkFlavorData(
            QuarkFlavor::Type quarkFlavorType);

    std::vector<QuarkFlavor> listQuarkTypeComputed();

    std::string toString();

    GPDComputeType* getGpdComputeType() const;
    void setGpdComputeType(GPDComputeType* gpdComputeType);
    double getHg() const;
    void setGluon(double gluon);
    double getSinglet() const;
    void setSinglet(double singlet);
    double getGluon() const;
//    double getSquareChargeAveraged() const;
//    void setSquareChargeAveraged(double squareChargeAveraged);
};

#endif /* GPD_RESULT_DATA_H */
