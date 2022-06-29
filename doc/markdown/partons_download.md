# Download %PARTONS {#download}

# Source code {#download_source}

The sources of %PARTONS include four sub-projects. These are:
* [elementary-utils](https://drf-gitlab.cea.fr/partons/core/elementary-utils): utility software (dependency for %PARTONS);
* [numa](https://drf-gitlab.cea.fr/partons/core/numa): numerical analysis C++ routines (dependency for %PARTONS);
* [partons](https://drf-gitlab.cea.fr/partons/core/partons): the %PARTONS library;
* [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example): executable of %PARTONS with examples (not mandatory).

The sources are accessible:
* via [our GitLab repositories](https://drf-gitlab.cea.fr/partons/core);
* via the Git command line tool:
~~~~~~~~~~~~~{.sh}
cd /path/to/some/directory
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/elementary-utils.git --branch release-v3 --depth 1
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/numa.git --branch release-v3 --depth 1
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/partons.git --branch release-v3 --depth 1
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/partons-example.git --branch release-v3 --depth 1
~~~~~~~~~~~~~
The option `--branch` is needed to checkout the specific tagged version from the *release* branch.
The option `GIT_SSL_NO_VERIFY=true` is needed because the CEA certificate is often not recognized.
The option `--depth 1` can be used to download only the tagged commit and nothing else from the git history.
* via the following links:
Version  | Status                                           | elementary-utils            | numa			| partons			| partons-example 		|
:------: | :----------------------------------------------: | :-------------------------: | :-------------------------:	| :---------------------------: | :---------------------------: |
3.0      | <pre style="color: green;">Latest version</pre>  | [tar.gz](https://drf-gitlab.cea.fr/partons/core/elementary-utils/-/archive/release-v3/elementary-utils-release-v3.tar.gz)  | [tar.gz](https://drf-gitlab.cea.fr/partons/core/numa/-/archive/release-v3/numa-release-v3.tar.gz)	| [tar.gz](https://drf-gitlab.cea.fr/partons/core/partons/-/archive/release-v3/partons-release-v3.tar.gz)	| [tar.gz](https://drf-gitlab.cea.fr/partons/core/partons-example/-/archive/release-v3/partons-example-release-v3.tar.gz)	|
2.0      | <pre style="color: red;">Old</pre>  | [tar.gz](https://drf-gitlab.cea.fr/partons/core/elementary-utils/-/archive/release-v2/elementary-utils-release-v2.tar.gz)  | [tar.gz](https://drf-gitlab.cea.fr/partons/core/numa/-/archive/release-v2/numa-release-v2.tar.gz)	| [tar.gz](https://drf-gitlab.cea.fr/partons/core/partons/-/archive/release-v2/partons-release-v2.tar.gz)	| [tar.gz](https://drf-gitlab.cea.fr/partons/core/partons-example/-/archive/release-v2/partons-example-release-v2.tar.gz)	|
1.0      | <pre style="color: red;">Old</pre>  | [tar.gz](https://drf-gitlab.cea.fr/partons/core/elementary-utils/-/archive/release-v1/elementary-utils-release-v1.tar.gz)  | [tar.gz](https://drf-gitlab.cea.fr/partons/core/numa/-/archive/release-v1/numa-release-v1.tar.gz)	| [tar.gz](https://drf-gitlab.cea.fr/partons/core/partons/-/archive/release-v1/partons-release-v1.tar.gz)	| [tar.gz](https://drf-gitlab.cea.fr/partons/core/partons-example/-/archive/release-v1/partons-example-release-v1.tar.gz)	|

<hr>

# Virtual machine {#download_vm}

You can also download our pre-configured virtual machine (see the [VM tutorial](@ref vm)):

Version   | Date       | %PARTONS version | Image                                                       | Size   |
:-------: | :--------: | :--------------: | :---------------------------------------------------------: | :----: |
User      | 29/04/2022 | 3.0              | [Download](http://partons.cea.fr/vm/PARTONS_290422.ova)     | 1.8 GB |
Developer | 29/04/2022 | 2.0              | [Download](http://partons.cea.fr/vm/PARTONS_290422_DEV.ova) | 2.8 GB |

The `Developer` version has a set-up development environment in Eclipse to start contributing to the %PARTONS project, and has more packages installed, which makes the image heavier.

# Docker {#download_docker}

Docker images containing PARTONS with its runtime environment are available via [DockerHub](https://hub.docker.com), see [here](https://hub.docker.com/repository/docker/partons/partons). The basic usage is the following:
* pull the image (by default containing the latest version of PARTONS)
~~~~~~~~~~~~~{.sh}
docker pull partons/partons
~~~~~~~~~~~~~
* run interactively
~~~~~~~~~~~~~{.sh}
docker run -it --rm partons/partons
~~~~~~~~~~~~~
* use image as executable: run specific scenario (here: 'myScenario.xml') stored in your host (here: in 'ABSOLUTEPATH/MYDIR' directory)
~~~~~~~~~~~~~{.sh}
docker run -it --rm -v ABSOLUTEPATH/MYDIR:/root/workspace/partons-example/scenarios partons/partons myScenario.xml
~~~~~~~~~~~~~

For more details please see Docker manual.
