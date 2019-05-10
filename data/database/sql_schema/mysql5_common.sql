/* MYSQL syntax */

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

CREATE TABLE observable_channel (
observable_channel_id INTEGER NOT NULL PRIMARY KEY,
observable_channel_short_name VARCHAR(10),
observable_channel_long_name VARCHAR(255));

CREATE TABLE physical_unit_type (
physical_unit_type_id INTEGER NOT NULL PRIMARY KEY,
physical_unit_type_short_name VARCHAR(10),
physical_unit_type_long_name VARCHAR(255));

CREATE VIEW result_info_view AS
SELECT c.computation_id, c.computation_date, sc.scenario_task_index_number, ec.env_conf_hash_sum, s.scenario_hash_sum
FROM computation c
INNER JOIN environment_configuration ec ON ec.env_conf_id = c.environment_configuration_id
INNER JOIN scenario_computation sc ON sc.computation_id = c.computation_id
INNER JOIN scenario s ON sc.scenario_id = s.scenario_id
ORDER BY c.computation_id;

/* === gpd_type === */

INSERT INTO gpd_type (gpd_type_id, gpd_type_short_name, gpd_type_long_name)
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

/* === quark_flavor === */

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

/* === observable_type === */

INSERT INTO observable_type (observable_type_id, observable_type_short_name, observable_type_long_name)
VALUES ('0', 'UNDEFINED', 'UNDEFINED');

INSERT INTO observable_type (observable_type_id, observable_type_short_name, observable_type_long_name)
VALUES ('1', 'PHI', 'PHI');

INSERT INTO observable_type (observable_type_id, observable_type_short_name, observable_type_long_name)
VALUES ('2', 'FOURIER', 'FOURIER');


/* === observable_channel === */

INSERT INTO observable_channel (observable_channel_id, observable_channel_short_name, observable_channel_long_name)
VALUES ('0', 'UNDEFINED', 'UNDEFINED');

INSERT INTO observable_channel (observable_channel_id, observable_channel_short_name, observable_channel_long_name)
VALUES ('1', 'DVCS', 'DVCS');

INSERT INTO observable_channel (observable_channel_id, observable_channel_short_name, observable_channel_long_name)
VALUES ('2', 'DVMP', 'DVMP');

INSERT INTO observable_channel (observable_channel_id, observable_channel_short_name, observable_channel_long_name)
VALUES ('3', 'TCS', 'TCS');

/* === physical_unit === */

/* undefined */
INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('0', 'UNDEFINED', 'UNDEFINED');

/* none */
INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('1', 'none', 'NONE');

/* energy, mass or momentum */
INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('2', 'eV', 'EV');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('3', 'keV', 'KEV');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('4', 'MeV', 'MEV');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('5', 'GeV', 'GEV');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('6', 'TeV', 'TEV');

/* energy, mass or momentum squared */
INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('7', 'eV2', 'EV2');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('8', 'keV2', 'KEV2');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('9', 'MeV2', 'MEV2');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('10', 'GeV2', 'GEV2');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('11', 'TeV2', 'TEV2');

/* distance or time*/
INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('12', 'eV-1', 'EVm1');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('13', 'keV-1', 'KEVm1');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('14', 'MeV-1', 'MEVm1');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('15', 'GeV-1', 'GEVm1');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('16', 'TeV-1', 'TEVm1');


INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('17', 'fm', 'FM');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('18', 'pm', 'PM');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('19', 'nm', 'NM');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('20', 'um', 'UM');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('21', 'mm', 'MM');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('22', 'm', 'M');


INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('23', 'fs', 'FS');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('24', 'ps', 'PS');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('25', 'ns', 'NS');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('26', 'us', 'US');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('27', 'ms', 'MS');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('28', 's', 'S');

/* cross section*/

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('29', 'GeVm2', 'GEVm2');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('30', 'fm2', 'FM2');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('31', 'fb', 'FB');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('32', 'pb', 'PB');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('33', 'nb', 'NB');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('34', 'ub', 'UB');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('35', 'mb', 'MB');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('36', 'b', 'B');

/* angle */
INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('37', 'deg', 'DEG');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('38', 'rad', 'RAD');

INSERT INTO physical_unit_type (physical_unit_type_id, physical_unit_type_short_name, physical_unit_type_long_name)
VALUES ('39', 'mrad', 'MRAD');

