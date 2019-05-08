/* MYSQL syntax */

CREATE TABLE gpd_kinematic (
gpd_kinematic_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
x DOUBLE,
x_unit INTEGER NOT NULL,
xi DOUBLE,
xi_unit INTEGER NOT NULL,
t DOUBLE,
t_unit INTEGER NOT NULL,
MuF2 DOUBLE,
MuF2_unit INTEGER NOT NULL,
MuR2 DOUBLE,
MuR2_unit INTEGER NOT NULL,
hash_sum VARCHAR(40) NOT NULL);
CREATE INDEX gpd_kinematic_index ON gpd_kinematic (hash_sum);

CREATE TABLE gpd_result (
gpd_result_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
computation_module_name VARCHAR(255) NOT NULL,
gpd_kinematic_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);
CREATE INDEX gpd_result_index ON gpd_result (computation_id);

CREATE TABLE parton_distribution (
parton_distribution_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
gluon_distribution_value DOUBLE);

CREATE TABLE gpd_result_parton_distribution (
gpd_result_parton_distribution_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
gpd_type_id INTEGER NOT NULL,
gpd_result_id INTEGER NOT NULL,
parton_distribution_id INTEGER NOT NULL);
CREATE INDEX grpd_index_a ON gpd_result_parton_distribution (parton_distribution_id, gpd_result_id);
CREATE INDEX grpd_index_b ON gpd_result_parton_distribution (gpd_result_id);

CREATE TABLE quark_distribution (
quark_distribution_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
quark_distribution_plus DOUBLE,
quark_distribution_minus DOUBLE,
quark_distribution DOUBLE,
quark_flavor_id INTEGER NOT NULL);

CREATE TABLE parton_distribution_quark_distribution (
parton_distribution_quark_distribution_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
parton_distribution_id INTEGER NOT NULL,
quark_distribution_id INTEGER NOT NULL);
CREATE INDEX pdqd_index_a ON parton_distribution_quark_distribution (quark_distribution_id, parton_distribution_id);
CREATE INDEX pdqd_index_b ON parton_distribution_quark_distribution (parton_distribution_id);

CREATE VIEW gpd_result_view AS 
SELECT gr.gpd_result_id, pd.parton_distribution_id, gr.computation_module_name, grpd.gpd_type_id, pd.gluon_distribution_value, qd.quark_flavor_id, qd.quark_distribution, qd.quark_distribution_plus, qd.quark_distribution_minus, gr.computation_id
FROM gpd_result gr
INNER JOIN computation c ON gr.computation_id = c.computation_id
INNER JOIN gpd_result_parton_distribution grpd ON gr.gpd_result_id = grpd.gpd_result_id
INNER JOIN parton_distribution pd ON grpd.parton_distribution_id = pd.parton_distribution_id
INNER JOIN parton_distribution_quark_distribution pdqd ON pd.parton_distribution_id = pdqd.parton_distribution_id
INNER JOIN quark_distribution qd ON pdqd.quark_distribution_id = qd.quark_distribution_id
ORDER BY gr.gpd_result_id;

CREATE VIEW gpd_plot_2d_view AS 
SELECT 	gr.computation_id, gk.gpd_kinematic_id, 
	gk.x, gk.x_unit, 
	gk.xi, gk.xi_unit, 
	gk.t, gk.t_unit, 
	gk.MuF2, gk.MuF2_unit, 
	gk.MuR2, gk.MuR2_unit, 
	gr.gpd_result_id, gr.computation_module_name, 
	gt.gpd_type_short_name, 
	pd.gluon_distribution_value, '1' as 'gluon_distribution_value_unit',
	qf.quark_flavor_short_name, 
	qd.quark_distribution, '1' as 'quark_distribution_unit', 
	qd.quark_distribution_plus, '1' as 'quark_distribution_plus_unit', 
	qd.quark_distribution_minus, '1' as 'quark_distribution_minus_unit'
FROM 	gpd_result gr
INNER JOIN gpd_kinematic gk ON gr.gpd_kinematic_id = gk.gpd_kinematic_id
INNER JOIN computation c ON gr.computation_id = c.computation_id
INNER JOIN gpd_result_parton_distribution grpd ON gr.gpd_result_id = grpd.gpd_result_id
INNER JOIN parton_distribution pd ON grpd.parton_distribution_id = pd.parton_distribution_id
INNER JOIN parton_distribution_quark_distribution pdqd ON pd.parton_distribution_id = pdqd.parton_distribution_id
INNER JOIN quark_distribution qd ON pdqd.quark_distribution_id = qd.quark_distribution_id
INNER JOIN gpd_type gt ON grpd.gpd_type_id = gt.gpd_type_id
INNER JOIN quark_flavor qf ON qd.quark_flavor_id = qf.quark_flavor_id
ORDER BY gr.gpd_result_id;
