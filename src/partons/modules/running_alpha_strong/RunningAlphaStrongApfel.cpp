#include "../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongApfel.h"

#include "../../../../include/partons/BaseObjectRegistry.h"

#include <apfel/apfelxx.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/logger/CustomException.h>
#include <iterator>

namespace PARTONS {

const std::string RunningAlphaStrongApfel::PARAM_NAME_ALPHAS_REF = "alphasRef";
const std::string RunningAlphaStrongApfel::PARAM_NAME_MU_REF = "muRef";

const std::string RunningAlphaStrongApfel::PARAM_NAME_THRESHOLDS = "thresholds";
const std::string RunningAlphaStrongApfel::PARAM_NAME_MASSES = "masses";

const std::string RunningAlphaStrongApfel::PARAM_NAME_TAB_NODES = "tabNodes";
const std::string RunningAlphaStrongApfel::PARAM_NAME_TAB_LOWER_BOUND = "tabLowerBound";
const std::string RunningAlphaStrongApfel::PARAM_NAME_TAB_UPPER_BOUND = "tabUpperBound";
const std::string RunningAlphaStrongApfel::PARAM_NAME_TAB_INTER_DEGREE = "tabInterDegree";

const unsigned int RunningAlphaStrongApfel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new RunningAlphaStrongApfel("RunningAlphaStrongApfel"));

RunningAlphaStrongApfel::RunningAlphaStrongApfel(const std::string &className) :
        RunningAlphaStrongModule(className),
	m_pertOrder(PerturbativeQCDOrderType::UNDEFINED),
	m_alphasRef(0),
	m_muRef(0),
	m_thresholds({}),
	m_masses({}),
	m_tabNodes(0),
	m_tabLowerBound(0),
	m_tabUpperBound(0),
	m_tabInterDegree(0) {
}

/*
 * Default destructor.
 */
RunningAlphaStrongApfel::~RunningAlphaStrongApfel() {
}

RunningAlphaStrongApfel* RunningAlphaStrongApfel::clone() const {
    return new RunningAlphaStrongApfel(*this);
}

void RunningAlphaStrongApfel::configure(const ElemUtils::Parameters &parameters) {

    RunningAlphaStrongModule::configure(parameters);

    //check and set
    if (parameters.isAvailable(PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE)) {
      try {
	setPertOrder(static_cast<PerturbativeQCDOrderType::Type>(parameters.getLastAvailable().toUInt()));
      } catch (const std::exception &e) {
	// if an exception is raised it means that it's a string configuration value
	setPertOrder(PerturbativeQCDOrderType(parameters.getLastAvailable().getString()).getType());
      }
      info(__func__, ElemUtils::Formatter() << PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE
	   << " configured with value = " << PerturbativeQCDOrderType(getPertOrder()).toString());
    }

    if (parameters.isAvailable(RunningAlphaStrongApfel::PARAM_NAME_ALPHAS_REF)) {
        setAlphasRef(parameters.getLastAvailable().toDouble());
	info(__func__, ElemUtils::Formatter() << RunningAlphaStrongApfel::PARAM_NAME_ALPHAS_REF
	     << " configured with value = " << getAlphasRef());
    }

    if (parameters.isAvailable(RunningAlphaStrongApfel::PARAM_NAME_MU_REF)) {
        setMuRef(parameters.getLastAvailable().toDouble());
	info(__func__, ElemUtils::Formatter() << RunningAlphaStrongApfel::PARAM_NAME_MU_REF
	     << " configured with value = " << getMuRef());
    }

    if (parameters.isAvailable(RunningAlphaStrongApfel::PARAM_NAME_THRESHOLDS)) {
        setThresholds(parameters.getLastAvailable().toVectorDouble());
	std::ostringstream oss;
	std::copy(m_thresholds.begin(), m_thresholds.end(), std::ostream_iterator<double>(oss, " "));
	info(__func__, ElemUtils::Formatter() << RunningAlphaStrongApfel::PARAM_NAME_THRESHOLDS
	     << " configured with value = [ " << oss.str() << "] GeV");
    }

    if (parameters.isAvailable(RunningAlphaStrongApfel::PARAM_NAME_MASSES)) {
        setMasses(parameters.getLastAvailable().toVectorDouble());
	std::ostringstream oss;
	std::copy(m_masses.begin(), m_masses.end(), std::ostream_iterator<double>(oss, " "));
	info(__func__, ElemUtils::Formatter() << RunningAlphaStrongApfel::PARAM_NAME_MASSES
	     << " configured with value = [ " << oss.str() << "] GeV");
    }

    if (parameters.isAvailable(RunningAlphaStrongApfel::PARAM_NAME_TAB_NODES)) {
        setTabNodes(parameters.getLastAvailable().toInt());
	info(__func__, ElemUtils::Formatter() << RunningAlphaStrongApfel::PARAM_NAME_TAB_NODES
	     << " configured with value = " << getTabNodes());
    }

    if (parameters.isAvailable(RunningAlphaStrongApfel::PARAM_NAME_TAB_LOWER_BOUND)) {
        setTabLowerBound(parameters.getLastAvailable().toDouble());
	info(__func__, ElemUtils::Formatter() << RunningAlphaStrongApfel::PARAM_NAME_TAB_LOWER_BOUND
	     << " configured with value = " << getTabLowerBound());
    }

    if (parameters.isAvailable(RunningAlphaStrongApfel::PARAM_NAME_TAB_UPPER_BOUND)) {
        setTabUpperBound(parameters.getLastAvailable().toDouble());
	info(__func__, ElemUtils::Formatter() << RunningAlphaStrongApfel::PARAM_NAME_TAB_UPPER_BOUND
	     << " configured with value = " << getTabUpperBound());
    }

    if (parameters.isAvailable(RunningAlphaStrongApfel::PARAM_NAME_TAB_INTER_DEGREE)) {
        setTabInterDegree(parameters.getLastAvailable().toInt());
	info(__func__, ElemUtils::Formatter() << RunningAlphaStrongApfel::PARAM_NAME_TAB_INTER_DEGREE
	     << " configured with value = " << getTabInterDegree());
    }

    // Instantiate apfel::AlphaQCD object
    apfel::AlphaQCD as{m_alphasRef, m_muRef, m_masses, m_thresholds, m_pertOrder - 1};

    // Pretabulate AlphaS on a grid
    const apfel::TabulateObject<double> Alphas{as, m_tabNodes, m_tabLowerBound, m_tabUpperBound, m_tabInterDegree};

    // Store AlphaS as a std::function
    m_alphasFunc = [=] (double const& mu) -> double{ return Alphas.Evaluate(mu); };
}

double RunningAlphaStrongApfel::compute() {
  return m_alphasFunc(m_Mu);
}

void RunningAlphaStrongApfel::setPertOrder(const PerturbativeQCDOrderType::Type& pertOrder) {
  m_pertOrder = pertOrder;
}

void RunningAlphaStrongApfel::setAlphasRef(const double& alphasRef) {
  m_alphasRef = alphasRef;
}

void RunningAlphaStrongApfel::setMuRef(const double& muRef) {
  m_muRef = muRef;
}

void RunningAlphaStrongApfel::setThresholds(const std::vector<double>& thresholds) {
  m_thresholds = thresholds;
}

void RunningAlphaStrongApfel::setMasses(const std::vector<double>& masses) {
  m_masses = masses;
}

void RunningAlphaStrongApfel::setTabNodes(const int& tabNodes) {
  m_tabNodes = tabNodes;
}

void RunningAlphaStrongApfel::setTabLowerBound(const double& tabLowerBound) {
  m_tabLowerBound = tabLowerBound;
}

void RunningAlphaStrongApfel::setTabUpperBound(const double& tabUpperBound) {
  m_tabUpperBound = tabUpperBound;
}

void RunningAlphaStrongApfel::setTabInterDegree(const int& tabInterDegree) {
  m_tabInterDegree = tabInterDegree;
}

PerturbativeQCDOrderType::Type RunningAlphaStrongApfel::getPertOrder() const {
  return m_pertOrder;
}

double RunningAlphaStrongApfel::getAlphasRef() const {
  return m_alphasRef;
}

double RunningAlphaStrongApfel::getMuRef() const {
  return m_muRef;
}

std::vector<double> RunningAlphaStrongApfel::getThresholds() const {
  return m_thresholds;
}
std::vector<double> RunningAlphaStrongApfel::getMasses() const {
  return m_masses;
}

int RunningAlphaStrongApfel::getTabNodes() const {
  return m_tabNodes;
}

double RunningAlphaStrongApfel::getTabLowerBound() const {
  return m_tabLowerBound;
}

double RunningAlphaStrongApfel::getTabUpperBound() const {
  return m_tabUpperBound;
}

int RunningAlphaStrongApfel::getTabInterDegree() const {
  return m_tabInterDegree;
}

RunningAlphaStrongApfel::RunningAlphaStrongApfel(const RunningAlphaStrongApfel &other) :
        RunningAlphaStrongModule(other) {
  m_pertOrder      = other.getPertOrder();
  m_alphasRef      = other.getAlphasRef();
  m_muRef          = other.getMuRef();
  m_thresholds     = other.getThresholds();
  m_masses         = other.getMasses();
  m_tabNodes       = other.getTabNodes();
  m_tabLowerBound  = other.getTabLowerBound();
  m_tabUpperBound  = other.getTabUpperBound();
  m_tabInterDegree = other.getTabInterDegree();
  m_alphasFunc     = other.getAlphasFunc();
}

void RunningAlphaStrongApfel::initModule() {
    RunningAlphaStrongModule::initModule();
}

void RunningAlphaStrongApfel::isModuleWellConfigured() {

    RunningAlphaStrongModule::isModuleWellConfigured();

    if (m_pertOrder == PerturbativeQCDOrderType::UNDEFINED)
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Perturbative order undefined");

    if (m_alphasRef <= 0)
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Reference value of alphaS not correctly set (negative)");

    if (m_muRef <= 0)
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Reference value of mu not correctly set (negative)");

    if (m_thresholds.empty())
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Vector of thresholds empty");

    if (m_masses.empty())
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Vector of masses empty");

    if (m_thresholds.size() != m_masses.size())
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Vectors of masses and thresholds different in size");

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

    if (!m_alphasFunc)
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "AlphaS function is not correctly set (empty)");
}

void RunningAlphaStrongApfel::setAlphasFunc(const std::function<double(double const&)>& alphasFunc) {
  m_alphasFunc = alphasFunc;
}

std::function<double(double const&)> RunningAlphaStrongApfel::getAlphasFunc() const {
  return m_alphasFunc;
}

} /* namespace PARTONS */
