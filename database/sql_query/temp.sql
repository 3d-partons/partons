SELECT DISTINCT obr.id, obr.observable_name , obr.observable_value , obr.phi
FROM observable_result obr, observable_kinematic obk
WHERE obr.observable_kinematic_id = obk.id
AND obk.id = '1'
AND obr.computation_id = '2';

SELECT *
FROM observable_kinematic
WHERE id = '1';