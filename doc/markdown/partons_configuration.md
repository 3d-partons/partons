# PARTONS configuration files {#config}

[TOC]

This wiki page describes the configuration files used by PARTONS. If you are using [our virtual machine](@ref vm), all configuration files will be set up and you will be able to run PARTONS as it was out-of-the-box there. However, if you have installed PARTONS at your own machine run under [Linux](@ref linux) or [Mac](@ref mac), you may need to set up these files manually. 

There are three PARTONS configuration files that you may be interested in:
* `partons.properties`: main configuration file
* `logger.cfg`: configuration of Logger
* `environment_configuration.dat`: file containing environment configuration information

All of these files are described in the following.

# partons.properties {#config_main}

This is the main configuration file of PARTONS and it should be placed together with the executable file. Its content may look as follows (all options are explained in the comments starting with the hash symbol '#'):

~~~~~~~~~~~~~{.py}

# PATHS #

# Path to the configuration file of logger 
log.file.path = bin/logger.cfg

# Path to the environment configuration information
environment.configuration.file.path = ../partons/environment_configuration.dat

# Path to the directory containing scenarios 
scenario.directory = ../partons/data/scenario/

# Path to the directory containing PDF replicas 
grid.directory = ../partons/data/grid/

# XML validation scheme needed by the parser in the automation.  
xml.schema.file.path = ../partons/data/automation/xmlSchema.xsd

# DATABASE #

# Database connection definition (for more information see our wiki page explaining the database usage) 
database.production.type = MYSQL
database.production.url = localhost
database.production.dbname = partons
database.production.user =  partons
database.production.passwd = partons

# Temporary working directory needed by the transaction mechanism
temporary.working.directory.path = /home/partons/git/partons-exe/bin/temp

# THREAD #

# Number of availible ptocessors to be used by threads
computation.nb.processor = 1

# Max. batch size of a given type (in a given task several batches may run in separate threads)
gpd.service.batch.size = 10000
ccf.service.batch.size = 1000
observable.service.batch.size = 1000
~~~~~~~~~~~~~

# logger.cfg {#config_logger}

This is the configuration file of Logger and the path to this file should be set in `partons.properties` via `log.file.path` option. Its content may look as follows (all options are explained in the comments starting with the hash symbol '#'):
~~~~~~~~~~~~~{.py}
# Enable logger: 
# true   - logger enable
# false  - logger disabled
enable = true

# Logger policy:
# ERROR  - print only errors 
# WARN   - print only warnings and errors 
# INFO   - print only info, warnings and errors 
# DEBUG  - print all, i.e.: debug, info, warnings and errors
default.level = INFO

# Logger policy may be also set locally, i.e. for a single PARTONS module. E.g.
# logger.DVCSCFFHeavyQuarkModel = DEBUG
# sets the debugging mode for DVCSCFFHeavyQuarkModel module only

# Destination:
# COUT   - standard output only
# FILE   - text file only
# BOTH   - both standard output and text file
print.mode = BOTH

# Path to the directory containing logger's output text files (see print.mode option) 
log.folder.path = bin
~~~~~~~~~~~~~

# environment_configuration.dat {#config_env}

This file contains environment configuration information and the path to this file should be set in `partons.properties` via `environment.configuration.file.path` option. Its content is stored in the database during the insertion of data, so latter one can reproduce the computational environment. This file must be set by the user. It may look like that:
~~~~~~~~~~~~~{.py}
system: Linux partonsVM 3.16.0-4-amd64 #1 SMP Debian 3.16.43-2 (2017-04-30) x86_64 GNU/Linux
g++: 4.9.2
root: 5.34/19
qt: 4.8.6
smfl: 2.1
eigen3: 3.2.2
cln: 1.3.4
git elementary-utils: master/2e0e9ee721aa46262545fa9963e935073f829e3e
git numa: master/c13779a34c4847b67cf2f55d12c58fde8c78696b
git partons: master/72bdcfc08c6dd7d8ec8386dc0d36449b4fd28fad
~~~~~~~~~~~~~
which is the effect of this script run under Linux:
~~~~~~~~~~~~~{.py}
#!/bin/bash

#paths
SMFL_PATH="/usr/include/SFML"
PARTONS_PATH="/home/partons/git"

#run
SFML_A="`cat $SMFL_PATH/Config.hpp | grep SFML_VERSION_MAJOR | grep -Eo '[[:digit:]]*'`"
SFML_B="`cat $SMFL_PATH/Config.hpp | grep SFML_VERSION_MINOR | grep -Eo '[[:digit:]]*'`"

cd $PARTONS_PATH/elementary-utils
GIT_ELEM_UTILS="`git branch | grep \* | sed -e 's/\* '//g`/`git rev-parse HEAD`"

cd $PARTONS_PATH/numa
GIT_NUMA="`git branch | grep \* | sed -e 's/\* '//g`/`git rev-parse HEAD`"

cd $PARTONS_PATH/partons
GIT_PARTONS="`git branch | grep \* | sed -e 's/\* '//g`/`git rev-parse HEAD`"

echo "system: `uname -a`"
echo "g++: `g++ -dumpversion`"
echo "root: `root-config --version`"
echo "qt: `pkg-config --modversion QtCore`"
echo "smfl: $SFML_A.$SFML_B"
echo "eigen3: `pkg-config --modversion eigen3`"
echo "cln: `pkg-config --modversion cln`"
echo "git elementary-utils: $GIT_ELEM_UTILS"
echo "git numa: $GIT_NUMA"
echo "git partons: $GIT_PARTONS"
~~~~~~~~~~~~~
