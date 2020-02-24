# %PARTONS configuration files {#config}

[TOC]
 
# Introduction {#config_intro} 

This tutorial describes the configuration files used by %PARTONS. If you are using [our virtual machine](@ref vm), all configuration files are set up there, so you can run %PARTONS as it was out-of-the-box. However, if you have installed %PARTONS on your own machine under [Linux](@ref linux) or [Mac](@ref mac), you may need to set up these files manually. 

There are three %PARTONS configuration files that you may be interested in:
* `partons.properties`: main configuration file;
* `logger.properties`: configuration of the Logger;
* `environment_configuration.dat`: environment configuration information.

%All these files are described in the following. Samples are also provided with the library [partons](https://drf-gitlab.cea.fr/partons/core/partons) (folder `data/config`) and the executable project [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example) (folder `bin`).

<hr>

# partons.properties {#config_main}

This is the main configuration file of %PARTONS to be placed together with the executable file. %All options are explained here in the comments starting with the hash symbol '#':

```py
# PATHS #

# Path to the configuration file of Logger 
log.file.path = /path/to/logger.properties

# Path to the environment configuration information
environment.configuration.file.path = /path/to/environment_configuration.dat

# Path to the directory containing PDF replicas 
grid.directory = ../partons/data/grid/

# Validation scheme used by XML parser  
xml.schema.file.path = ../partons/data/xmlSchema.xsd

# DATABASE #

# Database connection definition (for more information see our tutorial explaining the database usage) 
database.production.type = MYSQL
database.production.url = localhost
database.production.port = 3306
database.production.dbname = partons
database.production.user =  partons
database.production.passwd = partons

# Whether to use the temporary file transaction mechanism to load data 
database.load.infile.use = true

# Working directory needed by the temporary file transaction mechanism (if database.load.infile.use is true)
database.load.infile.directory = /path/to/tmp

# THREAD #

# Number of available processors to be used by threads
computation.nb.processor = 1

# Maximum size of batch for a given type (in one task several batches may be run in separate threads)
gpd.service.batch.size = 1000
ccf.service.batch.size = 1000
observable.service.batch.size = 1000
```

The file `xmlSchema.xsd` used by the XML parser is provided both with the library [partons](https://drf-gitlab.cea.fr/partons/core/partons) and the executable project [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example), in the directory `data`. When `partons` is installed, the file is copied typically to `/usr/local/share/PARTONS`. The same can be said for the `grid` folder containing the PDF replicas.

<hr>

# logger.properties {#config_logger}

This is the configuration file of the Logger. The path to this file should be set in `partons.properties` via the `log.file.path` option. %All options are explained here in the comments starting with the hash symbol `#`:

```py
# Enable Logger: 
# true   - Logger enabled
# false  - Logger disabled
enable = true

# Logger output policy:
# ERROR  - print errors only 
# WARN   - print warnings and errors only 
# INFO   - print info, warnings and errors only 
# DEBUG  - print all, i.e.: debug, info, warnings and errors
default.level = INFO

# Logger output policy may be also set locally, i.e. for a single PARTONS module. E.g.
# logger.DVCSCFFHeavyQuarkModel = DEBUG
# can be used to set the debugging mode for DVCSCFFHeavyQuarkModel module only

# Logger destination:
# COUT   - standard output only
# FILE   - text file only
# BOTH   - both standard output and text file
print.mode = BOTH

# Path to the directory containing Logger output text files 
log.folder.path = /path/to/output/folder
```

<hr>

# environment_configuration.dat {#config_env}

This file contains environment configuration information. The path to this file should be set in `partons.properties` via `environment.configuration.file.path` option. The main purpose of this file is to store it in the database during the insertion of data, so later one can easily reproduce the used computational environment. The file must be set by the user and its content may look as follows:

```
system: Linux partonsVM 3.16.0-4-amd64 #1 SMP Debian 3.16.43-2 (2017-04-30) x86_64 GNU/Linux
g++: 4.9.2
qt: 4.8.6
smfl: 2.1
eigen3: 3.2.2
cln: 1.3.4
gsl: 2.3
git elementary-utils: master/2e0e9ee721aa46262545fa9963e935073f829e3e
git numa: master/c13779a34c4847b67cf2f55d12c58fde8c78696b
git partons: master/72bdcfc08c6dd7d8ec8386dc0d36449b4fd28fad
```

The content was obtained with this script run under Linux:

```sh
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
echo "qt: `pkg-config --modversion QtCore`"
echo "smfl: $SFML_A.$SFML_B"
echo "eigen3: `pkg-config --modversion eigen3`"
echo "cln: `pkg-config --modversion cln`"
echo "gsl: `gsl-config --version`"
echo "git elementary-utils: $GIT_ELEM_UTILS"
echo "git numa: $GIT_NUMA"
echo "git partons: $GIT_PARTONS"
```
