#!/bin/bash
#author Bryan BERTHOU (SPhN / CEA Saclay)
# version 1.0


# Output file path to store information about current machine configuration
OUTPUT_FILE="environment_configuration.dat"

# Get linux OS information
uname -a > $OUTPUT_FILE

# Get first line printed by g++ --version command line
g++ --version | sed -n 1p  >> $OUTPUT_FILE

# Get QT4 version


# Get SFML version


# Get GSL version


# Get ROOT version


# Get CLN version

# SVN revision number for PARTONS
echo "PARTONS" >> $OUTPUT_FILE | svn info https://dsm-trac.cea.fr/svn/prophet/DEVELOPMENT/PARTONS/ --username bberthou | grep Revision >> $OUTPUT_FILE

# SVN revision number for UNIX
echo "NumA++" >> $OUTPUT_FILE | svn info https://dsm-trac.cea.fr/svn/prophet/DEVELOPMENT/NumA++/ --username bberthou | grep Revision >> $OUTPUT_FILE
