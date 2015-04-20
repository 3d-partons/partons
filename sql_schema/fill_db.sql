
INSERT INTO `quark_flavor`(`id`, `short_name`, `name`) values('0', 'u', 'UP');
INSERT INTO `quark_flavor`(`id`, `short_name`, `name`) values('1', 'd', 'DOWN');
INSERT INTO `quark_flavor`(`id`, `short_name`, `name`) values('2', 's', 'STRANGE');
INSERT INTO `quark_flavor`(`id`, `short_name`, `name`) values('3', 'c', 'CHARM');
INSERT INTO `quark_flavor`(`id`, `short_name`, `name`) values('4', 'b', 'BOTTOM');
INSERT INTO `quark_flavor`(`id`, `short_name`, `name`) values('5', 't', 'TOP');

INSERT INTO `gpd_compute_type`(`id`, `short_name`, `name`) values('0', 'H', 'H');
INSERT INTO `gpd_compute_type`(`id`, `short_name`, `name`) values('1', 'Ht', 'H transvers');
INSERT INTO `gpd_compute_type`(`id`, `short_name`, `name`) values('2', 'E', 'E');
INSERT INTO `gpd_compute_type`(`id`, `short_name`, `name`) values('3', 'Et', 'E transvers');

INSERT INTO `gpd_result`(`id`, `gpd_compute_type_id`, `gluon`, `singlet`) values('0', '0', '0.660866', '0');

INSERT INTO `gpd_result_quark_flavor`(`id`, `gpd_result_id`, `parton_distribution`, `parton_distribution_plus`, `parton_distribution_plus`) values('0', '0', '3.24235', '0', '0');

INSERT INTO `gpd_kinematic`(`id`, `x`, `xi`, `xB`, `t`, `MuF`, `MuR`) values('0', '0.1', '0', '0.001', '-0.3', '4', '4');
