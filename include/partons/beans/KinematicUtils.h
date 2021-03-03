#ifndef KINEMATIC_UTILS_H
#define KINEMATIC_UTILS_H

/**
 * @file KinematicUtils.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 27, 2015
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../utils/type/PhysicalUnit.h"
#include "convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "convol_coeff_function/TCS/TCSConvolCoeffFunctionKinematic.h"
#include "gpd/GPDKinematic.h"
#include "collinear_distribution/CollinearDistributionKinematic.h"
#include "List.h"
#include "observable/DVCS/DVCSObservableKinematic.h"
#include "observable/TCS/TCSObservableKinematic.h"

namespace PARTONS {

/**
 * @class KinematicUtils
 *
 * @brief Set of utilization tools to handle kinematic-like classes.
 *
 * This class is used as a container for utilization tools to handle kinematic-like classes.
 * It provides in particular tools to parse text files in order to create a set of kinematic-like objects.
 */
class KinematicUtils: public BaseObject {

public:

    /**
     * Default constructor.
     */
    KinematicUtils();

    /**
     * Destructor.
     */
    virtual ~KinematicUtils();

    /**
     * Parse a text file in order to retrieve a list of GPDKinematic objects.
     * The parsed file should display separate lines of the form "x | xi | t | MuF2 | MuR2".
     * @param filePath Path to file to be parsed.
     * @return List of extracted GPDKinematic objects.
     */
    List<GPDKinematic> getGPDKinematicFromFile(const std::string &filePath);

    /**
     * Parse a text file in order to retrieve a list of CollienearDistributionKinematic objects.
     * The parsed file should display separate lines of the form "x | MuF2 | MuR2".
     * @param filePath Path to file to be parsed.
     * @return List of extracted GPDKinematic objects.
     */
    List<CollinearDistributionKinematic> getCollinearDistributionKinematicFromFile(const std::string &filePath);

    /**
     * Parse a text file in order to retrieve a list of DVCSConvolCoeffFunctionKinematic objects.
     * The parsed file should display separate lines of the form "xi | t | Q2 | MuF2 | MuR2".
     * @param filePath Path to file to be parsed.
     * @return List of extracted DVCSConvolCoeffFunctionKinematic objects.
     */
    List<DVCSConvolCoeffFunctionKinematic> getDVCSCCFKinematicFromFile(
            const std::string &filePath);

    /**
     * Parse a text file in order to retrieve a list of TCSConvolCoeffFunctionKinematic objects.
     * The parsed file should display separate lines of the form "xi | t | Q2' | MuF2 | MuR2".
     * @param filePath Path to file to be parsed.
     * @return List of extracted TCSConvolCoeffFunctionKinematic objects.
     */
    List<TCSConvolCoeffFunctionKinematic> getTCSCCFKinematicFromFile(
            const std::string &filePath);

    /**
     * Parse a text file in order to retrieve a list of ObservableKinematic objects.
     * The parsed file should display separate lines of the form "xB | t | Q2 | E | phi".
     * @param filePath Path to file to be parsed.
     * @return List of extracted ObservableKinematic objects.
     */
    List<DVCSObservableKinematic> getDVCSObservableKinematicFromFile(
            const std::string &filePath);

    /**
     * Parse a text file in order to retrieve a list of ObservableKinematic objects.
     * The parsed file should display separate lines of the form "t | Q2' | E | phi | theta | MLepton".
     * @param filePath Path to file to be parsed.
     * @return List of extracted ObservableKinematic objects.
     */
    List<TCSObservableKinematic> getTCSObservableKinematicFromFile(
            const std::string &filePath);

private:

    /**
     * Throw a custom error.
     * @param funcName Name of function where the error has appeared.
     * @param msg User readable message to be thrown.
     */
    void error(const std::string &funcName, const std::string &msg);

    /**
     * Throw an error when unable to open a file.
     * @param funcName Name of function where the error has appeared.
     * @param msg User readable message to be thrown.
     */
    void errorCannotOpenFile(const std::string &funcName,
            const std::string &msg);

    /**
     * Check if content of a file is empty. If yes, throw an error.
     * @param funcName Name of function where the check is performed.
     * @param kinematicString Content of file.
     * @param filePath Path to file.
     */
    void checkEmptyInputFile(const std::string &funcName,
            const std::vector<std::string> &kinematicString,
            const std::string &filePath);

    /**
     * Return vector containing units according to a give string.
     * @param funcName Name of function to throw error if needed.
     * @param line Input line.
     * @return Vector containing returned units.
     */
    std::vector<PhysicalUnit> getUnitsFromInputFileLine(
            const std::string &funcName, const std::string &line) const;
};

} /* namespace PARTONS */

#endif /* KINEMATICUTILS_H_ */
