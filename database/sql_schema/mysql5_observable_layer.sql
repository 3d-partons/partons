/* MySQL syntax */

CREATE TABLE laboratory (
laboratory_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, 
laboratory_name VARCHAR(255));

CREATE TABLE collaboration (
collaboration_id INTEGER NOT NULL PRIMARY KEY, 
collaboration_name VARCHAR(255),
laboratory_id INTEGER);

CREATE TABLE experiment (
experiment_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, 
experiment_process VARCHAR(255), 
experiment_type VARCHAR(255), 
experiment_year INTEGER(4), 
experiment_reference VARCHAR(255),
collaboration_id INTEGER);

CREATE TABLE observable_kinematic (
observable_kinematic_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
bin_id INTEGER NOT NULL, 
xB DOUBLE NOT NULL, 
t DOUBLE NOT NULL, 
Q2 DOUBLE NOT NULL, 
phi DOUBLE NOT NULL,
experiment_id INTEGER);

CREATE TABLE observable_result (
observable_result_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,  
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


INSERT INTO collaboration (collaboration_id, collaboration_name, laboratory_id) 
VALUES ('0', 'COMPASS', 0);

INSERT INTO collaboration (collaboration_id, collaboration_name, laboratory_id) 
VALUES ('1', 'H1', 1);

INSERT INTO collaboration (collaboration_id, collaboration_name, laboratory_id) 
VALUES ('2', 'HERMES', 1);

INSERT INTO collaboration (collaboration_id, collaboration_name, laboratory_id) 
VALUES ('3', 'HERA', 1);

INSERT INTO collaboration (collaboration_id, collaboration_name, laboratory_id) 
VALUES ('4', 'ZEUS', 1);

INSERT INTO collaboration (collaboration_id, collaboration_name, laboratory_id) 
VALUES ('5', 'CLAS', 2);

INSERT INTO collaboration (collaboration_id, collaboration_name, laboratory_id) 
VALUES ('6', 'HALL A', 2);



CREATE VIEW observable_kinematic_view AS 
SELECT obr.computation_id, obk.observable_kinematic_id, obk.xB, obk.t, obk.Q2, obk.phi
FROM observable_result obr
INNER JOIN observable_kinematic obk ON obr.observable_kinematic_id = obk.observable_kinematic_id
INNER JOIN computation c ON obr.computation_id = c.computation_id
ORDER BY obr.observable_result_id;

/* Specific view to filter on Observable results to make a plot */
CREATE VIEW observable_plot_2d_view AS
SELECT obr.computation_id, obk.observable_kinematic_id, obk.xB, obk.t, obk.Q2, obk.phi, obr.observable_result_id, obr.computation_module_name, obr.observable_name, obr.observable_value
FROM observable_kinematic obk
INNER JOIN observable_result obr ON obr.observable_kinematic_id = obk.observable_kinematic_id
ORDER BY obr.observable_result_id;
/*
Output example :
+----------------+-------------------------+------+-------+------+-----+----------------------+-------------------------+-----------------+------------------+
| computation_id | observable_kinematic_id | xB   | t     | Q2   | phi | observable_result_id | computation_module_name | observable_name | observable_value |
+----------------+-------------------------+------+-------+------+-----+----------------------+-------------------------+-----------------+------------------+
|              1 |                       1 | 1.68 | 0.194 | 0.11 |  25 |                    1 | experimental data       | Aul             |             0.44 |
+----------------+-------------------------+------+-------+------+-----+----------------------+-------------------------+-----------------+------------------+
*/


