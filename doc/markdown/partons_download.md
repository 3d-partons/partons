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
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/elementary-utils.git --branch v1.0 --depth 1
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/numa.git --branch v1.0 --depth 1
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/partons.git --branch v1.0 --depth 1
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/partons-example.git --branch v1.0 --depth 1
~~~~~~~~~~~~~
The option `--branch` is needed to checkout the specific tagged version from the *release* branch.
The option `GIT_SSL_NO_VERIFY=true` is needed because the CEA certificate is often not recognized.
The option `--depth 1` can be used to download only the tagged commit and nothing else from the git history.
* via the following links:
Version  | Status                                           | elementary-utils            | numa			| partons			| partons-example 		|
:------: | :----------------------------------------------: | :-------------------------: | :-------------------------:	| :---------------------------: | :---------------------------: |
1.0      | <pre style="color: green;">Latest version</pre>  | [tar.gz](https://drf-gitlab.cea.fr/partons/core/elementary-utils/repository/v1.0/archive.tar.gz)  | [tar.gz](https://drf-gitlab.cea.fr/partons/core/numa/repository/v1.0/archive.tar.gz)	| [tar.gz](https://drf-gitlab.cea.fr/partons/core/partons/repository/v1.0/archive.tar.gz)	| [tar.gz](https://drf-gitlab.cea.fr/partons/core/partons-example/repository/v1.0/archive.tar.gz)	|


<hr>

# Virtual machine {#download_vm}

You can also download our pre-configured virtual machine (see the [VM tutorial](@ref vm)):

Version   | Date       | %PARTONS version | Image                                                       | Size   |
:-------: | :--------: | :--------------: | :---------------------------------------------------------: | :----: |
User      | 19/03/2018 | 1.0              | [Download](http://partons.cea.fr/vm/PARTONS_190318.ova)     | 1.6 GB |
Developer | 19/03/2018 | 1.0              | [Download](http://partons.cea.fr/vm/PARTONS_190318_DEV.ova) | 2.6 GB |

The `Developer` version has a set-up development environment in Eclipse to start contributing to the %PARTONS project, and has more packages installed, which makes the image heavier.
