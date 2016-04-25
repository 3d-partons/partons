//#ifndef COMPUTATION_CONFIGURATION_MANAGER_H
//#define COMPUTATION_CONFIGURATION_MANAGER_H
//
///**
// * @file ComputationConfigurationManager.h
// * @author Bryan BERTHOU (SPhN / CEA Saclay)
// * @date 22 March 2016
// * @version 1.0
// *
// * @class ComputationConfigurationManager
// *
// * @brief A ComputationConfiguration object is affected to a Computation object when a Result or a ResultList is computed.
// * A ComputationConfiguration object represents by an XML file the configuration of the hypothesis assumption or the different configuration of modules.
// * With automation process user can deals with many XML file to industrialize his computation.
// * To avoid possibly huge memory space allocation when manipulating those XML files associated to a ComputationConfuguration I created this manager.
// * Its goal store only one instance of ComputationConfiguration object that can refers to many Computation object.
// * In other hand it works like a proxy system. It stored only blank ComputationConfiguration object. I mean only pointer position in XML file is stored and not the XML file itself.
// * It loads XML file into memory only when getConfigurationFile(...) method is called : for example when object is printed or stored in database.
// * Same mechanism is used with MD5 or SHA256 : only computed when it's needed.
// */
//
//#include <map>
//#include <string>
//
//#include "beans/automation/ComputationConfiguration.h"
//
//class ComputationConfigurationManager {
//public:
//    static ComputationConfigurationManager* getInstance();
//
//    virtual ~ComputationConfigurationManager();
//
//
//
//private:
//    static ComputationConfigurationManager* pInstance; ///< Private pointer of this class for a unique instance
//
//    ComputationConfigurationManager();
//
//    // Disable copy
//    ComputationConfigurationManager(ComputationConfigurationManager const&); // Don't Implement
//    void operator=(ComputationConfigurationManager const&);   // Don't implement
//
//
//    typedef std::map<unsigned int, ComputationConfiguration*> ComputationConfigurationByXMLIndexMap;
//
//    std::map<std::string, ComputationConfigurationByXMLIndexMap> m_computationConfigurationList;
//};
//
//#endif /* COMPUTATION_CONFIGURATION_MANAGER_H */
