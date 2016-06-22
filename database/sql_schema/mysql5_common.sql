/* MYSQL syntax */

CREATE USER 'partons'@'localhost' IDENTIFIED BY 'partons';
GRANT SELECT, INSERT, UPDATE, DELETE ON *.* TO 'partons'@'localhost';

CREATE TABLE scenario (
scenario_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
store_date TIMESTAMP,
description VARCHAR(255),
xml_file BLOB,
hash_sum VARCHAR(40));

CREATE TABLE environment_configuration (
environment_configuration_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
store_date TIMESTAMP,
configuration VARCHAR(255),
hash_sum VARCHAR(40));

CREATE TABLE computation (
computation_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY, 
computation_date TIMESTAMP,
environment_configuration_id INTEGER NOT NULL /* Date and Time Datatype : TEXT as ISO8601 strings ("YYYY-MM-DD HH:MM:SS.SSS") */);

CREATE TABLE scenario_computation (
scenario_computation_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
scenario_task_index_number INTEGER,
scenario_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);

CREATE TABLE gpd_type (
gpd_type_id INTEGER NOT NULL PRIMARY KEY,
short_name VARCHAR(10),
long_name VARCHAR(255));

CREATE TABLE quark_flavor (
quark_flavor_id INTEGER NOT NULL PRIMARY KEY,
short_name VARCHAR(10),
long_name VARCHAR(255));

CREATE TABLE observable_type (
observable_type_id INTEGER NOT NULL PRIMARY KEY,
short_name VARCHAR(10),
long_name VARCHAR(255));



INSERT INTO gpd_type (gpd_type_id, short_name, long_name)
VALUES ('0', 'UNDEFINED', 'UNDEFINED');

INSERT INTO gpd_type (gpd_type_id, short_name, long_name)
VALUES ('1', 'ALL', 'ALL');

INSERT INTO gpd_type (gpd_type_id, short_name, long_name)
VALUES ('2', 'H', 'H');

INSERT INTO gpd_type (gpd_type_id, short_name, long_name)
VALUES ('3', 'E', 'E');

INSERT INTO gpd_type (gpd_type_id, short_name, long_name)
VALUES ('4', 'Ht', 'Ht');

INSERT INTO gpd_type (gpd_type_id, short_name, long_name)
VALUES ('5', 'Et', 'Et');



INSERT INTO quark_flavor (quark_flavor_id, short_name, long_name)
VALUES ('0', 'UNDEFINED', 'UNDEFINED');

INSERT INTO quark_flavor (quark_flavor_id, short_name, long_name)
VALUES ('1', 'u', 'UP');

INSERT INTO quark_flavor (quark_flavor_id, short_name, long_name)
VALUES ('2', 'd', 'DOWN');

INSERT INTO quark_flavor (quark_flavor_id, short_name, long_name)
VALUES ('3', 's', 'STRANGE');

INSERT INTO quark_flavor (quark_flavor_id, short_name, long_name)
VALUES ('4', 'c', 'CHARM');

INSERT INTO quark_flavor (quark_flavor_id, short_name, long_name)
VALUES ('5', 'b', 'BOTTOM');

INSERT INTO quark_flavor (quark_flavor_id, short_name, long_name)
VALUES ('6', 't', 'TOP');



INSERT INTO observable_type (observable_type_id, short_name, long_name)
VALUES ('0', 'UNDEFINED', 'UNDEFINED');

INSERT INTO observable_type (observable_type_id, short_name, long_name)
VALUES ('1', 'PHI', 'PHI');

INSERT INTO observable_type (observable_type_id, short_name, long_name)
VALUES ('2', 'FOURIER', 'FOURIER');

