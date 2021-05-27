#include "../../../../../include/partons/modules/evolution/gpd/GPDEvolutionApfel.h"

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongModule.h"

namespace PARTONS {

const std::string GPDEvolutionApfel::PARAM_NAME_THRESHOLDS = "thresholds";
const std::string GPDEvolutionApfel::PARAM_NAME_MASSES = "masses";

const std::string GPDEvolutionApfel::PARAM_NAME_SUBGRID_NODES = "subgridNodes";
const std::string GPDEvolutionApfel::PARAM_NAME_SUBGRID_LOWER_BOUNDS = "subgridLowerBounds";
const std::string GPDEvolutionApfel::PARAM_NAME_SUBGRID_INTER_DEGREES = "subgridInterDegrees";

const std::string GPDEvolutionApfel::PARAM_NAME_TAB_NODES = "tabNodes";
const std::string GPDEvolutionApfel::PARAM_NAME_TAB_LOWER_BOUND = "tabLowerBound";
const std::string GPDEvolutionApfel::PARAM_NAME_TAB_UPPER_BOUND = "tabUpperBound";
const std::string GPDEvolutionApfel::PARAM_NAME_TAB_INTER_DEGREE = "tabInterDegree";

const unsigned int GPDEvolutionApfel::classId = BaseObjectRegistry::getInstance()->registerBaseObject(new GPDEvolutionApfel("GPDEvolutionApfel"));

GPDEvolutionApfel::GPDEvolutionApfel(const std::string &className) :
        GPDEvolutionModule(className),
	m_thresholds({}),
	m_masses({}),
	m_subgridNodes({200, 200, 200, 200}),
	m_subgridLowerBounds({1.e-3, 2.e-1, 4.e-1, 8.e-1}),
	m_subgridInterDegrees({3, 3, 3, 3}),
	m_tabNodes(0),
	m_tabLowerBound(0),
	m_tabUpperBound(0),
	m_tabInterDegree(0),
        m_xi_prev(-1),
	m_g(nullptr),
	m_as(nullptr),
	m_tabulatedGpds(nullptr) {

	//Grid
	initializeGrid();
}

GPDEvolutionApfel::~GPDEvolutionApfel() {
}

GPDEvolutionApfel* GPDEvolutionApfel::clone() const {
    return new GPDEvolutionApfel(*this);
}

std::string GPDEvolutionApfel::toString() const {
    return GPDEvolutionModule::toString();
 }

void GPDEvolutionApfel::resolveObjectDependencies() {
    GPDEvolutionModule::resolveObjectDependencies();
}

void GPDEvolutionApfel::configure(const ElemUtils::Parameters &parameters) {
    GPDEvolutionModule::configure(parameters);

    if (parameters.isAvailable(GPDEvolutionApfel::PARAM_NAME_THRESHOLDS)) {
        setThresholds(parameters.getLastAvailable().toVectorDouble());
	std::ostringstream oss;
	std::copy(m_thresholds.begin(), m_thresholds.end(), std::ostream_iterator<double>(oss, " "));
	info(__func__, ElemUtils::Formatter() << GPDEvolutionApfel::PARAM_NAME_THRESHOLDS
	     << " configured with value = [ " << oss.str() << "] GeV");
    }

    if (parameters.isAvailable(GPDEvolutionApfel::PARAM_NAME_MASSES)) {
        setMasses(parameters.getLastAvailable().toVectorDouble());
	std::ostringstream oss;
	std::copy(m_masses.begin(), m_masses.end(), std::ostream_iterator<double>(oss, " "));
	info(__func__, ElemUtils::Formatter() << GPDEvolutionApfel::PARAM_NAME_MASSES
	     << " configured with value = [ " << oss.str() << "] GeV");
    }

    if (parameters.isAvailable(GPDEvolutionApfel::PARAM_NAME_SUBGRID_NODES)) {
        setSubgridNodes(parameters.getLastAvailable().toVectorInt());
	std::ostringstream oss;
	std::copy(m_subgridNodes.begin(), m_subgridNodes.end(), std::ostream_iterator<int>(oss, " "));
	info(__func__, ElemUtils::Formatter() << GPDEvolutionApfel::PARAM_NAME_SUBGRID_NODES
	     << " configured with value = [ " << oss.str() << "]");
    }

    if (parameters.isAvailable(GPDEvolutionApfel::PARAM_NAME_SUBGRID_LOWER_BOUNDS)) {
        setSubgridLowerBounds(parameters.getLastAvailable().toVectorDouble());
	std::ostringstream oss;
	std::copy(m_subgridLowerBounds.begin(), m_subgridLowerBounds.end(), std::ostream_iterator<double>(oss, " "));
	info(__func__, ElemUtils::Formatter() << GPDEvolutionApfel::PARAM_NAME_SUBGRID_LOWER_BOUNDS
	     << " configured with value = [ " << oss.str() << "]");
    }

    if (parameters.isAvailable(GPDEvolutionApfel::PARAM_NAME_SUBGRID_INTER_DEGREES)) {
        setSubgridInterDegrees(parameters.getLastAvailable().toVectorInt());
	std::ostringstream oss;
	std::copy(m_subgridInterDegrees.begin(), m_subgridInterDegrees.end(), std::ostream_iterator<int>(oss, " "));
	info(__func__, ElemUtils::Formatter() << GPDEvolutionApfel::PARAM_NAME_SUBGRID_INTER_DEGREES
	     << " configured with value = [ " << oss.str() << "]");
    }

    if (parameters.isAvailable(GPDEvolutionApfel::PARAM_NAME_TAB_NODES)) {
        setTabNodes(parameters.getLastAvailable().toInt());
	info(__func__, ElemUtils::Formatter() << GPDEvolutionApfel::PARAM_NAME_TAB_NODES
	     << " configured with value = " << getTabNodes());
    }

    if (parameters.isAvailable(GPDEvolutionApfel::PARAM_NAME_TAB_LOWER_BOUND)) {
        setTabLowerBound(parameters.getLastAvailable().toDouble());
	info(__func__, ElemUtils::Formatter() << GPDEvolutionApfel::PARAM_NAME_TAB_LOWER_BOUND
	     << " configured with value = " << getTabLowerBound());
    }

    if (parameters.isAvailable(GPDEvolutionApfel::PARAM_NAME_TAB_UPPER_BOUND)) {
        setTabUpperBound(parameters.getLastAvailable().toDouble());
	info(__func__, ElemUtils::Formatter() << GPDEvolutionApfel::PARAM_NAME_TAB_UPPER_BOUND
	     << " configured with value = " << getTabUpperBound());
    }

    if (parameters.isAvailable(GPDEvolutionApfel::PARAM_NAME_TAB_INTER_DEGREE)) {
        setTabInterDegree(parameters.getLastAvailable().toInt());
	info(__func__, ElemUtils::Formatter() << GPDEvolutionApfel::PARAM_NAME_TAB_INTER_DEGREE
	     << " configured with value = " << getTabInterDegree());
    }
}

void GPDEvolutionApfel::prepareSubModules(const std::map<std::string, BaseObjectData>& subModulesData) {
    GPDEvolutionModule::prepareSubModules(subModulesData);
}

PartonDistribution GPDEvolutionApfel::compute(GPDModule* pGPDModule, const GPDType::Type &type) {

	// Running coupling
	// TODO user may invoke setRunningAlphaStrongModule() at any time and there is no easy way to update m_as in such a case
	// TODO m_as is only used here, there is no point to make it a member of the class
	m_as = [=] (double const& mu) -> double{ return getRunningAlphaStrongModule()->compute(mu * mu); };

    // Set initial scale
    m_MuF2_ref = pGPDModule->getMuF2Ref();

    // Set current GPD type
    setGPDType(type);

    // Initialize QCD evolution objects only if xi changes
    if (m_xi != m_xi_prev) {
      // Construct the DGLAP evolution operators
      const auto EvolvedGpds = apfel::BuildDglap(apfel::InitializeGpdObjects(*m_g, m_thresholds, m_xi), initialScaleDistributions(pGPDModule), sqrt(m_MuF2_ref), this->getPertOrder() - 1, m_as);

      // Tabulate evolution Operator
      m_tabulatedGpds = std::shared_ptr<apfel::TabulateObject<apfel::Set<apfel::Distribution>>>(new apfel::TabulateObject<apfel::Set<apfel::Distribution>>
												{*EvolvedGpds, m_tabNodes, m_tabLowerBound, m_tabUpperBound, m_tabInterDegree});

      setPreviousXi(m_xi);
    }

    // Get kinematics
    const GPDKinematic kin = this->getKinematics();

    // Get evolution operators at the final scale
    const std::map<int, apfel::Distribution> gpds = apfel::QCDEvToPhys(m_tabulatedGpds->Evaluate(sqrt(kin.getMuF2().makeSameUnitAs(PhysicalUnit::GEV2).getValue())).GetObjects());

    // Put result in a parton distribution object and return it
    std::map<QuarkFlavor::Type, QuarkDistribution> qd;
    for (int i = 1; i <= 6; i++)
      {
          int j = i;
	  if (i == 1)
	      j = 2;
	  else if (i == 2)
	      j = 1;

	  const double q  = gpds.at(i).Evaluate(this->m_x) / this->m_x;
	  const double qb = gpds.at(-i).Evaluate(this->m_x) / this->m_x;
	  qd.insert({(QuarkFlavor::Type) j, QuarkDistribution{(QuarkFlavor::Type) j, q, q + qb, q - qb}});
      }

    PartonDistribution pd;
    pd.setGluonDistribution(GluonDistribution{gpds.at(0).Evaluate(this->m_x)});
    pd.setQuarkDistributions(qd);

    return pd;
}

std::function<std::map<int, double>(double const&, double const&)> GPDEvolutionApfel::initialScaleDistributions(GPDModule* pGPDModule) {
    return [=] (double const& x, double const& muF0) -> std::map<int, double> {

        // Define kinematics
        const double muF02 = pow(muF0, 2);
	const GPDKinematic kin{x, m_xi, m_t, muF02, muF02};

	// Get distributions
	PartonDistribution pd = pGPDModule->compute(kin, this->getGPDType());

	// Put them in a map
	std::map<int, double> physd{{0, 0.5 * pd.getGluonDistribution().getGluonDistribution()}};
	for (QuarkFlavor::Type const t : pd.listTypeOfQuarkFlavor()) {

	    // Swap up and down according to the PDG convention
	    int fl = (int) t;
	    if ((int) t == 1)
	      fl = 2;
	    else if ((int) t == 2)
	      fl = 1;

	    const double q  = x * pd.getQuarkDistribution(t).getQuarkDistribution();
	    const double qb = x * pd.getQuarkDistribution(t).getQuarkDistributionPlus() - q;
	    physd.insert({fl,  q});
	    physd.insert({-fl, qb});
	}

	// Rotate distributions into the QCD evolution basis and
	// return
	return apfel::PhysToQCDEv(physd);
    };
}

void GPDEvolutionApfel::setThresholds(const std::vector<double>& thresholds) {
  m_thresholds = thresholds;
}

void GPDEvolutionApfel::setMasses(const std::vector<double>& masses) {
  m_masses = masses;
}

void GPDEvolutionApfel::setSubgridNodes(const std::vector<int>& subgridNodes) {

  m_subgridNodes = subgridNodes;

  initializeGrid();
}

void GPDEvolutionApfel::setSubgridLowerBounds(const std::vector<double>& subgridLowerBounds) {

  m_subgridLowerBounds = subgridLowerBounds;

  initializeGrid();
}

void GPDEvolutionApfel::setSubgridInterDegrees(const std::vector<int>& subgridInterDegrees) {

  m_subgridInterDegrees = subgridInterDegrees;

  initializeGrid();
}

void GPDEvolutionApfel::setTabNodes(const int& tabNodes) {
  m_tabNodes = tabNodes;
}

void GPDEvolutionApfel::setTabLowerBound(const double& tabLowerBound) {
  m_tabLowerBound = tabLowerBound;
}

void GPDEvolutionApfel::setTabUpperBound(const double& tabUpperBound) {
  m_tabUpperBound = tabUpperBound;
}

void GPDEvolutionApfel::setTabInterDegree(const int& tabInterDegree) {
  m_tabInterDegree = tabInterDegree;
}

void GPDEvolutionApfel::setPreviousXi(const double& xi_prev) {
  m_xi_prev = xi_prev;
}

std::vector<double> GPDEvolutionApfel::getThresholds() const {
  return m_thresholds;
}

std::vector<double> GPDEvolutionApfel::getMasses() const {
  return m_masses;
}

std::vector<int> GPDEvolutionApfel::getSubgridNodes() const {
  return m_subgridNodes;
}

std::vector<double> GPDEvolutionApfel::getSubgridLowerBounds() const {
  return m_subgridLowerBounds;
}

std::vector<int> GPDEvolutionApfel::getSubgridInterDegrees() const {
  return m_subgridInterDegrees;
}

int GPDEvolutionApfel::getTabNodes() const {
  return m_tabNodes;
}

double GPDEvolutionApfel::getTabLowerBound() const {
  return m_tabLowerBound;
}

double GPDEvolutionApfel::getTabUpperBound() const {
  return m_tabUpperBound;
}

int GPDEvolutionApfel::getTabInterDegree() const {
  return m_tabInterDegree;
}

double GPDEvolutionApfel::getPreviousXi() const {
  return m_xi_prev;
}

std::shared_ptr<apfel::Grid> GPDEvolutionApfel::getGrid() const {
  return m_g;
}

std::function<double(double const&)> GPDEvolutionApfel::getAlphasFunc() const {
  return m_as;
}

std::shared_ptr<apfel::TabulateObject<apfel::Set<apfel::Distribution>>> GPDEvolutionApfel::getTabulatedGDPs() const {
  return m_tabulatedGpds;
}

GPDEvolutionApfel::GPDEvolutionApfel(const GPDEvolutionApfel &other) :
    GPDEvolutionModule(other) {
  m_thresholds          = other.getThresholds();
  m_masses              = other.getMasses();
  m_subgridNodes        = other.getSubgridNodes();
  m_subgridLowerBounds  = other.getSubgridLowerBounds();
  m_subgridInterDegrees = other.getSubgridInterDegrees();
  m_tabNodes            = other.getTabNodes();
  m_tabLowerBound       = other.getTabLowerBound();
  m_tabUpperBound       = other.getTabUpperBound();
  m_tabInterDegree      = other.getTabInterDegree();
  m_xi_prev             = other.getPreviousXi();
  m_as                  = other.getAlphasFunc();
  m_tabulatedGpds       = other.getTabulatedGDPs();

  initializeGrid();
}

void GPDEvolutionApfel::initModule() {
    GPDEvolutionModule::initModule();

    apfel::Banner();
    apfel::SetVerbosityLevel(0);


}

void GPDEvolutionApfel::isModuleWellConfigured() {
    if (m_pertOrder == PerturbativeQCDOrderType::UNDEFINED || m_pertOrder > PerturbativeQCDOrderType::LO)
      throw ElemUtils::CustomException(getClassName(), __func__, "pQCD order either UNDEFINED or not allowed");

    if (!m_pRunningAlphaStrongModule)
      throw ElemUtils::CustomException(getClassName(), __func__, "Pointer to RunningAlphaStrong module is NULL");

    if (!m_pActiveFlavorsModule)
      throw ElemUtils::CustomException(getClassName(), __func__, "Pointer to ActiveFlavorsModule module is NULL");

    if (m_x < -1. || m_x > 1.)
      throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "x is out of range: " << m_x);

    if (m_xi < 0. || m_xi > 1.)
      throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "xi is out of range: " << m_xi);

    if (m_t > 0.)
      throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "t is out of range: " << m_t);

    if (m_MuF2 <= 0.)
      throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "muF2 is out of range: " << m_MuF2);

    if (m_MuR2 <= 0.)
      throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "muR2 is out of range:" << m_MuR2);

    if (m_thresholds.empty())
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Vector of thresholds empty");

    if (m_masses.empty())
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Vector of masses empty");

    if (m_thresholds.size() != m_masses.size())
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Vectors of masses and thresholds different in size");

    if (m_subgridNodes.empty())
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Vector of numbers of grid nodes not correctly set (empty)");

    if (m_subgridLowerBounds.empty())
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Vector of lower bounds of the grid not correctly set (empty)");

    if (m_subgridInterDegrees.empty())
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Vector of interpolation degrees not correctly set (empty)");

    if (!(m_subgridNodes.size() && m_subgridLowerBounds.size() && m_subgridInterDegrees.size()))
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Size of vectors differs");

    if (m_tabNodes <= 0)
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Number of grid nodes not correctly set (negative)");

    if (m_tabLowerBound <= 0)
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Lower bound of the grid not correctly set (negative)");

    if (m_tabUpperBound <= 0)
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Upper bound of the grid not correctly set (negative)");

    if (m_tabUpperBound <= m_tabLowerBound)
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Upper bound smaller than the lower bound of the grid");

    if (m_tabInterDegree <= 0)
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Interpolation degree not correctly set (negative)");
}

void GPDEvolutionApfel::initializeGrid(){

	   std::vector<apfel::SubGrid> vsg;

	    for (int i = 0; i < (int) m_subgridNodes.size(); i++)
	        vsg.push_back(apfel::SubGrid{m_subgridNodes[i], m_subgridLowerBounds[i], m_subgridInterDegrees[i]});

	    m_g = std::unique_ptr<apfel::Grid> (new apfel::Grid{vsg});
}

} /* namespace PARTONS */

