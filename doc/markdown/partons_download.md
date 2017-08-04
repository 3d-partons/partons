# Download %PARTONS {#download}

The sources of %PARTONS include four sub-projects. These are:
* [elementary-utils](https://drf-gitlab.cea.fr/partons/core/elementary-utils): Utility software 
* [numa](https://drf-gitlab.cea.fr/partons/core/numa): numerical analysis C++ routines
* [partons](https://drf-gitlab.cea.fr/partons/core/partons): %PARTONS project
* [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example): executable of %PARTONS with examples (not mendatory) 

The sources are accessible:
* via [our GitLab server](https://drf-gitlab.cea.fr/partons/core)
* via Git command line tool
~~~~~~~~~~~~~{.sh}
cd /path/to/some/directory
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/elementary-utils.git --branch v1
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/numa.git --branch v1
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/partons.git --branch v1
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/partons-example.git --branch v1
~~~~~~~~~~~~~
The option `--branch` is needed to checkout the *release* branch (in that case the first version). The option `GIT_SSL_NO_VERIFY=true` is needed because the CEA certificate is often not recognized.
* via this page
Version  | Status                                           | elementary-utils            | numa			| partons			| partons-example 		|
:------: | :----------------------------------------------: | :-------------------------: | :-------------------------:	| :---------------------------: | :---------------------------: |
1.0      | <pre style="color: green;">Latest version</pre>  | [tar.gz](http://to.be.set)  | [tar.gz](http://to.be.set)	| [tar.gz](http://to.be.set)	| [tar.gz](http://to.be.set)	|
