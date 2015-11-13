/* SQLITE syntax */

CREATE TABLE convol_coeff_function_kinematic (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
xi DOUBLE NOT NULL,
t DOUBLE NOT NULL,
Q2 DOUBLE NOT NULL,
MuF2 DOUBLE NOT NULL,
MuR2 DOUBLE NOT NULL);

CREATE TABLE convol_coeff_function_result (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
real_part DOUBLE NOT NULL,
img_part DOUBLE NOT NULL,
gpd_type_id INTEGER NOT NULL,
computation_module_name VARCHAR(255) NOT NULL,
convol_coeff_function_kinematic_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);
