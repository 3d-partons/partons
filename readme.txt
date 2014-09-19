# For x64 architecture

Right click on project -> properties -> C/C++ Build
Switch Configuration to All
GCC C++ Compiler -> Miscellaneous
check -fPIC

# Configure Include and Libraries path (Unix example)

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
