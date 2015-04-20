CREATE DATABASE IF NOT EXISTS `partons` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

USE `PARTONS`;

CREATE TABLE IF NOT EXISTS `quark_flavor` (
	`id` int NOT NULL,
	`short_name` VARCHAR(32) NOT NULL, 
	`name` VARCHAR(32) NOT NULL,

	PRIMARY KEY(`id`),
	UNIQUE KEY `idx_short_name` (`short_name`),
	UNIQUE KEY `idx_name` (`name`)

) DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC ;


CREATE TABLE IF NOT EXISTS `gpd_compute_type` (
	`id` int NOT NULL,
	`short_name` VARCHAR(32) NOT NULL, 
	`name` VARCHAR(32) NOT NULL,

	PRIMARY KEY(`id`),
	UNIQUE KEY `idx_short_name` (`short_name`),
	UNIQUE KEY `idx_name` (`name`)

) DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC ;

CREATE TABLE IF NOT EXISTS `gpd_result` (
	`id` int NOT NULL,
	`gpd_compute_type_id` int NOT NULL,
	`gluon` DOUBLE NOT NULL DEFAULT 0, 
	`singlet` DOUBLE NOT NULL DEFAULT 0,

	PRIMARY KEY(`id`),
	FOREIGN KEY (`gpd_compute_type_id`)
	REFERENCES `gpd_compute_type`(`id`)

) DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC ;

CREATE TABLE IF NOT EXISTS `gpd_result_quark_flavor` (
	`id` int NOT NULL,
	`gpd_result_id` int NOT NULL,
	`parton_distribution` DOUBLE NOT NULL DEFAULT 0, 
	`parton_distribution_minus` DOUBLE NOT NULL DEFAULT 0, 
	`parton_distribution_plus` DOUBLE NOT NULL DEFAULT 0, 

	PRIMARY KEY(`id`),
	FOREIGN KEY (`gpd_result_id`)
	REFERENCES `gpd_result`(`id`)

) DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC ;

CREATE TABLE IF NOT EXISTS `gpd_kinematic` (
	`id` int NOT NULL,
	`x` DOUBLE NOT NULL DEFAULT 0,
	`xi` DOUBLE NOT NULL DEFAULT 0, 
	`xB` DOUBLE NOT NULL DEFAULT 0, 
	`t` DOUBLE NOT NULL DEFAULT 0,
	`MuF` DOUBLE NOT NULL DEFAULT 0, 
	`MuR` DOUBLE NOT NULL DEFAULT 0,  

	PRIMARY KEY(`id`)

) DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC ;

