/* SQLITE syntax */

CREATE TABLE gpd_kinematic (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
x DOUBLE,
xi DOUBLE,
t DOUBLE,
MuF2 DOUBLE,
MuR2 DOUBLE);

CREATE TABLE gpd_result (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
computation_module_name VARCHAR(255) NOT NULL,
gpd_kinematic_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);

CREATE TABLE parton_distribution (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
gluon_distribution_value DOUBLE);

CREATE TABLE gpd_result_parton_distribution (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
gpd_type_id INTEGER NOT NULL,
gpd_result_id INTEGER NOT NULL,
parton_distribution_id INTEGER NOT NULL);

CREATE TABLE quark_distribution (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
quark_distribution_plus DOUBLE,
quark_distribution_minus DOUBLE,
quark_distribution DOUBLE,
quark_flavor_id INTEGER NOT NULL);

CREATE TABLE parton_distribution_quark_distribution (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
parton_distribution_id INTEGER NOT NULL,
quark_distribution_id INTEGER NOT NULL);
