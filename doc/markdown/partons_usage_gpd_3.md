# GPD service task {#usage_gpd_3}
 
[TOC]

# Task name {#usage_gpd_3_name}

~~~~~~~~~~~~~{.xml}
printResults
~~~~~~~~~~~~~

# Description {#usage_gpd_3_description}
This task is used to print out results of a GPD computation to the standard output. It should be always preceded by other GPD computational task. If it is preceded by two or more GPD computational tasks, it will only print out the results returned by the last task.

# Example {#usage_gpd_3_example}

* Input XML scenario:
~~~~~~~~~~~~~{.xml} 
<!-- XML prolog -->
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>

<!-- Scenario: date and description -->
<scenario date="2017-06-15" description="How to compute GPD">

   <!-- For the explanation of this task see its documentation --> 
   <task service="GPDService" method="computeGPDModel" storeInDB="0">
      <kinematics type="GPDKinematic">
         <param name="x" value="0.15" />
         <param name="xi" value="0.1" />
         <param name="t" value="-0.1" />
         <param name="MuF2" value="2." />
         <param name="MuR2" value="2." />
      </kinematics>
      <computation_configuration>
         <module type="GPDModule" name="GK11Model">
         </module>
      </computation_configuration>
   </task>

   <!-- Task: service name and method name -->
    	<task service="GPDService" method="printResults">
    	</task>
</scenario>
~~~~~~~~~~~~~

* Standard output:
~~~~~~~~~~~~~{.xml} 
24-06-2017 07:21:01 [INFO] (GPDService::getFinalGPDTypeList) 4 GPDType will be computed
24-06-2017 07:21:01 [INFO] (GPDService::printResultListBuffer) [GPDResult]
m_className = GPDResult - m_objectId = 417 indexId = -1
[GPDKinematic]
m_className = GPDKinematic - m_objectId = 231 indexId = -1
m_x = 0.15 m_xi = 0.1 m_t = -0.1 m_MuF2 = 2(Gev2) m_MuR2 = 2(Gev2)
[PartonDistributionList]
GPD_H
[PartonDistribution]
m_className = PartonDistribution - m_objectId = 436 indexId = -1
GluonDistribution = 0.722456284705623
u = 4.12309
u(+) = 4.60404
u(-) = 3.64213
d = 2.39771
d(+) = 2.87866
d(-) = 1.91675
s = 0.233108
s(+) = 0.466216
s(-) = 0

GPD_E
[PartonDistribution]
m_className = PartonDistribution - m_objectId = 421 indexId = -1
GluonDistribution = 0.264735370786291
u = 2.26772
u(+) = 1.9206
u(-) = 2.61483
d = -3.27606
d(+) = -3.62318
d(-) = -2.92895
s = -0.347116
s(+) = -0.694232
s(-) = 0

GPD_Ht
[PartonDistribution]
m_className = PartonDistribution - m_objectId = 426 indexId = -1
GluonDistribution = 0.281674657482674
u = 1.75465
u(+) = 1.75465
u(-) = 1.75465
d = -0.652245
d(+) = -0.652245
d(-) = -0.652245
s = 0
s(+) = 0
s(-) = 0

GPD_Et
[PartonDistribution]
m_className = PartonDistribution - m_objectId = 431 indexId = -1
GluonDistribution = 0
u = 14.3922
u(+) = 14.3922
u(-) = 14.3922
d = 4.11206
d(+) = 4.11206
d(-) = 4.11206
s = 0
s(+) = 0
s(-) = 0
~~~~~~~~~~~~~


