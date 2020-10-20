#include "../../../../../include/partons/modules/evolution/collinear_distribution/CollinearDistributionEvolutionApfel.h"

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongModule.h"

namespace PARTONS {

const std::string CollinearDistributionEvolutionApfel::PARAM_NAME_THRESHOLDS =
        "thresholds";
const std::string CollinearDistributionEvolutionApfel::PARAM_NAME_MASSES =
        "masses";

const std::string CollinearDistributionEvolutionApfel::PARAM_NAME_SUBGRID_NODES =
        "subgridNodes";
const std::string CollinearDistributionEvolutionApfel::PARAM_NAME_SUBGRID_LOWER_BOUNDS =
        "subgridLowerBounds";
const std::string CollinearDistributionEvolutionApfel::PARAM_NAME_SUBGRID_INTER_DEGREES =
        "subgridInterDegrees";

const std::string CollinearDistributionEvolutionApfel::PARAM_NAME_TAB_NODES =
        "tabNodes";
const std::string CollinearDistributionEvolutionApfel::PARAM_NAME_TAB_LOWER_BOUND =
        "tabLowerBound";
const std::string CollinearDistributionEvolutionApfel::PARAM_NAME_TAB_UPPER_BOUND =
        "tabUpperBound";
const std::string CollinearDistributionEvolutionApfel::PARAM_NAME_TAB_INTER_DEGREE =
        "tabInterDegree";

const unsigned int CollinearDistributionEvolutionApfel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new CollinearDistributionEvolutionApfel(
                        "CollinearDistributionEvolutionApfel"));

CollinearDistributionEvolutionApfel::CollinearDistributionEvolutionApfel(
        const std::string &className) :
        CollinearDistributionEvolutionModule(className), m_thresholds( { }), m_masses(
                { }), m_subgridNodes( { }), m_subgridLowerBounds( { }), m_subgridInterDegrees(
                { }), m_tabNodes(0), m_tabLowerBound(0), m_tabUpperBound(0), m_tabInterDegree(
                0) {
}

CollinearDistributionEvolutionApfel::~CollinearDistributionEvolutionApfel() {
}

CollinearDistributionEvolutionApfel* CollinearDistributionEvolutionApfel::clone() const {
    return new CollinearDistributionEvolutionApfel(*this);
}

std::string CollinearDistributionEvolutionApfel::toString() const {
    return CollinearDistributionEvolutionModule::toString();
}

void CollinearDistributionEvolutionApfel::resolveObjectDependencies() {
    CollinearDistributionEvolutionModule::resolveObjectDependencies();
}

void CollinearDistributionEvolutionApfel::configure(
        const ElemUtils::Parameters &parameters) {
    CollinearDistributionEvolutionModule::configure(parameters);

    if (parameters.isAvailable(
            CollinearDistributionEvolutionApfel::PARAM_NAME_THRESHOLDS)) {
        setThresholds(parameters.getLastAvailable().toVectorDouble());
        std::ostringstream oss;
        std::copy(m_thresholds.begin(), m_thresholds.end(),
                std::ostream_iterator<double>(oss, " "));
        info(__func__,
                ElemUtils::Formatter()
                        << CollinearDistributionEvolutionApfel::PARAM_NAME_THRESHOLDS
                        << " configured with value = [ " << oss.str()
                        << "] GeV");
    }

    if (parameters.isAvailable(
            CollinearDistributionEvolutionApfel::PARAM_NAME_MASSES)) {
        setMasses(parameters.getLastAvailable().toVectorDouble());
        std::ostringstream oss;
        std::copy(m_masses.begin(), m_masses.end(),
                std::ostream_iterator<double>(oss, " "));
        info(__func__,
                ElemUtils::Formatter()
                        << CollinearDistributionEvolutionApfel::PARAM_NAME_MASSES
                        << " configured with value = [ " << oss.str()
                        << "] GeV");
    }

    if (parameters.isAvailable(
            CollinearDistributionEvolutionApfel::PARAM_NAME_SUBGRID_NODES)) {
        setSubgridNodes(parameters.getLastAvailable().toVectorInt());
        std::ostringstream oss;
        std::copy(m_subgridNodes.begin(), m_subgridNodes.end(),
                std::ostream_iterator<int>(oss, " "));
        info(__func__,
                ElemUtils::Formatter()
                        << CollinearDistributionEvolutionApfel::PARAM_NAME_SUBGRID_NODES
                        << " configured with value = [ " << oss.str() << "]");
    }

    if (parameters.isAvailable(
            CollinearDistributionEvolutionApfel::PARAM_NAME_SUBGRID_LOWER_BOUNDS)) {
        setSubgridLowerBounds(parameters.getLastAvailable().toVectorDouble());
        std::ostringstream oss;
        std::copy(m_subgridLowerBounds.begin(), m_subgridLowerBounds.end(),
                std::ostream_iterator<double>(oss, " "));
        info(__func__,
                ElemUtils::Formatter()
                        << CollinearDistributionEvolutionApfel::PARAM_NAME_SUBGRID_LOWER_BOUNDS
                        << " configured with value = [ " << oss.str() << "]");
    }

    if (parameters.isAvailable(
            CollinearDistributionEvolutionApfel::PARAM_NAME_SUBGRID_INTER_DEGREES)) {
        setSubgridInterDegrees(parameters.getLastAvailable().toVectorInt());
        std::ostringstream oss;
        std::copy(m_subgridInterDegrees.begin(), m_subgridInterDegrees.end(),
                std::ostream_iterator<int>(oss, " "));
        info(__func__,
                ElemUtils::Formatter()
                        << CollinearDistributionEvolutionApfel::PARAM_NAME_SUBGRID_INTER_DEGREES
                        << " configured with value = [ " << oss.str() << "]");
    }

    if (parameters.isAvailable(
            CollinearDistributionEvolutionApfel::PARAM_NAME_TAB_NODES)) {
        setTabNodes(parameters.getLastAvailable().toInt());
        info(__func__,
                ElemUtils::Formatter()
                        << CollinearDistributionEvolutionApfel::PARAM_NAME_TAB_NODES
                        << " configured with value = " << getTabNodes());
    }

    if (parameters.isAvailable(
            CollinearDistributionEvolutionApfel::PARAM_NAME_TAB_LOWER_BOUND)) {
        setTabLowerBound(parameters.getLastAvailable().toDouble());
        info(__func__,
                ElemUtils::Formatter()
                        << CollinearDistributionEvolutionApfel::PARAM_NAME_TAB_LOWER_BOUND
                        << " configured with value = " << getTabLowerBound());
    }

    if (parameters.isAvailable(
            CollinearDistributionEvolutionApfel::PARAM_NAME_TAB_UPPER_BOUND)) {
        setTabUpperBound(parameters.getLastAvailable().toDouble());
        info(__func__,
                ElemUtils::Formatter()
                        << CollinearDistributionEvolutionApfel::PARAM_NAME_TAB_UPPER_BOUND
                        << " configured with value = " << getTabUpperBound());
    }

    if (parameters.isAvailable(
            CollinearDistributionEvolutionApfel::PARAM_NAME_TAB_INTER_DEGREE)) {
        setTabInterDegree(parameters.getLastAvailable().toInt());
        info(__func__,
                ElemUtils::Formatter()
                        << CollinearDistributionEvolutionApfel::PARAM_NAME_TAB_INTER_DEGREE
                        << " configured with value = " << getTabInterDegree());
    }
}

void CollinearDistributionEvolutionApfel::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    CollinearDistributionEvolutionModule::prepareSubModules(subModulesData);

    apfel::Banner();
    apfel::SetVerbosityLevel(0);
std::cout << m_subgridNodes.size() << std::endl;
    // Setup APFEL++ x-space
    std::vector<apfel::SubGrid> vsg;
    for (int i = 0; i < (int) m_subgridNodes.size(); i++)
        vsg.push_back(apfel::SubGrid { m_subgridNodes[i],
                m_subgridLowerBounds[i], m_subgridInterDegrees[i] });
    m_g = std::unique_ptr<apfel::Grid>(new apfel::Grid { vsg });

    // Running coupling
    const auto as =
            [=] (double const& mu) -> double {return getRunningAlphaStrongModule()->compute(mu * mu);};

    // Initialize QCD evolution objects
    const auto DglapObj = InitializeDglapObjectsQCD(*m_g, m_masses,
            m_thresholds, true);

    // Construct the DGLAP evolution operators
    const auto EvolvedOps = BuildDglap(DglapObj, this->getMuF_ref(),
            this->getPertOrder() - 1, as);

    // Tabulate evolution Operator
    m_tabulatedOps = std::unique_ptr<
            apfel::TabulateObject<apfel::Set<apfel::Operator>>>(new apfel::TabulateObject<apfel::Set<apfel::Operator>>
    {   *EvolvedOps, m_tabNodes, m_tabLowerBound, m_tabUpperBound, m_tabInterDegree});
}

PartonDistribution CollinearDistributionEvolutionApfel::compute(
        CollinearDistributionModule* pCollinearDistributionModule) {

    // Get kinematics
    const CollinearDistributionKinematic kin = this->getKinematics();

    // Get evolution operators at the final scale
    auto tops = m_tabulatedOps->Evaluate(
            sqrt(kin.getMuF2().makeSameUnitAs(PhysicalUnit::GEV2).getValue()));

    // Set appropriate convolution basis for the evolution operators
    // and convolute them with initial-scale distributions.
    tops.SetMap(apfel::EvolveDistributionsBasisQCD { });
    const std::map<int, apfel::Distribution> topsdist =
            apfel::QCDEvToPhys(
                    (tops
                            * apfel::Set<apfel::Distribution> {
                                    apfel::EvolveDistributionsBasisQCD { },
                                    DistributionMap(*m_g,
                                            initialScaleDistributions(
                                                    pCollinearDistributionModule)) }).GetObjects());
    //const std::map<int, apfel::Distribution> topsdist = apfel::QCDEvToPhys(DistributionMap(*m_g, initialScaleDistributions(pCollinearDistributionModule)));

    // Put result in a parton distribution object and return it
    std::map<QuarkFlavor::Type, QuarkDistribution> qd;
    for (int i = 1; i <= 6; i++) {
        int j = i;
        if (i == 1)
            j = 2;
        else if (i == 2)
            j = 1;

        const double q = topsdist.at(i).Evaluate(this->m_x);
        const double qb = topsdist.at(-i).Evaluate(this->m_x);
        qd.insert( { (QuarkFlavor::Type) j, QuarkDistribution {
                (QuarkFlavor::Type) j, q, q + qb, q - qb } });
    }

    PartonDistribution pd;
    pd.setGluonDistribution(
            GluonDistribution { topsdist.at(0).Evaluate(this->m_x) });
    pd.setQuarkDistributions(qd);

    return pd;
}

std::function<std::map<int, double>(double const&)> CollinearDistributionEvolutionApfel::initialScaleDistributions(
        CollinearDistributionModule* pCollinearDistributionModule) {
    return [=] (double const& x) -> std::map<int, double> {

        // Define kinematics
        const double muF02 = this->getMuF2_ref();
        const CollinearDistributionKinematic kin {x, muF02, muF02};

        // Get distributions
        PartonDistribution pd = pCollinearDistributionModule->compute(kin, this->getCollinearDistributionType());

        // Put them in a map
        std::map<int, double> physd { {0, pd.getGluonDistribution().getGluonDistribution()}};
        for (QuarkFlavor::Type const t : pd.listTypeOfQuarkFlavor()) {

            // Swap up and down according to the PDG convention
            int fl = (int) t;
            if ((int) t == 1)
            fl = 2;
            else if ((int) t == 2)
            fl = 1;

            const double q = pd.getQuarkDistribution(t).getQuarkDistribution();
            const double qb = pd.getQuarkDistribution(t).getQuarkDistributionPlus() - q;
            physd.insert( {fl, q});
            physd.insert( {-fl, qb});
        }

        // Rotate distributions into the QCD evolution basis and
        // return
        return apfel::PhysToQCDEv(physd);
    };
}

void CollinearDistributionEvolutionApfel::setThresholds(
        const std::vector<double>& thresholds) {
    m_thresholds = thresholds;
}

void CollinearDistributionEvolutionApfel::setMasses(
        const std::vector<double>& masses) {
    m_masses = masses;
}

void CollinearDistributionEvolutionApfel::setSubgridNodes(
        const std::vector<int>& subgridNodes) {
    m_subgridNodes = subgridNodes;
}

void CollinearDistributionEvolutionApfel::setSubgridLowerBounds(
        const std::vector<double>& subgridLowerBounds) {
    m_subgridLowerBounds = subgridLowerBounds;
}

void CollinearDistributionEvolutionApfel::setSubgridInterDegrees(
        const std::vector<int>& subgridInterDegrees) {
    m_subgridInterDegrees = subgridInterDegrees;
}

void CollinearDistributionEvolutionApfel::setTabNodes(const int& tabNodes) {
    m_tabNodes = tabNodes;
}

void CollinearDistributionEvolutionApfel::setTabLowerBound(
        const double& tabLowerBound) {
    m_tabLowerBound = tabLowerBound;
}

void CollinearDistributionEvolutionApfel::setTabUpperBound(
        const double& tabUpperBound) {
    m_tabUpperBound = tabUpperBound;
}

void CollinearDistributionEvolutionApfel::setTabInterDegree(
        const int& tabInterDegree) {
    m_tabInterDegree = tabInterDegree;
}

std::vector<double> CollinearDistributionEvolutionApfel::getThresholds() const {
    return m_thresholds;
}

std::vector<double> CollinearDistributionEvolutionApfel::getMasses() const {
    return m_masses;
}

std::vector<int> CollinearDistributionEvolutionApfel::getSubgridNodes() const {
    return m_subgridNodes;
}

std::vector<double> CollinearDistributionEvolutionApfel::getSubgridLowerBounds() const {
    return m_subgridLowerBounds;
}

std::vector<int> CollinearDistributionEvolutionApfel::getSubgridInterDegrees() const {
    return m_subgridInterDegrees;
}

int CollinearDistributionEvolutionApfel::getTabNodes() const {
    return m_tabNodes;
}

double CollinearDistributionEvolutionApfel::getTabLowerBound() const {
    return m_tabLowerBound;
}

double CollinearDistributionEvolutionApfel::getTabUpperBound() const {
    return m_tabUpperBound;
}

int CollinearDistributionEvolutionApfel::getTabInterDegree() const {
    return m_tabInterDegree;
}

CollinearDistributionEvolutionApfel::CollinearDistributionEvolutionApfel(
        const CollinearDistributionEvolutionApfel &other) :
        CollinearDistributionEvolutionModule(other) {
    m_thresholds = other.getThresholds();
    m_masses = other.getMasses();
    m_subgridNodes = other.getSubgridNodes();
    m_subgridLowerBounds = other.getSubgridLowerBounds();
    m_subgridInterDegrees = other.getSubgridInterDegrees();
    m_tabNodes = other.getTabNodes();
    m_tabLowerBound = other.getTabLowerBound();
    m_tabUpperBound = other.getTabUpperBound();
    m_tabInterDegree = other.getTabInterDegree();
}

void CollinearDistributionEvolutionApfel::initModule() {
    CollinearDistributionEvolutionModule::initModule();
}

void CollinearDistributionEvolutionApfel::isModuleWellConfigured() {
    CollinearDistributionEvolutionModule::isModuleWellConfigured();

    if (m_thresholds.empty())
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Vector of thresholds empty");

    if (m_masses.empty())
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Vector of masses empty");

    if (m_thresholds.size() != m_masses.size())
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Vectors of masses and thresholds different in size");

    if (m_subgridNodes.empty())
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Vector of numbers of grid nodes not correctly set (empty)");

    if (m_subgridLowerBounds.empty())
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Vector of lower bounds of the grid not correctly set (empty)");

    if (m_subgridInterDegrees.empty())
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Vector of interpolation degrees not correctly set (empty)");

    if (!(m_subgridNodes.size() && m_subgridLowerBounds.size()
            && m_subgridInterDegrees.size()))
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Size of vectors differs");

    if (m_tabNodes <= 0)
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Number of grid nodes not correctly set (negative)");

    if (m_tabLowerBound <= 0)
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Lower bound of the grid not correctly set (negative)");

    if (m_tabUpperBound <= 0)
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Upper bound of the grid not correctly set (negative)");

    if (m_tabUpperBound <= m_tabLowerBound)
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Upper bound smaller than the lower bound of the grid");

    if (m_tabInterDegree <= 0)
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Interpolation degree not correctly set (negative)");
}

} /* namespace PARTONS */

