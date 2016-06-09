/* MYSQL syntax */

CREATE TABLE gpd_kinematic (
id INTEGER NOT NULL AUTO_INCREMENT,
x DOUBLE,
xi DOUBLE,
t DOUBLE,
MuF2 DOUBLE,
MuR2 DOUBLE,
hash_sum VARCHAR(40),
PRIMARY KEY (id),
INDEX gpd_kinematic_index (hash_sum));

CREATE TABLE gpd_result (
id INTEGER NOT NULL AUTO_INCREMENT,
computation_module_name VARCHAR(255) NOT NULL,
gpd_kinematic_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL,
PRIMARY KEY (id),
INDEX gpd_result_index (computation_id));

CREATE TABLE parton_distribution (
id INTEGER NOT NULL AUTO_INCREMENT,
gluon_distribution_value DOUBLE,
PRIMARY KEY (id));

CREATE TABLE gpd_result_parton_distribution (
id INTEGER NOT NULL AUTO_INCREMENT,
gpd_type_id INTEGER NOT NULL,
gpd_result_id INTEGER NOT NULL,
parton_distribution_id INTEGER NOT NULL,
PRIMARY KEY (id),
INDEX grpd_index_a (parton_distribution_id, gpd_result_id),
INDEX grpd_index_b (gpd_result_id));

CREATE TABLE quark_distribution (
id INTEGER NOT NULL AUTO_INCREMENT,
quark_distribution_plus DOUBLE,
quark_distribution_minus DOUBLE,
quark_distribution DOUBLE,
quark_flavor_id INTEGER NOT NULL,
PRIMARY KEY (id));

CREATE TABLE parton_distribution_quark_distribution (
id INTEGER NOT NULL AUTO_INCREMENT,
parton_distribution_id INTEGER NOT NULL,
quark_distribution_id INTEGER NOT NULL,
PRIMARY KEY (id),
INDEX pdqd_index_a (quark_distribution_id, parton_distribution_id),
INDEX pdqd_index_b (parton_distribution_id));
