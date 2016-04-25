/* SQLITE syntax */

CREATE TABLE laboratory (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, 
name VARCHAR(255));

CREATE TABLE collaboration (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, 
name VARCHAR(255),
laboratory_id INTEGER);

CREATE TABLE experiment (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, 
process VARCHAR(255), 
experiment_type VARCHAR(255), 
year INTEGER(4), 
reference VARCHAR(255),
collaboration_id INTEGER);

CREATE TABLE observable_kinematic (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
bin_id INTEGER NOT NULL, 
xB DOUBLE NOT NULL, 
t DOUBLE NOT NULL, 
Q2 DOUBLE NOT NULL, 
phi DOUBLE NOT NULL,
experiment_id INTEGER);

CREATE TABLE observable_result (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,  
observable_name VARCHAR(255) NOT NULL, 
observable_value DOUBLE NOT NULL, 
stat_error_lb DOUBLE,
stat_error_ub DOUBLE,
syst_error_lb DOUBLE,
syst_error_ub DOUBLE,
total_error DOUBLE,
computation_module_name VARCHAR(255) NOT NULL,
observable_type_id INTEGER NOT NULL,
observable_kinematic_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);



INSERT INTO laboratory (id, name) 
VALUES ('0', 'CERN');

INSERT INTO laboratory (id, name) 
VALUES ('1', 'DESY');

INSERT INTO laboratory (id, name) 
VALUES ('2', 'Jefferson Lab');



INSERT INTO collaboration (id, name, laboratory_id) 
VALUES ('0', 'COMPASS', 0);

INSERT INTO collaboration (id, name, laboratory_id) 
VALUES ('1', 'H1', 1);

INSERT INTO collaboration (id, name, laboratory_id) 
VALUES ('2', 'HERMES', 1);

INSERT INTO collaboration (id, name, laboratory_id) 
VALUES ('3', 'HERA', 1);

INSERT INTO collaboration (id, name, laboratory_id) 
VALUES ('4', 'ZEUS', 1);

INSERT INTO collaboration (id, name, laboratory_id) 
VALUES ('5', 'CLAS', 2);

INSERT INTO collaboration (id, name, laboratory_id) 
VALUES ('6', 'HALL A', 2);



INSERT INTO experiment (id, process, experiment_type, year, reference, collaboration_id) 
VALUES ('0', 'ep2epgamma', 'fixed target', '2006', 'nucl-ex/0607029 | private tables', '6');

/*
INSERT INTO observable_kinematic (id, bin_id, xB, t, Q2, phi) 
VALUES ('0', '0', '0.36', '-0.17', '2.3', '7.5');

INSERT INTO computation (id, date) 
VALUES ('0', '2006-01-01 00:00:00.000');

INSERT INTO observable_result (observable_name, observable_value, stat_error_lb, stat_error_ub, syst_error_lb, syst_error_ub, total_error, computation_module_name, observable_type_id, observable_kinematic_id, computation_id) 
VALUES ('BSS', '0.0907029', '0.00274594', NULL, '0.00507936', NULL, '0', 'UNDEFINED', '0', '0', '0');
*/