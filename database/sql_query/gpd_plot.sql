SELECT DISTINCT gk.x, pd.gluon_distribution
FROM quark_flavor qf, gpd_type gt, kinematic_type kt, gpd_kinematic gk, module m, parton_distribution pd, quark_distribution qd, gpd_result gr
WHERE gr.gpd_kinematic_id = gk.id
AND gr.module_id = m.id
AND pd.gpd_result_id = gr.id
AND pd.gpd_type_id = gt.id
AND qd.parton_distribution_id = pd.id
AND gt.short_name = 'H'
AND gk.xi = 0.2
AND gk.t = -0.1
AND gk.MuF = 2
AND gk.MuR = 2;

SELECT DISTINCT gk.x, pd.gluon_distribution_value
FROM gpd_kinematic gk, parton_distribution pd, gpd_type gt, gpd_result gr, gpd_result_parton_distribution grpd
WHERE gr.gpd_kinematic_id = gk.id
AND grpd.gpd_result_id = gr.id
AND grpd.parton_distribution_id = pd.id
AND grpd.gpd_type_id = gt.id
AND gr.computation_id = '1'
AND gt.short_name = 'H';

SELECT DISTINCT gk.x, qd.quark_distribution
FROM gpd_kinematic gk, parton_distribution pd, gpd_type gt, gpd_result gr, gpd_result_parton_distribution grpd, quark_flavor qf, quark_distribution qd
WHERE gr.gpd_kinematic_id = gk.id
AND grpd.gpd_result_id = gr.id
AND grpd.parton_distribution_id = pd.id
AND grpd.gpd_type_id = gt.id
AND qd.parton_distribution_id = pd.id
AND qd.quark_flavor_id = qf.id
AND gr.computation_id = '1'
AND gt.short_name = 'H'
AND qf.short_name = 'u';
