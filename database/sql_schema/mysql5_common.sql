/* MYSQL syntax */

/* FILE privilege is to be able to execute LOAD DATA INFILE query */
CREATE USER 'partons'@'localhost' IDENTIFIED BY 'partons';
GRANT SELECT, INSERT, UPDATE, DELETE, FILE ON *.* TO 'partons'@'localhost';



CREATE TABLE scenario (
scenario_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
scenario_store_date TIMESTAMP,
scenario_description VARCHAR(255),
scenario_xml_file BLOB,
scenario_hash_sum VARCHAR(40));

CREATE TABLE environment_configuration (
env_conf_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
env_conf_store_date TIMESTAMP,
env_conf_configuration VARCHAR(255),
env_conf_hash_sum VARCHAR(40));

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
gpd_type_short_name VARCHAR(10),
gpd_type_long_name VARCHAR(255));

CREATE TABLE quark_flavor (
quark_flavor_id INTEGER NOT NULL PRIMARY KEY,
quark_flavor_short_name VARCHAR(10),
quark_flavor_long_name VARCHAR(255));

CREATE TABLE observable_type (
observable_type_id INTEGER NOT NULL PRIMARY KEY,
observable_type_short_name VARCHAR(10),
observable_type_long_name VARCHAR(255));



CREATE VIEW result_info_view AS
SELECT c.computation_id, c.computation_date, sc.scenario_task_index_number, ec.env_conf_hash_sum, s.scenario_hash_sum
FROM computation c
INNER JOIN environment_configuration ec ON ec.environment_configuration_id = c.environment_configuration_id
INNER JOIN scenario_computation sc ON sc.computation_id = c.computation_id
INNER JOIN scenario s ON sc.scenario_id = s.scenario_id
ORDER BY c.computation_id;



INSERT INTO gpd_type (gpd_type_id, short_name, gpd_type_long_name)
VALUES ('0', 'UNDEFINED', 'UNDEFINED');

INSERT INTO gpd_type (gpd_type_id, gpd_type_short_name, gpd_type_long_name)
VALUES ('1', 'ALL', 'ALL');

INSERT INTO gpd_type (gpd_type_id, gpd_type_short_name, gpd_type_long_name)
VALUES ('2', 'H', 'H');

INSERT INTO gpd_type (gpd_type_id, gpd_type_short_name, gpd_type_long_name)
VALUES ('3', 'E', 'E');

INSERT INTO gpd_type (gpd_type_id, gpd_type_short_name, gpd_type_long_name)
VALUES ('4', 'Ht', 'Ht');

INSERT INTO gpd_type (gpd_type_id, gpd_type_short_name, gpd_type_long_name)
VALUES ('5', 'Et', 'Et');



INSERT INTO quark_flavor (quark_flavor_id, quark_flavor_short_name, quark_flavor_long_name)
VALUES ('0', 'UNDEFINED', 'UNDEFINED');

INSERT INTO quark_flavor (quark_flavor_id, quark_flavor_short_name, quark_flavor_long_name)
VALUES ('1', 'u', 'UP');

INSERT INTO quark_flavor (quark_flavor_id, quark_flavor_short_name, quark_flavor_long_name)
VALUES ('2', 'd', 'DOWN');

INSERT INTO quark_flavor (quark_flavor_id, quark_flavor_short_name, quark_flavor_long_name)
VALUES ('3', 's', 'STRANGE');

INSERT INTO quark_flavor (quark_flavor_id, quark_flavor_short_name, quark_flavor_long_name)
VALUES ('4', 'c', 'CHARM');

INSERT INTO quark_flavor (quark_flavor_id, quark_flavor_short_name, quark_flavor_long_name)
VALUES ('5', 'b', 'BOTTOM');

INSERT INTO quark_flavor (quark_flavor_id, quark_flavor_short_name, quark_flavor_long_name)
VALUES ('6', 't', 'TOP');



INSERT INTO observable_type (observable_type_id, observable_type_short_name, observable_type_long_name)
VALUES ('0', 'UNDEFINED', 'UNDEFINED');

INSERT INTO observable_type (observable_type_id, observable_type_short_name, observable_type_long_name)
VALUES ('1', 'PHI', 'PHI');

INSERT INTO observable_type (observable_type_id, observable_type_short_name, observable_type_long_name)
VALUES ('2', 'FOURIER', 'FOURIER');
