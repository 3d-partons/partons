/* MySQL syntax */

CREATE TABLE laboratory (
id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, 
name VARCHAR(255));

CREATE TABLE collaboration (
id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, 
name VARCHAR(255),
laboratory_id INTEGER);

CREATE TABLE experiment (
id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, 
process VARCHAR(255), 
experiment_type VARCHAR(255), 
year INTEGER(4), 
reference VARCHAR(255),
collaboration_id INTEGER);

CREATE TABLE observable_kinematic (
id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
bin_id INTEGER NOT NULL, 
xB DOUBLE NOT NULL, 
t DOUBLE NOT NULL, 
Q2 DOUBLE NOT NULL, 
phi DOUBLE NOT NULL,
experiment_id INTEGER);

CREATE TABLE observable_result (
id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,  
observable_name VARCHAR(255) NOT NULL, 
observable_value DOUBLE NOT NULL, 
stat_error_lb DOUBLE,
stat_error_ub DOUBLE,
syst_error_lb DOUBLE,
syst_error_ub DOUBLE,
total_error DOUBLE,
computation_module_name VARCHAR(255) NOT NULL,
observable_type_id INTEGER NOT NULL,
observable_kinematic_id INTEGER NOT NULL,
computation_id INTEGER NOT NULL);
