# Database {#database}

[TOC]

# Introduction {#database_intro}

When working with PARTONS one may want to use SQL database for these reasons (the list does not exploit all possibilities):
* to keep track of results obtained in the past, which is useful in particular to study the evolution of code and the impact of third-party library versioning on physics results - both important to avoid any kind of regression.
* to avoid repeating time-consuming computations, in particular in those cases where a modification in one computation step (e.g. due to a different physics assumption) does not entail the repetition of other steps (which results can be kept in the database).
* to establish an aggregation point collecting data streaming from several computation units, which is useful in particular for those users who perform exhaustive calculations at computing farms.
* to store experimental results to be used in all kinds of systematic comparisons to theoretical predictions, in particular in fits.

This wiki page explains how to set up your own SQL database and connect PARTONS to it. In addition, it illustrates how to perform transactions between this database and PARTONS and it gives some remarks on the storing of experimental data.

# Database set-up # {#database_setup}

Currently we support only MySQL databases.

If you are using PARTONS together with [virtual machine provided by our team](@ref vm), you will find MySQL server ready to be used there. It comes with _partons_ database, which is set up and can be used out-of-the-box. You may connect to it and check its content as _partons_ MySQL user via command-line tool in the following way:
~~~~~~~~~~~~~{.sh}
# in shell 
# connect to server (it will ask you for password)
mysql -u partons -p
~~~~~~~~~~~~~
~~~~~~~~~~~~~{.sql}
# in MySQL command-line interface
# use specific database
USE partons;
~~~~~~~~~~~~~
The password for both _partons_ and _root_ MySQL users is _partons_.

If you do not use Virtual Machine provided by our team, but you want to use database-related functionalities in PARTONS, you need to get an access to MySQL server by your own. Possibly, it can be granted by your home institute, which is useful, if you intend to use the same database together with your collaborators. You can also install MySQL server at your local machine, which nowadays is really easy. E.g. for Debian-like Linux distributions you can proceed in the following way:
~~~~~~~~~~~~~{.sh}
# use repositories to install MySQL server 
sudo apt-get install mysql-server 

# run this script
sudo mysql_secure_installation
~~~~~~~~~~~~~
During the installation process or when executing `mysql_secure_installation` script you will be asked to set your own administration password to manage your server. For other Linux distributions and operating systems some adjustments to the installation receipt may be needed.

With MySQL server available, its administrator (possibly you) should set up PARTONS database. This can be accomplished easily with MySQL scripts provided by our team. These scripts are available in _database/sql_schema_ directory of your PARTONS copy and they should be used in the following way:
~~~~~~~~~~~~~{.sh}
# in shell 
# go to directory containing scripts 
cd database/sql_schema

# use MySQL command-line tool (it will ask you for administrator password, possibly the one set during MySQL server installation)
mysql -u root -p
# on some systems you may need to run this command as root 
sudo mysql -u root -p
~~~~~~~~~~~~~
~~~~~~~~~~~~~{.sql}
/* in MySQL command-line interface */
/* create new database */
CREATE DATABASE partons;

/* create new user whom you should use for all kind of non-administrative activities */
CREATE USER 'partons'@'localhost' IDENTIFIED BY 'partons';
GRANT SELECT, INSERT, UPDATE, DELETE, FILE ON *.* TO 'partons'@'localhost';

/* use database */ 
USE partons;

/* load scripts */
SOURCE mysql5_common.sql;
SOURCE mysql5_gpd_layer.sql;
SOURCE mysql5_convol_coeff_function_layer.sql;
SOURCE mysql5_observable_layer.sql;
~~~~~~~~~~~~~
Here, we have set the name of database and the name of user and his password as `partons`. Note however, that you may modify these parameters freely - you just need to set them correctly in the PARTONS configuration to enable the connection.  

# Connect PARTONS to your database # {#database_connection}

The connection parameters should be set in [the main configuration file of PARTONS](@ref config), i.e. the one called _partons.properties_. Analyze these examples that illustrate the configuration used to establish the connection to either a local (your computer) and or a remote MySQL server.

Connection to a local server:
~~~~~~~~~~~~~{.sh}
database.production.type = MYSQL
database.production.url = localhost
database.production.dbname = your_partons_database_name
database.production.user = your_sql_user_name 
database.production.passwd = your_sql_user_password 
~~~~~~~~~~~~~

Connection to a remote server:
~~~~~~~~~~~~~{.sh}
database.production.type = MYSQL
database.production.url = remote.address.example.com 
database.production.dbname = your_partons_database_name
database.production.user = your_sql_user_name
database.production.passwd = your_sql_user_password
~~~~~~~~~~~~~

# Database design # {#database_design}

The database design reflects the layered structure used in PARTONS, with tables corresponding to C++ objects and rows corresponding to appliances of those objects. There exist also tables storing general information on performed computations. The database design is explained here separately for the common tables and each of the layers by a single UML graph: 

* [database design of tables used by all layers](@ref database_design_1) 
* [database design of gpd layer](@ref database_design_2)
* [database design of cff layer](@ref database_design_3)
* [database design of observable layer](@ref database_design_4)

# PARTONS database interface # {#database_interface}

PARTONS database interface is provided by Services. Services are developed in order to either store or retrieve complex C++ objects from database tables in a simple and generic way. They are designed in a way that allows to perform SQL transactions by users who do not know SQL query language and the database design. Services use the Qt management of connectors, which are the third-party libraries allowing transactions with specific database types. The involvement of Qt makes possible to send the same SQL request to databases of different types (MySQL, SQLite, ...). With transaction and rollback mechanisms, Services ensure the integrity of database being currently used - if something wrong happens, the database always stays in a stable state. In addition, Services improve querying speed by using transaction and commit mechanisms for a large number of simultaneous queries.

In [XML interface](@ref usage) users may ignore the existence of Services - they are used implicitly. If a given computation is done via XML interface the results can be stored in the database together with the associated scenario, i.e. the input XML file. It allows either to track back all hypothesis underlying the results, or to be able to evaluate them again later. With this feature, it becomes easy to see how something was computed even if we ask ourselves a long time after.

This example illustrates how to store a single GPD result into the database via XML interface (note _storeInDB="1"_ option being crucial here):

~~~~~~~~~~~~~{.xml}
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
~~~~~~~~~~~~~

In [C++ interface](@ref usage) users use Services explicitly. Each Service may be seen as a bridge between a single physics-related object and a corresponding SQL table (see [Database design](#database_design) section). Services are listed in the following table together with the aforementioned correspondence:
| Service | Corresponding C++ object | Corresponding SQL table name |
| :---- | :---- | :---- |
| GPDKinematicDaoService | GPDKinematic | [gpd_kinematic](md_partons_database_design_2.html) |
| GPDResultDaoService | GPDResult | [gpd_result](md_partons_database_design_2.html) |
||||
| ConvolCoeffFunctionKinematicDaoService | ConvolCoeffFunctionKinematic | [cff_kinematic](md_partons_database_design_3.html) |
| ConvolCoeffFunctionResultDaoService | ConvolCoeffFunctionResult | [cff_result](md_partons_database_design_3.html) |
||||
| ObservableKinematicDaoService | ObservableKinematic | [observable_kienamtic](md_partons_database_design_4.html) |
| ObservableResultDaoService | ObservableResult | [observable_result](md_partons_database_design_4.html) |

This example illustrates how to use Services via C++ interface:
~~~~~~~~~~~~~{.cpp}
//evaluate exemplary GPD result to be inserted in database

//retrieve GPD service
GPDService* pGPDService = Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

//load GPD module with the BaseModuleFactory
GPDModule* pGPDModel = Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MMS13Model::classId);

//define GPD kinematics used in computation
GPDKinematic gpdKinematic(-0.1, 0.05, 0., 2., 2.);

//define list of GPD types to be computed
List<GPDType> gpdTypeList;
gpdTypeList.add(GPDType::ALL);

//evaluate
GPDResult gpdResult = pGPDService->computeGPDModel(gpdKinematic, pGPDModel, gpdTypeList);

//get GPDResultDaoService
GPDResultDaoService gpdResultDaoService;

//insert result into database
int id = gpdResultDaoService.insert(gpdResult);
~~~~~~~~~~~~~
For the list of all possible operations provided by Services see their documentation. Note, that the documentation contains a number of additional examples. 

# Store experimental data in database # {#database_experimentaldata}

The design of PARTONS database allows to store experimental data. Not only kinematics and results with uncertainties, but also information concerning related experiments can be stored in the database. These information can be used latter to make systematic comparisons with theoretical predictions, where experimental data are easily selected with a list of user-defined criteria. To introduce a new set of experimental data into the database, one can use scripts provided by our team, to be found in _database/insert_exp_data_ folder of PARTONS copy. We refer to _database/insert_exp_data/README_ file for more information.

