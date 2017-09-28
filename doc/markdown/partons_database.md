# %Database {#database}

[TOC]

# Introduction {#database_intro}

When working with %PARTONS, one may want to use an SQL database for these reasons (the list is not exhaustive):
* To keep track of results obtained in the past, which is useful in particular to study the evolution of a code and the impact of third-party library versioning on physics results - both important to avoid any kind of regression.
* To avoid repeating time-consuming computations, in particular in those cases where a modification in one computation step (e.g. due to a different physics assumption) does not entail the repetition of other steps (whose results can be kept in the database).
* To establish an aggregation point collecting data streaming from several computation units, which is useful in particular for those users who perform exhaustive calculations at computing farms.
* To store experimental results to be used in all kinds of systematic comparisons to theoretical predictions, in particular in fits.

This tutorial explains how to set up your own SQL database and connect %PARTONS to it. In addition, it illustrates how to perform transactions between this database and %PARTONS and it gives some remarks on the storing of experimental data.

<hr>

# Database set-up {#database_setup}

Currently we support only MySQL databases.

If you are using %PARTONS together with the [virtual machine provided by our team](@ref vm), you will find a MySQL server ready to be used there. It comes with a databse called `partons`, which is set up and can be used out-of-the-box. You may connect to it and check its content as the MySQL user `partons` via command-line tool in the following way:
```py
# in shell 
# connect to server (it will ask you for password)
mysql -u partons -p
```
```sql
-- in MySQL command-line interface
-- use specific database
USE partons;
```
The password for both `partons` and `root` MySQL users is `partons`.

If you do not use the Virtual Machine provided by our team, but you want to use database-related functionalities in %PARTONS, you need to get an access to a MySQL server by your own. Possibly, it can be granted by your home institute, which is useful, if you intend to use the same database together with your collaborators. You can also install a MySQL server on your local machine, which nowadays is really easy. For *e.g.* Debian-like Linux distributions, you can proceed in the following way:
```py
# use repositories to install MySQL server 
sudo apt-get install mysql-server 

# run this script
sudo mysql_secure_installation
```
During the installation process or when executing the script `mysql_secure_installation`, you will be asked to set your own administration password to manage your server. For other Linux distributions and operating systems some adjustments to the installation receipt may be needed.

With the MySQL server available, its administrator (possibly you) should set up a database for %PARTONS. This can be accomplished easily with MySQL scripts provided by our team. These scripts are available in the directory `data/database/sql_schema` of the %PARTONS library ([partons](https://drf-gitlab.cea.fr/partons/core/partons) project) and they should be used in the following way:
```py
# in shell 
# go to directory containing scripts 
cd database/sql_schema

# use MySQL command-line tool (it will ask you for administrator password, possibly the one set during MySQL server installation)
mysql -u root -p
# on some systems you may need to run this command as root 
sudo mysql -u root -p
```
```sql
/* in MySQL command-line interface */
/* create new database */
CREATE DATABASE partons;

/* create new user whom you should use for all kind of non-administrative activities */
CREATE USER 'partons'@'localhost' IDENTIFIED BY 'partons';
GRANT SELECT, INSERT, UPDATE, DELETE, FILE ON partons.* TO 'partons'@'localhost';

/* use database */ 
USE partons;

/* load scripts */
SOURCE mysql5_common.sql;
SOURCE mysql5_gpd_layer.sql;
SOURCE mysql5_convol_coeff_function_layer.sql;
SOURCE mysql5_observable_layer.sql;
```
Here, we have set the name of the database, the name of the user and his password to `partons`. Note however that you may modify these parameters freely - you just need to set them correctly in the %PARTONS configuration file to enable the connection.

<hr>

# Connect PARTONS to your database {#database_connection}

The connection parameters should be set in [the main configuration file of PARTONS](@ref config), i.e. the one called `partons.properties`. Analyze these examples that illustrate the configuration used to establish the connection to either a local (your computer) or a remote MySQL server.

Connection to a local server:
```py
database.production.type = MYSQL
database.production.url = localhost
database.production.dbname = your_partons_database_name
database.production.user = your_sql_user_name 
database.production.passwd = your_sql_user_password 
```

Connection to a remote server:
```py
database.production.type = MYSQL
database.production.url = remote.address.example.com 
database.production.dbname = your_partons_database_name
database.production.user = your_sql_user_name
database.production.passwd = your_sql_user_password
```

These two parameters:
```py
database.load.infile.use = true
database.load.infile.directory = /path/to/tmp
```
allow to enable the transaction mechanism that is based on temporary files loaded into the database. The mechanism is very fast, which is important for the insertion of large sets of data. However, not all databases allow it by default, so it may require a special configuration of the server - this subject is however not covered in this tutorial. 

<hr>

# Database design {#database_design}

The database design reflects the layered structure used in %PARTONS, with tables corresponding to C++ objects and rows corresponding to instances of these objects. There exist also tables storing general information on performed computations. The database design is explained here separately for the common tables and each of the layers by a single UML graph: 

* [Database design of tables used by all layers](@ref database_design_1).
* [Database design of GPD layer](@ref database_design_2).
* [Database design of CFF layer](@ref database_design_3).
* [Database design of observable layer](@ref database_design_4).

<hr>

# PARTONS database interface {#database_interface}

%PARTONS' database interface is provided by Services. These Services are developed in order to either store or retrieve complex C++ objects from database tables in a simple and generic way. They are designed in a way that allows to perform SQL transactions by users who do not know the SQL query language and the database design. These Services use the management of connectors of Qt, a third-party library, allowing transactions with specific database types. The involvement of Qt makes it possible to send the same SQL request to databases of different types (MySQL, SQLite, ...). With transaction and rollback mechanisms, the Services ensure the integrity of the database being currently used - if something wrong happens, the database always stays in a stable state. In addition, Services improve querying speed by using transaction and commit mechanisms for a large number of simultaneous queries.

With the [XML interface](@ref usage_xml), users may ignore the existence of Services - they are used implicitly. If a given computation is done via XML interface, the results can be stored in the database together with the associated scenario, i.e. the input XML file. It allows either to track back all hypothesis underlying the results, or to be able to evaluate them again later. With this feature, it becomes easy to see how something was computed even if we ask ourselves a long time after.

This example illustrates how to store a single GPD result into the database via XML interface (note `storeInDB="1"` option being crucial here):

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>

<scenario date="2017-01-01" description="Exemplary description - description are very useful!">

    <task service="GPDService" method="computeGPDModel" storeInDB="1">
        <kinematics type="GPDKinematic">
            <param name="x" value="-0.1" />
            <param name="xi" value="0.05" />
            <param name="t" value="0." />
            <param name="MuF2" value="2." />
            <param name="MuR2" value="2." />
        </kinematics>

        <computation_configuration>
            <module type="GPDModule">
                <param name="className" value="MMS13Model" />
            </module>
        </computation_configuration>

    </task>

</scenario>
```

With the [C++ interface](@ref usage_cpp), users use Services explicitly. Each Service may be seen as a bridge between a single physics-related object and a corresponding SQL table (see [Database design](@ref database_design) section). Services are listed in the following table together with the aforementioned correspondence:
| Service | Corresponding C++ object | Corresponding SQL table name |
| :---- | :---- | :---- |
| [GPDKinematicDaoService](@ref PARTONS::GPDKinematicDaoService) | [GPDKinematic](@ref PARTONS::GPDKinematic) | [gpd_kinematic](@ref database_design_2) |
| [GPDResultDaoService](@ref PARTONS::GPDResultDaoService) | [GPDResult](@ref PARTONS::GPDResult) | [gpd_result](@ref database_design_2) |
||||
| [ConvolCoeffFunctionKinematicDaoService](@ref PARTONS::ConvolCoeffFunctionKinematicDaoService) | [DVCSConvolCoeffFunctionKinematic](@ref PARTONS::DVCSConvolCoeffFunctionKinematic) | [cff_kinematic](@ref database_design_3) |
| [ConvolCoeffFunctionResultDaoService](@ref PARTONS::ConvolCoeffFunctionResultDaoService) | [DVCSConvolCoeffFunctionResult](@ref PARTONS::DVCSConvolCoeffFunctionResult) | [cff_result](@ref database_design_3) |
||||
| [ObservableKinematicDaoService](@ref PARTONS::ObservableKinematicDaoService) | [ObservableKinematic](@ref PARTONS::ObservableKinematic) | [observable_kinematic](@ref database_design_4) |
| [ObservableResultDaoService](@ref PARTONS::ObservableResultDaoService) | [ObservableResult](@ref PARTONS::ObservableResult) | [observable_result](@ref database_design_4) |

This example illustrates how to use Services via C++ interface:
```cpp
//evaluate exemplary GPD result to be inserted in database

//retrieve GPD service
PARTONS::GPDService* pGPDService = PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

//load GPD module with the BaseModuleFactory
PARTONS::GPDModule* pGPDModel = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(PARTONS::GPDMMS13::classId);

//define GPD kinematics used in computation
PARTONS::GPDKinematic gpdKinematic(-0.1, 0.05, 0., 2., 2.);

//define list of GPD types to be computed
PARTONS::List<GPDType> gpdTypeList;
gpdTypeList.add(PARTONS::GPDType::ALL);

//evaluate
PARTONS::GPDResult gpdResult = pGPDService->computeGPDModel(gpdKinematic, pGPDModel, gpdTypeList);

//get GPDResultDaoService
PARTONS::GPDResultDaoService gpdResultDaoService;

//insert result into database
int id = gpdResultDaoService.insert(gpdResult);
```
For the list of all possible operations provided by Services, see their documentation. Note, that the documentation contains a number of additional examples. 

<hr>

# Store experimental data in database {#database_experimentaldata}

The design of the %PARTONS database allows to store experimental data. Not only kinematics and results with uncertainties, but also information concerning related experiments can be stored in the database. These informations can be used later to make systematic comparisons with theoretical predictions, where experimental data are easily selected with a list of user-defined criteria. To introduce a new set of experimental data into the database, one can use scripts provided by our team, to be found in the `data/database/insert_exp_data` folder of your %PARTONS copy. We refer to the file `data/database/insert_experimental_data/README.md` for more information.

Note that, like all resources in the folder `data`, during the installation of %PARTONS, these are copied by default to `/usr/local/share/PARTONS`.
