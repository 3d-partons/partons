#include "../../../../../include/partons/modules/evolution/collinear_distribution/CollinearDistributionEvolutionApfel.h"

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongModule.h"

namespace PARTONS {

const std::string CollinearDistributionEvolutionApfel::PARAM_NAME_SUBGRID_NODES = "subgridNodes";
const std::string CollinearDistributionEvolutionApfel::PARAM_NAME_SUBGRID_LOWER_BOUNDS = "subgridLowerBounds";
const std::string CollinearDistributionEvolutionApfel::PARAM_NAME_SUBGRID_INTER_DEGREES = "subgridInterDegrees";

const unsigned int CollinearDistributionEvolutionApfel::classId = BaseObjectRegistry::getInstance()->registerBaseObject(new CollinearDistributionEvolutionApfel("CollinearDistributionEvolutionApfel"));

CollinearDistributionEvolutionApfel::CollinearDistributionEvolutionApfel(
        const std::string &className) :
        CollinearDistributionEvolutionModule(className),
	m_subgridNodes({100, 60, 50, 50}),
	m_subgridLowerBounds({0.0001, 0.1, 0.6, 0.8}),
	m_subgridInterDegrees({3, 3, 3, 3}) {
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
}

void CollinearDistributionEvolutionApfel::prepareSubModules(const std::map<std::string, BaseObjectData>& subModulesData) {
    CollinearDistributionEvolutionModule::prepareSubModules(subModulesData);

    apfel::Banner();
    apfel::SetVerbosityLevel(0);

    // Setup APFEL++ x-space
    std::vector<apfel::SubGrid> vsg;
    for (int i = 0; i < (int) m_subgridNodes.size(); i++)
      vsg.push_back(apfel::SubGrid {m_subgridNodes[i], m_subgridLowerBounds[i], m_subgridInterDegrees[i]});
    m_g = std::unique_ptr<apfel::Grid>(new apfel::Grid { vsg });

    // Get thresholds. Set to zero whatever is below one.
    std::vector<double> thresholds;
    for (ActiveFlavorsThresholds aft : m_pActiveFlavorsModule->getNfIntervals())
      thresholds.push_back(aft.getLowerBound() < 1 ? 0 : sqrt(aft.getLowerBound()));

    // Initialize QCD evolution objects
    m_dglapobj = InitializeDglapObjectsQCD(*m_g, thresholds);
}

PartonDistribution CollinearDistributionEvolutionApfel::compute(CollinearDistributionModule* pCollinearDistributionModule) {
    // Running coupling
    const auto as = [=] (double const& mu) -> double {return getRunningAlphaStrongModule()->compute(mu * mu);};

    // Construct the DGLAP evolution operators
    const auto EvolvedPDFs = BuildDglap(m_dglapobj, initialScaleDistributions(pCollinearDistributionModule), sqrt(pCollinearDistributionModule->getMuF2Ref()), this->getPertOrder() - 1, as);

    // Get kinematics
    const CollinearDistributionKinematic kin = this->getKinematics();

    // Get PDFs at the final scale
    const std::map<int, apfel::Distribution> topsdist =
      apfel::QCDEvToPhys(EvolvedPDFs->Evaluate(sqrt(kin.getMuF2().makeSameUnitAs(PhysicalUnit::GEV2).getValue())).GetObjects());

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

std::function<std::map<int, double>(double const&, double const&)> CollinearDistributionEvolutionApfel::initialScaleDistributions(CollinearDistributionModule* pCollinearDistributionModule) {
    return [=] (double const& x, double const& Q) -> std::map<int, double> {

        // Define kinematics
        const double muF02 = pow(Q, 2);
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

std::vector<int> CollinearDistributionEvolutionApfel::getSubgridNodes() const {
    return m_subgridNodes;
}

std::vector<double> CollinearDistributionEvolutionApfel::getSubgridLowerBounds() const {
    return m_subgridLowerBounds;
}

std::vector<int> CollinearDistributionEvolutionApfel::getSubgridInterDegrees() const {
    return m_subgridInterDegrees;
}

CollinearDistributionEvolutionApfel::CollinearDistributionEvolutionApfel(
        const CollinearDistributionEvolutionApfel &other) :
        CollinearDistributionEvolutionModule(other) {
    m_subgridNodes = other.getSubgridNodes();
    m_subgridLowerBounds = other.getSubgridLowerBounds();
    m_subgridInterDegrees = other.getSubgridInterDegrees();
}

void CollinearDistributionEvolutionApfel::initModule() {
    CollinearDistributionEvolutionModule::initModule();
}

void CollinearDistributionEvolutionApfel::isModuleWellConfigured() {
    CollinearDistributionEvolutionModule::isModuleWellConfigured();

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
}

} /* namespace PARTONS */

