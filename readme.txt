#
# Author: Bryan BERTHOU (CEA Saclay)
# Date : 19 September 2014
# 
# A readme file to help project's configuration
#

#
# Configure your eclipse environnement
#

Window -> Preferences -> C/C++ -> Build -> Environment

Add this two entries (exemples) :

LD_LIBRARY_PATH = /usr/local/root/v5.34.21/lib
ROOTSYS =  /usr/local/root/v5.34.21

#
# For x64 architecture
#

Right click on project -> properties -> C/C++ Build -> Settings
Switch Configuration to All
GCC C++ Compiler -> Miscellaneous
check -fPIC

#
# For thread use
#

Right click on project -> properties -> C/C++ Build -> Settings
Switch Configuration to All
GCC C++ Compiler -> Miscellaneous
Add "-pthread" to Other flags

Right click on project -> properties -> C/C++ General -> Paths and Symbols
Switch Configuration to All and select Libraires tab
Add "pthread"

#
# Configure Include and Libraries path (Unix example)
#

Right click on project -> properties -> C/C++ General -> Paths and Symbols
Switch Configuration to All

Includes -> GNU C++
${ROOTSYS}/include
/usr/include

Library Paths
${LD_LIBRARY_PATH}
/usr/lib

Libraries
cln
Physics
MathMore
Hist



#
# Mu or Mu2
#

Impossible to use sqrt in xml file used for automation when users want to work with Mu2 = Q2 or Mu = Q.
