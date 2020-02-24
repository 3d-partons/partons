/* MYSQL syntax */

/* kinematics */

CREATE TABLE dvcs_ccf_kinematic (
dvcs_ccf_kinematic_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
xi DOUBLE NOT NULL,
xi_unit INTEGER NOT NULL,
t DOUBLE NOT NULL,
t_unit INTEGER NOT NULL,
Q2 DOUBLE NOT NULL,
Q2_unit INTEGER NOT NULL,
MuF2 DOUBLE NOT NULL,
MuF2_unit INTEGER NOT NULL,
MuR2 DOUBLE NOT NULL,
MuR2_unit INTEGER NOT NULL,
hash_sum VARCHAR(40) NOT NULL);
CREATE INDEX dvcs_ccf_kinematic_index ON dvcs_ccf_kinematic (hash_sum);

CREATE TABLE tcs_ccf_kinematic (
tcs_ccf_kinematic_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
xi DOUBLE NOT NULL,
xi_unit INTEGER NOT NULL,
t DOUBLE NOT NULL,
t_unit INTEGER NOT NULL,
Q2Prim DOUBLE NOT NULL,
Q2Prim_unit INTEGER NOT NULL,
MuF2 DOUBLE NOT NULL,
MuF2_unit INTEGER NOT NULL,
MuR2 DOUBLE NOT NULL,
MuR2_unit INTEGER NOT NULL,
hash_sum VARCHAR(40) NOT NULL);
CREATE INDEX tcs_ccf_kinematic_index ON tcs_ccf_kinematic (hash_sum);

/* result */

CREATE TABLE ccf_result (
ccf_result_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
computation_module_name VARCHAR(255) NOT NULL,
channel_id INTEGER NOT NULL,
ccf_kinematic_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);
CREATE INDEX ccf_result_index ON ccf_result (computation_id);

CREATE TABLE dvcs_ccf_result (
dvcs_ccf_result_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
gpd_type_id INTEGER NOT NULL,
real_part DOUBLE NOT NULL,
img_part DOUBLE NOT NULL,
ccf_result_id INTEGER NOT NULL);
CREATE INDEX dvcs_ccf_result_index ON dvcs_ccf_result (ccf_result_id);

CREATE TABLE tcs_ccf_result (
tcs_ccf_result_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
gpd_type_id INTEGER NOT NULL,
real_part DOUBLE NOT NULL,
img_part DOUBLE NOT NULL,
ccf_result_id INTEGER NOT NULL);
CREATE INDEX tcs_ccf_result_index ON tcs_ccf_result (ccf_result_id);

/* view */

CREATE VIEW dvcs_ccf_kinematic_view AS
SELECT ccfk.dvcs_ccf_kinematic_id, ccfk.xi, ccfk.xi_unit, ccfk.t, ccfk.t_unit, ccfk.Q2, ccfk.Q2_unit, ccfk.MuF2, ccfk.MuF2_unit, ccfk.MuR2, ccfk.MuR2_unit, ccfk.hash_sum
FROM dvcs_ccf_kinematic ccfk 
ORDER BY ccfk.dvcs_ccf_kinematic_id;

CREATE VIEW tcs_ccf_kinematic_view AS
SELECT ccfk.tcs_ccf_kinematic_id, ccfk.xi, ccfk.xi_unit, ccfk.t, ccfk.t_unit, ccfk.Q2Prim, ccfk.Q2Prim_unit, ccfk.MuF2, ccfk.MuF2_unit, ccfk.MuR2, ccfk.MuR2_unit, ccfk.hash_sum 
FROM tcs_ccf_kinematic ccfk 
ORDER BY ccfk.tcs_ccf_kinematic_id;

CREATE VIEW dvcs_ccf_result_view AS 
SELECT ccfr.ccf_result_id, ccfr.computation_module_name, ccfr.channel_id, ccfrc.gpd_type_id, ccfrc.real_part, ccfrc.img_part, ccfr.computation_id, ccfr.ccf_kinematic_id
FROM ccf_result ccfr
INNER JOIN dvcs_ccf_result ccfrc ON ccfr.ccf_result_id = ccfrc.ccf_result_id
ORDER BY ccfr.ccf_result_id;

CREATE VIEW tcs_ccf_result_view AS 
SELECT ccfr.ccf_result_id, ccfr.computation_module_name, ccfr.channel_id, ccfrc.gpd_type_id, ccfrc.real_part, ccfrc.img_part, ccfr.computation_id, ccfr.ccf_kinematic_id
FROM ccf_result ccfr
INNER JOIN tcs_ccf_result ccfrc ON ccfr.ccf_result_id = ccfrc.ccf_result_id
ORDER BY ccfr.ccf_result_id;

/* view for plots */

CREATE VIEW dvcs_ccf_plot_2d_view AS
SELECT 	ccfr.computation_id, ccfk.dvcs_ccf_kinematic_id, 
	ccfk.xi, ccfk.xi_unit,
	ccfk.t, ccfk.t_unit,
	ccfk.Q2, ccfk.Q2_unit,
	ccfk.MuF2, ccfk.MuF2_unit,
	ccfk.MuR2, ccfk.MuR2_unit,
	ccfr.ccf_result_id, ccfr.computation_module_name, gt.gpd_type_short_name, 
	ccfrc.real_part, '1' as 'real_part_unit',
	ccfrc.img_part, '1' as 'img_part_unit'
FROM dvcs_ccf_result ccfrc
INNER JOIN ccf_result ccfr ON ccfr.ccf_result_id = ccfrc.ccf_result_id
INNER JOIN dvcs_ccf_kinematic ccfk ON ccfk.dvcs_ccf_kinematic_id = ccfr.ccf_kinematic_id
INNER JOIN gpd_type gt ON ccfrc.gpd_type_id = gt.gpd_type_id
ORDER BY ccfr.ccf_result_id;

CREATE VIEW tcs_ccf_plot_2d_view AS
SELECT 	ccfr.computation_id, ccfk.tcs_ccf_kinematic_id, 
	ccfk.xi, ccfk.xi_unit,
	ccfk.t, ccfk.t_unit,
	ccfk.Q2Prim, ccfk.Q2Prim_unit,
	ccfk.MuF2, ccfk.MuF2_unit,
	ccfk.MuR2, ccfk.MuR2_unit,
	ccfr.ccf_result_id, ccfr.computation_module_name, gt.gpd_type_short_name, 
	ccfrc.real_part, '1' as 'real_part_unit',
	ccfrc.img_part, '1' as 'img_part_unit'
FROM tcs_ccf_result ccfrc
INNER JOIN ccf_result ccfr ON ccfr.ccf_result_id = ccfrc.ccf_result_id
INNER JOIN tcs_ccf_kinematic ccfk ON ccfk.tcs_ccf_kinematic_id = ccfr.ccf_kinematic_id
INNER JOIN gpd_type gt ON ccfrc.gpd_type_id = gt.gpd_type_id
ORDER BY ccfr.ccf_result_id;
