/*
ATTENTION: Ne supporte pas les tabulation
*/

CREATE TABLE quark_flavor (id INTEGER NOT NULL PRIMARY KEY, short_name VARCHAR(10), long_name VARCHAR(255), UNIQUE(short_name));

CREATE TABLE gpd_type (	id INTEGER NOT NULL PRIMARY KEY, short_name VARCHAR(10), long_name VARCHAR(255), UNIQUE(short_name));

CREATE TABLE kinematic_type (id INTEGER NOT NULL PRIMARY KEY, short_name VARCHAR(10), long_name VARCHAR(255), UNIQUE(short_name));

CREATE TABLE module (id INTEGER NOT NULL PRIMARY KEY, class_name VARCHAR(255),description VARCHAR(255), UNIQUE(class_name));

/* SQLITE syntax */

CREATE TABLE gpd_kinematic (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, kinematic_type_id INTEGER, x DOUBLE, xi DOUBLE, t DOUBLE, MuF2 DOUBLE, MuR2 DOUBLE, UNIQUE(x, xi, t, MuF2, MuR2));

CREATE TABLE parton_distribution (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, gpd_result_id INTEGER, gpd_type_id INTEGER,gluon_distribution DOUBLE);

CREATE TABLE quark_distribution (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, parton_distribution_id INTEGER, quark_flavor_id INTEGER,quark_distribution DOUBLE,quark_distribution_minus DOUBLE,quark_distribution_plus DOUBLE);

CREATE TABLE gpd_result (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, module_configuration_id INTEGER, gpd_kinematic_id INTEGER);

CREATE TABLE module_configuration (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, module_id INTEGER, parameters VARCHAR(255), UNIQUE(module_id, parameters));

/* MYSQL syntax */ 

CREATE TABLE gpd_kinematic (id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, kinematic_type_id INTEGER, x DOUBLE, xi DOUBLE, t DOUBLE, MuF2 DOUBLE, MuR2 DOUBLE, UNIQUE(x, xi, t, MuF2, MuR2));

CREATE TABLE parton_distribution (id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, gpd_result_id INTEGER, gpd_type_id INTEGER,gluon_distribution DOUBLE);

CREATE TABLE quark_distribution (id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, parton_distribution_id INTEGER, quark_flavor_id INTEGER,quark_distribution DOUBLE,quark_distribution_minus DOUBLE,quark_distribution_plus DOUBLE);

CREATE TABLE gpd_result (id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, module_configuration_id INTEGER, gpd_kinematic_id INTEGER);

CREATE TABLE module_configuration (id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, module_id INTEGER, parameters VARCHAR(255), UNIQUE(module_id, parameters));


DROP TABLE quark_flavor;
DROP TABLE gpd_type;
DROP TABLE kinematic_type;
DROP TABLE module;

DROP TABLE gpd_kinematic;
DROP TABLE parton_distribution;
DROP TABLE quark_distribution;
DROP TABLE gpd_result;
DROP TABLE module_configuration;

/* Fill database */ 

INSERT INTO quark_flavor (id, short_name, long_name) VALUES ('0', 'UNDEFINED', 'UNDEFINED');
INSERT INTO quark_flavor (id, short_name, long_name) VALUES ('1', 'u', 'UP');
INSERT INTO quark_flavor (id, short_name, long_name) VALUES ('2', 'd', 'DOWN');
INSERT INTO quark_flavor (id, short_name, long_name) VALUES ('3', 's', 'STRANGE');
INSERT INTO quark_flavor (id, short_name, long_name) VALUES ('4', 'c', 'CHARM');
INSERT INTO quark_flavor (id, short_name, long_name) VALUES ('5', 'b', 'BOTTOM');
INSERT INTO quark_flavor (id, short_name, long_name) VALUES ('6', 't', 'TOP');

INSERT INTO kinematic_type (id, short_name, long_name) VALUES ('0', 'UNDEFINED', 'UNDEFINED');
INSERT INTO kinematic_type (id, short_name, long_name) VALUES ('1', 'theo', 'theoretical');
INSERT INTO kinematic_type (id, short_name, long_name) VALUES ('2', 'exp', 'experimental');

INSERT INTO gpd_type (id, short_name, long_name) VALUES ('0', 'UNDEFINED', 'UNDEFINED');
INSERT INTO gpd_type (id, short_name, long_name) VALUES ('2', 'H', 'H');
INSERT INTO gpd_type (id, short_name, long_name) VALUES ('3', 'E', 'E');
INSERT INTO gpd_type (id, short_name, long_name) VALUES ('4', 'Ht', 'Ht');
INSERT INTO gpd_type (id, short_name, long_name) VALUES ('5', 'Et', 'Et');

INSERT INTO module (id, class_name, description) VALUES ('1', 'GK11Model', 'UNDEFINED');

INSERT INTO module_configuration (id, module_id, parameters) VALUES ('1', '1', 'DEFAULT');

