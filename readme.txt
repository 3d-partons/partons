#
# Author: Bryan BERTHOU (CEA Saclay)
# Date : 19 September 2014
# 
# A readme file to help project's configuration
#

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
/usr/include/root
/usr/include

Library Paths
/usr/lib
/usr/lib/root

Libraries
cln
MathCore
Hist
Matrix
Core
Cint
