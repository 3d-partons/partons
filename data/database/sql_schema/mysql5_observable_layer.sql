/* MySQL syntax */

/* experiments */

CREATE TABLE laboratory (
laboratory_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, 
laboratory_name VARCHAR(255));

CREATE TABLE collaboration (
collaboration_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, 
collaboration_name VARCHAR(255),
laboratory_id INTEGER);

CREATE TABLE experiment (
experiment_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, 
experiment_process VARCHAR(255), 
experiment_type VARCHAR(255), 
experiment_year INTEGER(4), 
experiment_reference VARCHAR(255),
collaboration_id INTEGER);

/* kinematics */

CREATE TABLE dvcs_observable_kinematic (
dvcs_observable_kinematic_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
xB DOUBLE NOT NULL, 
xB_unit INTEGER NOT NULL,
t DOUBLE NOT NULL, 
t_unit INTEGER NOT NULL,
Q2 DOUBLE NOT NULL, 
Q2_unit INTEGER NOT NULL,
E DOUBLE NOT NULL, 
E_unit INTEGER NOT NULL,
phi DOUBLE NOT NULL,
phi_unit INTEGER NOT NULL,
experiment_id INTEGER,
hash_sum VARCHAR(40) NOT NULL);
CREATE INDEX dvcs_observable_kinematic_index ON dvcs_observable_kinematic (hash_sum);

CREATE TABLE tcs_observable_kinematic (
tcs_observable_kinematic_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
xB DOUBLE NOT NULL, 
xB_unit INTEGER NOT NULL,
t DOUBLE NOT NULL, 
t_unit INTEGER NOT NULL,
Q2Prim DOUBLE NOT NULL, 
Q2Prim_unit INTEGER NOT NULL,
E DOUBLE NOT NULL, 
E_unit INTEGER NOT NULL,
phi DOUBLE NOT NULL,
phi_unit INTEGER NOT NULL,
experiment_id INTEGER,
hash_sum VARCHAR(40) NOT NULL);
CREATE INDEX tcs_observable_kinematic_index ON tcs_observable_kinematic (hash_sum);

/* result */

CREATE TABLE dvcs_observable_result (
dvcs_observable_result_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,  
observable_name VARCHAR(255) NOT NULL, 
observable_value DOUBLE NOT NULL, 
stat_error_lb DOUBLE,
stat_error_ub DOUBLE,
syst_error_lb DOUBLE,
syst_error_ub DOUBLE,
scale_error_lb DOUBLE,
scale_error_ub DOUBLE,
unit INTEGER NOT NULL,
dvcs_observable_kinematic_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);
CREATE INDEX dvcs_observable_result_index ON dvcs_observable_result (computation_id);

CREATE TABLE tcs_observable_result (
tcs_observable_result_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,  
observable_name VARCHAR(255) NOT NULL, 
observable_value DOUBLE NOT NULL, 
stat_error_lb DOUBLE,
stat_error_ub DOUBLE,
syst_error_lb DOUBLE,
syst_error_ub DOUBLE,
scale_error_lb DOUBLE,
scale_error_ub DOUBLE,
unit INTEGER NOT NULL,
tcs_observable_kinematic_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);
CREATE INDEX tcs_observable_result_index ON tcs_observable_result (computation_id);

/* view */

CREATE VIEW dvcs_observable_kinematic_view AS 
SELECT obk.xB, obk.t, obk.Q2, obk.E, obk.phi, obk.observable_kinematic_id, obr.computation_id
FROM dvcs_observable_result obr
INNER JOIN dvcs_observable_kinematic obk ON obr.dvcs_observable_kinematic_id = obk.dvcs_observable_kinematic_id
INNER JOIN computation c ON obr.computation_id = c.computation_id
ORDER BY obr.observable_result_id;

CREATE VIEW tcs_observable_kinematic_view AS 
SELECT obk.xB, obk.t, obk.Q2Prim, obk.E, obk.phi, obk.observable_kinematic_id, obr.computation_id
FROM tcs_observable_result obr
INNER JOIN tcs_observable_kinematic obk ON obr.tcs_observable_kinematic_id = obk.tcs_observable_kinematic_id
INNER JOIN computation c ON obr.computation_id = c.computation_id
ORDER BY obr.observable_result_id;

/* view for plots */

/*
CREATE VIEW observable_plot_2d_view AS
SELECT obr.computation_id, obk.observable_kinematic_id, obk.xB, obk.t, obk.Q2, obk.E, obk.phi, obr.observable_result_id, obr.computation_module_name, obr.observable_name, obr.observable_value
FROM observable_kinematic obk
INNER JOIN observable_result obr ON obr.observable_kinematic_id = obk.observable_kinematic_id
ORDER BY obr.observable_result_id;
*/
