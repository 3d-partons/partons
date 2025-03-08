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

CREATE TABLE dvmp_observable_kinematic (
dvmp_observable_kinematic_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
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
meson_type_id INTEGER NOT NULL,
experiment_id INTEGER,
hash_sum VARCHAR(40) NOT NULL);
CREATE INDEX dvmp_observable_kinematic_index ON dvmp_observable_kinematic (hash_sum);

CREATE TABLE tcs_observable_kinematic (
tcs_observable_kinematic_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
t DOUBLE NOT NULL, 
t_unit INTEGER NOT NULL,
Q2Prim DOUBLE NOT NULL, 
Q2Prim_unit INTEGER NOT NULL,
E DOUBLE NOT NULL, 
E_unit INTEGER NOT NULL,
phi DOUBLE NOT NULL,
phi_unit INTEGER NOT NULL,
theta DOUBLE NOT NULL,
theta_unit INTEGER NOT NULL,
experiment_id INTEGER,
hash_sum VARCHAR(40) NOT NULL);
CREATE INDEX tcs_observable_kinematic_index ON tcs_observable_kinematic (hash_sum);

CREATE TABLE ddvcs_observable_kinematic (
ddvcs_observable_kinematic_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
xB DOUBLE NOT NULL, 
xB_unit INTEGER NOT NULL,
t DOUBLE NOT NULL, 
t_unit INTEGER NOT NULL,
Q2 DOUBLE NOT NULL, 
Q2_unit INTEGER NOT NULL,
Q2Prim DOUBLE NOT NULL, 
Q2Prim_unit INTEGER NOT NULL,
E DOUBLE NOT NULL, 
E_unit INTEGER NOT NULL,
phi DOUBLE NOT NULL,
phi_unit INTEGER NOT NULL,
phiL DOUBLE NOT NULL,
phiL_unit INTEGER NOT NULL,
thetaL DOUBLE NOT NULL,
thetaL_unit INTEGER NOT NULL,
experiment_id INTEGER,
hash_sum VARCHAR(40) NOT NULL);
CREATE INDEX ddvcs_observable_kinematic_index ON ddvcs_observable_kinematic (hash_sum);

/* result */

CREATE TABLE dvcs_observable_result (
dvcs_observable_result_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,  
observable_name VARCHAR(255) NOT NULL, 
value DOUBLE NOT NULL, 
stat_error_lb DOUBLE,
stat_error_ub DOUBLE,
syst_error_lb DOUBLE,
syst_error_ub DOUBLE,
scale_error_lb DOUBLE,
scale_error_ub DOUBLE,
value_unit INTEGER NOT NULL,
dvcs_observable_kinematic_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);
CREATE INDEX dvcs_observable_result_index ON dvcs_observable_result (computation_id);

CREATE TABLE dvmp_observable_result (
dvmp_observable_result_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,  
observable_name VARCHAR(255) NOT NULL, 
value DOUBLE NOT NULL, 
stat_error_lb DOUBLE,
stat_error_ub DOUBLE,
syst_error_lb DOUBLE,
syst_error_ub DOUBLE,
scale_error_lb DOUBLE,
scale_error_ub DOUBLE,
value_unit INTEGER NOT NULL,
dvmp_observable_kinematic_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);
CREATE INDEX dvmp_observable_result_index ON dvmp_observable_result (computation_id);

CREATE TABLE tcs_observable_result (
tcs_observable_result_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,  
observable_name VARCHAR(255) NOT NULL, 
value DOUBLE NOT NULL, 
stat_error_lb DOUBLE,
stat_error_ub DOUBLE,
syst_error_lb DOUBLE,
syst_error_ub DOUBLE,
scale_error_lb DOUBLE,
scale_error_ub DOUBLE,
value_unit INTEGER NOT NULL,
tcs_observable_kinematic_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);
CREATE INDEX tcs_observable_result_index ON tcs_observable_result (computation_id);

CREATE TABLE ddvcs_observable_result (
ddvcs_observable_result_id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,  
observable_name VARCHAR(255) NOT NULL, 
value DOUBLE NOT NULL, 
stat_error_lb DOUBLE,
stat_error_ub DOUBLE,
syst_error_lb DOUBLE,
syst_error_ub DOUBLE,
scale_error_lb DOUBLE,
scale_error_ub DOUBLE,
value_unit INTEGER NOT NULL,
ddvcs_observable_kinematic_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);
CREATE INDEX ddvcs_observable_result_index ON ddvcs_observable_result (computation_id);

/* view */

CREATE VIEW dvcs_observable_kinematic_view AS 
SELECT 	obk.dvcs_observable_kinematic_id, obk.xB, obk.xB_unit, obk.t, obk.t_unit, obk.Q2, obk.Q2_unit, obk.E, obk.E_unit, obk.phi, obk.phi_unit, obk.hash_sum
FROM dvcs_observable_kinematic obk
ORDER BY obk.dvcs_observable_kinematic_id;

CREATE VIEW dvmp_observable_kinematic_view AS 
SELECT 	obk.dvmp_observable_kinematic_id, obk.xB, obk.xB_unit, obk.t, obk.t_unit, obk.Q2, obk.Q2_unit, obk.E, obk.E_unit, obk.phi, obk.phi_unit, obk.meson_type_id, obk.hash_sum
FROM dvmp_observable_kinematic obk
ORDER BY obk.dvmp_observable_kinematic_id;

CREATE VIEW tcs_observable_kinematic_view AS 
SELECT 	obk.tcs_observable_kinematic_id, obk.t, obk.t_unit, obk.Q2Prim, obk.Q2Prim_unit, obk.E, obk.E_unit, obk.phi, obk.phi_unit, obk.theta, obk.theta_unit, obk.hash_sum
FROM tcs_observable_kinematic obk
ORDER BY obk.tcs_observable_kinematic_id;

CREATE VIEW ddvcs_observable_kinematic_view AS 
SELECT 	obk.ddvcs_observable_kinematic_id, obk.xB, obk.xB_unit, obk.t, obk.t_unit, obk.Q2, obk.Q2_unit, obk.Q2Prim, obk.Q2Prim_unit, obk.E, obk.E_unit, obk.phi, obk.phi_unit, obk.phiL, obk.phiL_unit, obk.thetaL, obk.thetaL_unit, obk.hash_sum
FROM ddvcs_observable_kinematic obk
ORDER BY obk.ddvcs_observable_kinematic_id;

/* view for plots */

CREATE VIEW dvcs_observable_plot_2d_view AS
SELECT	obr.computation_id, obk.dvcs_observable_kinematic_id, 
	obk.xB, obk.xB_unit,
	obk.t, obk.t_unit,
	obk.Q2, obk.Q2_unit, 
	obk.E, obk.E_unit, 
	obk.phi, obk.phi_unit,
	obr.dvcs_observable_result_id, obr.observable_name, 
	obr.value, obr.value_unit
FROM dvcs_observable_result obr
INNER JOIN dvcs_observable_kinematic obk ON obr.dvcs_observable_kinematic_id = obk.dvcs_observable_kinematic_id
ORDER BY obr.dvcs_observable_result_id;

CREATE VIEW dvmp_observable_plot_2d_view AS
SELECT	obr.computation_id, obk.dvmp_observable_kinematic_id, 
	obk.xB, obk.xB_unit,
	obk.t, obk.t_unit,
	obk.Q2, obk.Q2_unit, 
	obk.E, obk.E_unit, 
	obk.phi, obk.phi_unit,
	mt.meson_type_short_name,
	obr.dvmp_observable_result_id, obr.observable_name, 
	obr.value, obr.value_unit
FROM dvmp_observable_result obr
INNER JOIN dvmp_observable_kinematic obk ON obr.dvmp_observable_kinematic_id = obk.dvmp_observable_kinematic_id
INNER JOIN meson_type mt ON obk.meson_type_id = mt.meson_type_id
ORDER BY obr.dvmp_observable_result_id;

CREATE VIEW tcs_observable_plot_2d_view AS
SELECT	obr.computation_id, obk.tcs_observable_kinematic_id, 
	obk.t, obk.t_unit,
	obk.Q2Prim, obk.Q2Prim_unit, 
	obk.E, obk.E_unit, 
	obk.phi, obk.phi_unit,
	obk.theta, obk.theta_unit,
	obr.tcs_observable_result_id, obr.observable_name, 
	obr.value, obr.value_unit
FROM tcs_observable_result obr
INNER JOIN tcs_observable_kinematic obk ON obr.tcs_observable_kinematic_id = obk.tcs_observable_kinematic_id
ORDER BY obr.tcs_observable_result_id;

CREATE VIEW ddvcs_observable_plot_2d_view AS
SELECT	obr.computation_id, obk.ddvcs_observable_kinematic_id, 
	obk.xB, obk.xB_unit,
	obk.t, obk.t_unit,
	obk.Q2, obk.Q2_unit, 
	obk.Q2Prim, obk.Q2Prim_unit, 
	obk.E, obk.E_unit, 
	obk.phi, obk.phi_unit,
	obk.phiL, obk.phiL_unit,
	obk.thetaL, obk.thetaL_unit,
	obr.ddvcs_observable_result_id, obr.observable_name, 
	obr.value, obr.value_unit
FROM ddvcs_observable_result obr
INNER JOIN ddvcs_observable_kinematic obk ON obr.ddvcs_observable_kinematic_id = obk.ddvcs_observable_kinematic_id
ORDER BY obr.ddvcs_observable_result_id;
