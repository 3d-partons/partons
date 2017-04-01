/* MYSQL syntax */

CREATE TABLE gpd_kinematic (
gpd_kinematic_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
x DOUBLE,
xi DOUBLE,
t DOUBLE,
MuF2 DOUBLE,
MuR2 DOUBLE,
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

/* Specific view to filter on GPD results to make a plot */
CREATE VIEW gpd_plot_2d_view AS 
SELECT gr.computation_id, gk.gpd_kinematic_id, gk.x, gk.xi, gk.t, gk.MuF2, gk.MuR2, gr.gpd_result_id, gr.computation_module_name, gt.gpd_type_short_name, pd.gluon_distribution_value, qf.quark_flavor_short_name, qd.quark_distribution, qd.quark_distribution_plus, qd.quark_distribution_minus
FROM gpd_result gr
INNER JOIN gpd_kinematic gk ON gr.gpd_kinematic_id = gk.gpd_kinematic_id
INNER JOIN computation c ON gr.computation_id = c.computation_id
INNER JOIN gpd_result_parton_distribution grpd ON gr.gpd_result_id = grpd.gpd_result_id
INNER JOIN parton_distribution pd ON grpd.parton_distribution_id = pd.parton_distribution_id
INNER JOIN parton_distribution_quark_distribution pdqd ON pd.parton_distribution_id = pdqd.parton_distribution_id
INNER JOIN quark_distribution qd ON pdqd.quark_distribution_id = qd.quark_distribution_id
INNER JOIN gpd_type gt ON grpd.gpd_type_id = gt.gpd_type_id
INNER JOIN quark_flavor qf ON qd.quark_flavor_id = qf.quark_flavor_id
ORDER BY gr.gpd_result_id;
/*
Output example :
+----------------+------------------+-------+------+------+------+------+---------------+-------------------------+---------------------+--------------------------+-------------------------+---------------------------+--------------------------+--------------------------+
| computation_id | gpd_kinematic_id | x     | xi   | t    | MuF2 | MuR2 | gpd_result_id | computation_module_name | gpd_type_short_name | gluon_distribution_value | quark_flavor_short_name | quark_distribution        | quark_distribution_plus  | quark_distribution_minus |
+----------------+------------------+-------+------+------+------+------+---------------+-------------------------+---------------------+--------------------------+-------------------------+---------------------------+--------------------------+--------------------------+
|              1 |                1 | -0.99 | 0.99 |    0 |    1 |    1 |             1 | GK11Model               | H                   |   0.00007509382801838713 | u                       |    -0.0000102006278065657 |    -0.015296739384740365 |     0.015276338129127234 |
|              1 |                1 | -0.99 | 0.99 |    0 |    1 |    1 |             1 | GK11Model               | H                   |   0.00007509382801838713 | d                       |    -0.0000102006278065657 |    -0.004517075541054799 |     0.004496674285441668 |
|              1 |                1 | -0.99 | 0.99 |    0 |    1 |    1 |             1 | GK11Model               | H                   |   0.00007509382801838713 | s                       | -0.0000029686059797710613 | -0.000005937211959542123 |                        0 |
|              1 |                1 | -0.99 | 0.99 |    0 |    1 |    1 |             1 | GK11Model               | E                   |  0.000005022836218104747 | u                       |  0.0000020206722563367355 |   -0.0049832141420576076 |     0.004987255486570281 |
+----------------+------------------+-------+------+------+------+------+---------------+-------------------------+---------------------+--------------------------+-------------------------+---------------------------+--------------------------+--------------------------+

*/

