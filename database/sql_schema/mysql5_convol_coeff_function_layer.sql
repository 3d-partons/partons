/* MYSQL syntax */

CREATE TABLE ccf_kinematic (
ccf_kinematic_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
xi DOUBLE NOT NULL,
t DOUBLE NOT NULL,
Q2 DOUBLE NOT NULL,
MuF2 DOUBLE NOT NULL,
MuR2 DOUBLE NOT NULL,
hash_sum VARCHAR(40));
CREATE INDEX ccf_kinematic_index ON ccf_kinematic (hash_sum);

CREATE TABLE ccf_result (
ccf_result_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
computation_module_name VARCHAR(255) NOT NULL,
channel_id INTEGER NOT NULL,
ccf_kinematic_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);
CREATE INDEX ccf_result_index ON ccf_result (computation_id);

CREATE TABLE ccf_result_complex (
ccf_result_complex_id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
gpd_type_id INTEGER NOT NULL,
real_part DOUBLE NOT NULL,
img_part DOUBLE NOT NULL,
ccf_result_id INTEGER NOT NULL);
CREATE INDEX ccf_result_complex_index ON ccf_result_complex (ccf_result_id);


CREATE VIEW ccf_result_view AS 
SELECT ccfr.ccf_result_id, ccfr.computation_module_name, ccfr.channel_id, ccfrc.gpd_type_id, ccfrc.real_part, ccfrc.img_part, c.computation_id
FROM ccf_result ccfr
INNER JOIN computation c ON ccfr.computation_id = c.computation_id
INNER JOIN ccf_result_complex ccfrc ON ccfr.ccf_result_id = ccfrc.ccf_result_id
ORDER BY ccfr.ccf_result_id;

/* Specific view to filter on CCF results to make a plot */
CREATE VIEW ccf_plot_2d_view AS
SELECT ccfr.computation_id, ccfk.ccf_kinematic_id, ccfk.xi, ccfk.t, ccfk.Q2, ccfk.MuF2, ccfk.MuR2, ccfr.ccf_result_id, oc.observable_channel_short_name, ccfr.computation_module_name, gt.gpd_type_short_name, ccfrc.real_part, ccfrc.img_part
FROM ccf_kinematic ccfk
INNER JOIN ccf_result ccfr ON ccfr.ccf_kinematic_id = ccfk.ccf_kinematic_id
INNER JOIN ccf_result_complex ccfrc ON ccfrc.ccf_result_id = ccfr.ccf_result_id
INNER JOIN gpd_type gt ON ccfrc.gpd_type_id = gt.gpd_type_id
INNER JOIN observable_channel oc ON ccfr.channel_id = oc.observable_channel_id
ORDER BY ccfr.ccf_result_id;
/*
Output example :
+----------------+------------------+------+---+----+------+------+---------------+-------------------------------+-------------------------+---------------------+--------------------+----------------------+
| computation_id | ccf_kinematic_id | xi   | t | Q2 | MuF2 | MuR2 | ccf_result_id | observable_channel_short_name | computation_module_name | gpd_type_short_name | real_part          | img_part             |
+----------------+------------------+------+---+----+------+------+---------------+-------------------------------+-------------------------+---------------------+--------------------+----------------------+
|              6 |                1 | 0.99 | 0 |  1 |    1 |    1 |             1 | DVCS                          | DVCSCFFModel            | H                   | 1.0830598567370253 | 0.022937106659652748 |
+----------------+------------------+------+---+----+------+------+---------------+-------------------------------+-------------------------+---------------------+--------------------+----------------------+
*/

