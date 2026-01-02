#ifndef JET_CFFTABLES_H
#define JET_CFFTABLES_H

/**
 * @file JETCFFTables.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date 06 April 2021
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <complex>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <tuple>

#include "JETConvolCoeffFunctionModule.h"
#include "../../../../algorithm/linterp/linterp.h"

namespace PARTONS {

/**
 * @class JETCFFTables
 *
 * @brief Module for evaluation of CFF values from lookup tables.
 *
 * Module for evaluation of CFF values from lookup tables (xi, t, z, qPerp2, Q2, jetType).
 *
 * Since the tables are for z, qPerp2, Q2 and jetType it is explicitly assumed that muF2 = muR2 = m^2 + qPerp2^2 + z \bar{z} Q^2
 *
 * User may change between available sets of tables with JETCFFTables::setCFFSetFile(),
 * or JETCFFTables::configure() using JETCFFTables::PARAMETER_NAME_SET_FILE tag.
 * E.g. JETCFFTables::setCFFSetFile("data/JETCFFTables/tables_GK.dat") will probe tables
 * stored in "data/JETCFFTables/tables_GK.dat" file.
 *
 * By default "data/JETCFFTables/tables_GK.dat" tables are probed.
 *
 * The interpolation over lookup table is done with linterp library (https://rncarpio.github.io/linterp).
 */
class JETCFFTables: public JETConvolCoeffFunctionModule {

public:

    ///< Key.
    using Key = std::tuple<GPDType::Type, JetType::Type, JetCFFType::Type>;

    ///< Hash key.
    struct KeyHash {
        std::size_t operator()(const Key& key) const{

            size_t a = static_cast<size_t>(std::get<0>(key));
            size_t b = static_cast<size_t>(std::get<1>(key));
            size_t c = static_cast<size_t>(std::get<2>(key));

            if (b >= 10 || c >= 10) {
                throw ElemUtils::CustomException("KeyHash", __func__, "Illegal value of hashed quantities");
            }

            return c + 10 * b + 100 * a;
        }
    };

    static const std::string PARAMETER_NAME_SET_FILE; ///< String used to switch between available CFF sets.

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    JETCFFTables(const std::string &className);

    virtual JETCFFTables* clone() const;

    /**
     * Default destructor.
     */
    virtual ~JETCFFTables();

    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual std::complex<double> computeCFF();

    /**
     * Get CFF set file.
     */
    const std::string& getCFFSetFile() const;

    /**
     * Set CFF set file.
     */
    void setCFFSetFile(const std::string& cffSetFile);

protected:

    /**
     * Copy constructor.
     @param other Object to be copied.
     */
    JETCFFTables(const JETCFFTables &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    /**
     * Load grids from default location and given set.
     */
    void loadGrids();

    /**
     * Load single grid.
     */
    std::pair<std::shared_ptr<NDInterpolator_5_ML>,
            std::shared_ptr<NDInterpolator_5_ML> > loadGrid(
            const std::string& type);

    /**
     * Read grid.
     */
    std::vector<std::vector<double> > readGrid(const std::string& type) const;

    /**
     * Check and set range.
     */
    void checkAndSetRange(std::pair<double, double>& target,
            const std::pair<double, double>& source);

    /**
     * Check range.
     */
    bool checkRange(double& value, const std::pair<double, double>& range,
            const std::pair<bool, bool>& freeze);

    /**
     * Find index.
     */
    int findIndex(const std::vector<double>& vec, double val) const;

    /**
     * Check if value already stored in vector.
     */
    bool checkIfUnique(const std::vector<double>& vec, double val) const;

    std::string m_cffSetFile; ///< Current set file.

    bool m_tablesLoaded; ///< True is tables loaded.

    std::vector<std::pair<double, double> > m_ranges; ///< Kinematic ranges

    std::unordered_map<Key, std::pair<std::shared_ptr<NDInterpolator_5_ML>, std::shared_ptr<NDInterpolator_5_ML> >, KeyHash> m_map; ///< Map containing pointers to interpolation grids.

    bool m_printOutsideGridWarnings; ///< Print more warnings.
};

} /* namespace PARTONS */

#endif /* JET_CFFTABLES_H */
