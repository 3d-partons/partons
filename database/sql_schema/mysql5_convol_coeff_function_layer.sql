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

