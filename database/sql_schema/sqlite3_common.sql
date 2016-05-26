/* SQLITE syntax */

CREATE TABLE scenario (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
store_date TIMESTAMP,
description VARCHAR(255),
xml_file BLOB,
hash_sum VARCHAR(32));

CREATE TABLE environment_configuration (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
store_date TIMESTAMP,
configuration VARCHAR(255),
hash_sum VARCHAR(32));

CREATE TABLE computation (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, 
computation_date TIMESTAMP,
environment_configuration_id INTEGER NOT NULL); /* Date and Time Datatype : TEXT as ISO8601 strings ("YYYY-MM-DD HH:MM:SS.SSS") */

CREATE TABLE scenario_computation (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
scenario_task_index_number INTEGER,
scenario_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);

CREATE TABLE gpd_type (
id INTEGER NOT NULL PRIMARY KEY,
short_name VARCHAR(10),
long_name VARCHAR(255));

CREATE TABLE quark_flavor (
id INTEGER NOT NULL PRIMARY KEY,
short_name VARCHAR(10),
long_name VARCHAR(255));

CREATE TABLE observable_type (
id INTEGER NOT NULL PRIMARY KEY,
short_name VARCHAR(10),
long_name VARCHAR(255));



INSERT INTO gpd_type (id, short_name, long_name)
VALUES ('0', 'UNDEFINED', 'UNDEFINED');

INSERT INTO gpd_type (id, short_name, long_name)
VALUES ('1', 'ALL', 'ALL');

INSERT INTO gpd_type (id, short_name, long_name)
VALUES ('2', 'H', 'H');

INSERT INTO gpd_type (id, short_name, long_name)
VALUES ('3', 'E', 'E');

INSERT INTO gpd_type (id, short_name, long_name)
VALUES ('4', 'Ht', 'Ht');

INSERT INTO gpd_type (id, short_name, long_name)
VALUES ('5', 'Et', 'Et');



INSERT INTO quark_flavor (id, short_name, long_name)
VALUES ('0', 'UNDEFINED', 'UNDEFINED');

INSERT INTO quark_flavor (id, short_name, long_name)
VALUES ('1', 'u', 'UP');

INSERT INTO quark_flavor (id, short_name, long_name)
VALUES ('2', 'd', 'DOWN');

INSERT INTO quark_flavor (id, short_name, long_name)
VALUES ('3', 's', 'STRANGE');

INSERT INTO quark_flavor (id, short_name, long_name)
VALUES ('4', 'c', 'CHARM');

INSERT INTO quark_flavor (id, short_name, long_name)
VALUES ('5', 'b', 'BOTTOM');

INSERT INTO quark_flavor (id, short_name, long_name)
VALUES ('6', 't', 'TOP');



INSERT INTO observable_type (id, short_name, long_name)
VALUES ('0', 'UNDEFINED', 'UNDEFINED');

INSERT INTO observable_type (id, short_name, long_name)
VALUES ('1', 'PHI', 'PHI');

INSERT INTO observable_type (id, short_name, long_name)
VALUES ('2', 'FOURIER', 'FOURIER');
