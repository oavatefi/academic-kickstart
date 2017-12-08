

/* auto-generated code, any modifications made to it will be lost! */
#ifndef I_CFGH_SET_H
#  define I_CFGH_SET_H

#  define CFGH_CONFIGURATION_DATATYPE struct APPL_codingdata_S

enum CFGH_carconfig_index_E
{
    CFGH_PARKMAN_CONFIG,
    CFGH_CARCONFIG_COUNT
};
#  ifdef CFGH_ROM_DATA_IS_COMPRESSED
const u8 compressed_configurations[] =
#  else
const CFGH_CONFIGURATION_DATATYPE configurations[] =
#  endif
#  if defined (APPL_CAR_CONFIG)
/* Data from "..\CarConfiguration_DAS_3_2.xls" */

{
    /* PARKMAN_CONFIG (16h) */
    { /* active_coding_dataset */
        { /* active_coding_dataset.das_cfg */
            { /* active_coding_dataset.das_cfg.dapm_cfg */
                { /* active_coding_dataset.das_cfg.dapm_cfg.variant */
                    1 /* func_odom_wcc_det */
                },
                { /* active_coding_dataset.das_cfg.dapm_cfg.vehicle_cfg */
                    4933, /* length_mm */
                    1852, /* width_mm */
                    2939, /* wheel_base_mm */
                    1600, /* track_width_rear_mm */
                    841, /* front_axle_to_front_mm */
                    282, /* outline_dist_front_x_mm */
                    97, /* outline_dist_front_y_mm */
                    170, /* outline_dist_rear_x_mm */
                    97, /* outline_dist_rear_y_mm */
                    6124, /* max_wheel_angle_front_u16f */
                    0, /* max_wheel_angle_rear_u16f */
                    2100, /* default_wheel_circ_front_mm */
                    2100, /* default_wheel_circ_rear_mm */
                    2200, /* mirror_base_pt_x_mm */
                    20, /* mirror_size_x_mm */
                    20, /* mirror_size_y_mm */
                    8192 /* vehicle_contour_angle_rear_u16f */
                },
                { /* active_coding_dataset.das_cfg.dapm_cfg.num_sectors */
                    4, /* num_sectors_front */
                    4, /* num_sectors_rear */
                    4, /* num_sectors_side */
                    0 /* reserved */
                },
                { /* active_coding_dataset.das_cfg.dapm_cfg.us_num_sensors */
                    6, /* num_sensors_front */
                    6 /* num_sensors_rear */
                },
                { /* active_coding_dataset.das_cfg.dapm_cfg.sensor_group_cfg */
                    { /* active_coding_dataset.das_cfg.dapm_cfg.sensor_group_cfg.sensor_front_cfg */
                        { /* active_coding_dataset.das_cfg.dapm_cfg.sensor_group_cfg.sensor_front_cfg.sensor_cfg */
                            { /* active_coding_dataset.das_cfg.dapm_cfg.sensor_group_cfg.sensor_front_cfg.sensor_cfg.ARRAY_0 */
                                0, /* pose_p_x_mm */
                                0, /* pose_p_y_mm */
                                0, /* ang_u16f */
                                0, /* max_aperture_u16f */
                                0, /* max_detection_range_cm */
                                0, /* min_detection_range_cm */
                                0, /* max_detection_width_cm */
                                0, /* height_cm */
                                0 /* type */
                            },
                            { /* active_coding_dataset.das_cfg.dapm_cfg.sensor_group_cfg.sensor_front_cfg.sensor_cfg.ARRAY_1 */
                                3354, /* pose_p_x_mm */
                                -897, /* pose_p_y_mm */
                                51900, /* ang_u16f */
                                10923, /* max_aperture_u16f */
                                450, /* max_detection_range_cm */
                                40, /* min_detection_range_cm */
                                200, /* max_detection_width_cm */
                                49, /* height_cm */
                                1 /* type */
                            },
                            { /* active_coding_dataset.das_cfg.dapm_cfg.sensor_group_cfg.sensor_front_cfg.sensor_cfg.ARRAY_2 */
                                3555, /* pose_p_x_mm */
                                -757, /* pose_p_y_mm */
                                57525, /* ang_u16f */
                                18204, /* max_aperture_u16f */
                                450, /* max_detection_range_cm */
                                40, /* min_detection_range_cm */
                                140, /* max_detection_width_cm */
                                36, /* height_cm */
                                0 /* type */
                            },
                            { /* active_coding_dataset.das_cfg.dapm_cfg.sensor_group_cfg.sensor_front_cfg.sensor_cfg.ARRAY_3 */
                                3725, /* pose_p_x_mm */
                                -300, /* pose_p_y_mm */
                                63806, /* ang_u16f */
                                18204, /* max_aperture_u16f */
                                450, /* max_detection_range_cm */
                                40, /* min_detection_range_cm */
                                140, /* max_detection_width_cm */
                                45, /* height_cm */
                                0 /* type */
                            }
                        }
                    },
                    { /* active_coding_dataset.das_cfg.dapm_cfg.sensor_group_cfg.sensor_rear_cfg */
                        { /* active_coding_dataset.das_cfg.dapm_cfg.sensor_group_cfg.sensor_rear_cfg.sensor_cfg */
                            { /* active_coding_dataset.das_cfg.dapm_cfg.sensor_group_cfg.sensor_rear_cfg.sensor_cfg.ARRAY_0 */
                                0, /* pose_p_x_mm */
                                0, /* pose_p_y_mm */
                                0, /* ang_u16f */
                                0, /* max_aperture_u16f */
                                0, /* max_detection_range_cm */
                                0, /* min_detection_range_cm */
                                0, /* max_detection_width_cm */
                                0, /* height_cm */
                                0 /* type */
                            },
                            { /* active_coding_dataset.das_cfg.dapm_cfg.sensor_group_cfg.sensor_rear_cfg.sensor_cfg.ARRAY_1 */
                                -767, /* pose_p_x_mm */
                                -859, /* pose_p_y_mm */
                                46785, /* ang_u16f */
                                18204, /* max_aperture_u16f */
                                450, /* max_detection_range_cm */
                                40, /* min_detection_range_cm */
                                140, /* max_detection_width_cm */
                                47, /* height_cm */
                                0 /* type */
                            },
                            { /* active_coding_dataset.das_cfg.dapm_cfg.sensor_group_cfg.sensor_rear_cfg.sensor_cfg.ARRAY_2 */
                                -1062, /* pose_p_x_mm */
                                -739, /* pose_p_y_mm */
                                38684, /* ang_u16f */
                                18204, /* max_aperture_u16f */
                                450, /* max_detection_range_cm */
                                40, /* min_detection_range_cm */
                                140, /* max_detection_width_cm */
                                49, /* height_cm */
                                0 /* type */
                            },
                            { /* active_coding_dataset.das_cfg.dapm_cfg.sensor_group_cfg.sensor_rear_cfg.sensor_cfg.ARRAY_3 */
                                -1126, /* pose_p_x_mm */
                                -300, /* pose_p_y_mm */
                                33477, /* ang_u16f */
                                18204, /* max_aperture_u16f */
                                450, /* max_detection_range_cm */
                                40, /* min_detection_range_cm */
                                140, /* max_detection_width_cm */
                                34, /* height_cm */
                                0 /* type */
                            }
                        }
                    }
                },
                { /* active_coding_dataset.das_cfg.dapm_cfg.distances_cfg */
                    0, /* dist_rss */
                    33, /* dist_rso */
                    48, /* dist_rio */
                    60, /* dist_rii */
                    0, /* dist_fss */
                    29, /* dist_fso */
                    49, /* dist_fio */
                    60 /* dist_fii */
                },
                { /* active_coding_dataset.das_cfg.dapm_cfg.reserved */
                    0, /* si16_1 */
                    0, /* si16_2 */
                    0, /* si16_3 */
                    0, /* si16_4 */
                    0, /* u16_1 */
                    0, /* u16_2 */
                    0, /* u16_3 */
                    0, /* u16_4 */
                    0, /* u8_1 */
                    0, /* u8_2 */
                    0, /* u8_3 */
                    0 /* u8_4 */
                },
                {58982,59856,60730,61604,62478,63351,64225,65099,437,1311,2185,3058,3932,4806,5680,6553}, /* wheel_ang_front_fw_tbl_u16f */
                {59749,60497,61256,62022,62796,63574,64358,65143,393,1179,1962,2740,3514,4281,5039,5786}, /* wheel_ang_front_bw_tbl_u16f - from Anto (VS-283) (26.7.2017)*/
                {-6554,-5680,-4806,-3932,-3058,-2185,-1311,-437,437,1311,2185,3058,3932,4806,5680,6553}, /* steer_in_out_front_tbl */
                1000, /* wa_tbl_morph_incr_u16f_s */
                2000, /* clear_dist_mm */
                -1000, /* min_depth_midf_in_slot_mm */
                1, /* errt_error_level_thresh */
                100, /* max_dist_against_gear_cm */
                0, /* max_dr_dist_at_act_cm */
                0, /* trailer_hitch_length_cm */
                0, /* is_rear_sensor_silenced_for_psm */
                1, /* trailer_dis_rear_side_sensors */
                0, /* global_timer_offset_start_ms */
                0, /* max_v_front_slot_valid_10thkmh */
                70, /* max_way_behind_hint_dm */
                1, /* is_veh_lot_controlled */
                255, /* standstill_reset_time_1s */
#    ifdef CFGD_LONGI_PROFILE
                4, /* lot_ctrl_mode_freedrive */
#    endif
#    ifdef CFGD_LONGI_PROFILE
#  		ifdef XAPPL_LOT_CNTRL_SUPP
                4, /* lot_ctrl_mode_active_maneuver */
#    	else
                0, /* lot_ctrl_mode_active_maneuver */
#    	endif
#    endif
                20 /* vct_cycle_time */
            },
#    ifdef CFGD_SECTOR
            { /* active_coding_dataset.das_cfg.vdim_cfg */
                { /* active_coding_dataset.das_cfg.vdim_cfg.sect_cfg */
                    { /* active_coding_dataset.das_cfg.vdim_cfg.sect_cfg.sect_acoustical_cfg */
                        1325, /* acoustical_border_pos_x_mm */
                        500, /* acoustical_border_dist_front_mm */
                        500, /* acoustical_border_dist_rear_mm */
                        0, /* ct_reduction_cm */
                        0, /* acoustical_mapping */
                        1, /* acoustical_warn_strategy */
                        0, /* force_continuous_tone */
                        250, /* weak_ct_speaker_ctrl_value */
                        1, /* suppress_tone_vs_wd_in_ss */
                        1, /* is_dt_sync_req */
                        0 /* reserve1 */
                    },
#      ifdef CFGD_PREDICTED_PDC
                    {200,200,200,200,200,200,200,200}, /* prediction_time_ms */
#      endif
                    { /* active_coding_dataset.das_cfg.vdim_cfg.sect_cfg.sect_graphical_cfg */
                        1, /* graphical_warn_strategy */
                        255 /* min_length_long_high_obj_cm */
                    },
                    { /* active_coding_dataset.das_cfg.vdim_cfg.sect_cfg.sect_trend_cfg */
                        1, /* it_trend_strategy_front */
                        0, /* ct_trend_strategy_front */
                        1, /* it_trend_strategy_rear */
                        0, /* ct_trend_strategy_rear */
                        1, /* it_trend_strategy_left */
                        0, /* ct_trend_strategy_left */
                        1, /* it_trend_strategy_right */
                        0, /* ct_trend_strategy_right */
                        1, /* it_trend_strategy_vs_wd */
                        0 /* ct_trend_strategy_vs_wd */
                    },
                    { /* active_coding_dataset.das_cfg.vdim_cfg.sect_cfg.sect_tube_cfg */
                        20, /* tube_safety_dist_cm */
                        0 /* tube_type */
                    },
                    0, /* min_rel_height_front_rear */
                    1, /* is_curb_suppr_front_rear_active */
                    0, /* min_rel_height_side */
                    1, /* is_curb_suppr_side_active */
                    0, /* high_velo_ops_mode */
                    8, /* min_high_velo_ops_kmh */
                    1, /* num_sect_trailer_hitch_span */
                    10, /* dist_to_warn_in_contour_cm */
                    0, /* activate_front_on_active_only */
                    0 /* intersect_pt_stab_area */
                },
                { /* active_coding_dataset.das_cfg.vdim_cfg.sect_hysteresis_cfg */
                    0, /* hyst_dist_time_a_ms */
                    0, /* hyst_dist_time_d_ms */
                    500, /* hyst_dist_ss_time_a_ms */
                    1000, /* hyst_dist_ss_time_d_ms */
                    0, /* hyst_obj_time_entry_ms */
                    0, /* hyst_obj_time_exit_ms */
                    80, /* hyst_ushrp_obj_time_entry_ms */
                    240, /* hyst_ushrp_obj_time_exit_ms */
                    500, /* hyst_obj_ss_time_entry_ms */
                    1000, /* hyst_obj_ss_time_exit_ms */
                    500, /* hyst_ushrp_obj_ss_time_entry_ms */
                    1000, /* hyst_ushrp_obj_ss_time_exit_ms */
                    80, /* hyst_obj_time_fast_steer_ms */
                    500, /* min_v_wa_fast_hyst_u16fs */
                    5, /* sect_hyst_dist_cm */
                    5, /* adj_sect_hyst_inac_cm */
                    2 /* adj_sect_hyst_ac_cm */
                },
                { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg */
                    { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points */
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.front_sector0_p0 */
                            3716, /* x_mm */
                            455, /* y_mm */
                            3823 /* angle_u16f */
                        },
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.front_sector0_p1 */
                            3545, /* x_mm */
                            787, /* y_mm */
                            10194 /* angle_u16f */
                        },
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.front_sector1_p0 */
                            3780, /* x_mm */
                            0, /* y_mm */
                            0 /* angle_u16f */
                        },
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.front_sector2_p0 */
                            0, /* x_mm */
                            0, /* y_mm */
                            0 /* angle_u16f */
                        },
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.front_sector3_p0 */
                            0, /* x_mm */
                            0, /* y_mm */
                            0 /* angle_u16f */
                        },
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.rear_sector0_p0 */
                            -985, /* x_mm */
                            830, /* y_mm */
                            22755 /* angle_u16f */
                        },
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.rear_sector0_p1 */
                            -1125, /* x_mm */
                            470, /* y_mm */
                            28945 /* angle_u16f */
                        },
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.rear_sector1_p1 */
                            -1153, /* x_mm */
                            0, /* y_mm */
                            32768 /* angle_u16f */
                        },
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.rear_sector2_p1 */
                            0, /* x_mm */
                            0, /* y_mm */
                            0 /* angle_u16f */
                        },
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.rear_sector3_p1 */
                            0, /* x_mm */
                            0, /* y_mm */
                            0 /* angle_u16f */
                        },
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.side_sector0_p1 */
                            3163, /* x_mm */
                            926, /* y_mm */
                            16384 /* angle_u16f */
                        },
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.side_sector1_p1 */
                            1325, /* x_mm */
                            926, /* y_mm */
                            16384 /* angle_u16f */
                        },
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.side_sector2_p1 */
                            -505, /* x_mm */
                            926, /* y_mm */
                            16384 /* angle_u16f */
                        },
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.side_sector3_p1 */
                            0, /* x_mm */
                            0, /* y_mm */
                            0 /* angle_u16f */
                        },
                        { /* active_coding_dataset.das_cfg.vdim_cfg.sect_layout_cfg.sector_points.side_sector4_p1 */
                            0, /* x_mm */
                            0, /* y_mm */
                            0 /* angle_u16f */
                        }
                    },
                    {1,1,0,0,1,1,0,0,1,1,1,1,0,0}, /* sector_active_cfg */
                    {1,1,1,0,1,1,1,0,1,0,0,1,0,0}, /* sector_type_cfg */
                    {103,100,0,0,65,101,0,0,64,60,60,60,0,0}, /* sector_p0_depth_cm_cfg */
                    {64,103,0,0,101,100,0,0,60,60,60,65,0,0}, /* sector_p1_depth_cm_cfg */
                    {20,20,0,0,20,20,0,0,20,20,20,20,0,0}, /* sector_ct_border_cm_cfg */
                    {100,100,0,0,100,100,0,0,60,60,60,60,0,0} /* sector_it_border_cm_cfg */
                },
#    ifdef CFGD_BSD_FILT_OUTPUT
                { /* active_coding_dataset.das_cfg.vdim_cfg.bsda_cfg */
                    1092, /* alarm_indication_hyst_ms */
                    1, /* min_speed_active_kmh */
                    60 /* max_speed_active_kmh */
                },
#  endif
#  ifdef CFGD_HAPTIC
                { /* active_coding_dataset.das_cfg.vdim_cfg.hapt_cfg */
                    900, /* add_wa_drv_pref_u16f */
                    10, /* min_torq_drv_to_act_100thnm */
                    501, /* min_torq_drv_to_deact_100thnm */
                    5461, /* min_wa_dev_to_deact_u16f */
                    5461, /* max_wa_dev_to_act_u16f */
                    50, /* min_torq_cs_100thnm */
                    100, /* max_torq_cs_100thnm */
                    {0,0,0,182}, /* dev_wa_u16f */
                    {0,0,0,300}, /* torq_to_apply_100thnm */
                    5000, /* torq_slope_inc_100thnms */
                    500, /* torq_slope_dec_100thnms */
                    150, /* min_torq_drv_ss_to_act_100thnm */
                    5000, /* max_rel_ttc_ms */
                    900, /* add_wa_old_rslt_u16f */
                    1800, /* max_speed_100thkmh */
                    3640, /* max_delta_wa_to_support_u16f */
                    0, /* entry_offset_u16f */
                    327, /* exit_offset_u16f */
                    0, /* min_entry_offset_u16f */
                    0, /* min_offset_free_u16f */
                    0, /* min_dist_low_speed_hapt_out_cm */
                    0, /* wa_area_relevance */
                    0, /* no_hapt_both_dir_possible */
                    0, /* inhibit_if_braking_imminent */
                    0, /* is_drv_pref_torq_active */
                    0, /* is_drv_behaviour_active */
                    100, /* min_out_factor */
                    200, /* max_out_factor */
                    1, /* filt_d */
                    20, /* filt_1_t_pt2 */
                    0, /* is_pt2_pre_filt_active */
                    1 /* is_ramp_filt_active */
                },
#endif
                1092, /* edgcov_min_wa_u16 */
                0, /* edgcov_enable */
                60 /* edgcov_max_warning_dist_cm */
            },
#endif
            { /* active_coding_dataset.das_cfg.ulfm_cfg */
#ifdef CFGD_ULFX_BSD
                { /* active_coding_dataset.das_cfg.ulfm_cfg.dsfe_cfg */
                    0, /* rel_velocity_slope */
                    2000, /* stagnation_delay_ms */
                    32768 /* delay_time_factor */
                },
#endif
#ifdef CFGD_ULFX_BSD_SIDE_ASSIST
                { /* active_coding_dataset.das_cfg.ulfm_cfg.sai_cfg */
                    { /* active_coding_dataset.das_cfg.ulfm_cfg.sai_cfg.noise_det_cfg */
                        33554432, /* noise_integration_constant_low */
                        50331647, /* noise_integration_constant_high */
                        600, /* noise_std_thresh */
                        5000, /* noise_high_thresh */
                        5000 /* min_spd_nse_est_upd_100th_kph */
                    },
                    120, /* t_tune_blocking_fov_ms */
                    30, /* t_tune_uc_det_fov_ms */
                    1000, /* warn_hold_time_ms */
                    2000, /* max_rel_speed_to_warn_100th_kph */
                    5000, /* min_speed_uc_det_100th_kph */
                    1500, /* min_rel_speed_pfr_100th_kph */
                    3000, /* max_dist_considered_mm */
                    1, /* both_sides_quiet_logic */
                    0 /* reserved */
                },
#endif
                { /* active_coding_dataset.das_cfg.ulfm_cfg.ulfc_blend_out */
                    { /* active_coding_dataset.das_cfg.ulfm_cfg.ulfc_blend_out.sgw_blend_out_cfg */
                        { /* active_coding_dataset.das_cfg.ulfm_cfg.ulfc_blend_out.sgw_blend_out_cfg.ARRAY_0 */
                            0, /* phys_sens_id_rx */
                            0, /* phys_sens_id_tx */
                            0, /* distance_min_cm */
                            0 /* distance_max_cm */
                        },
                        { /* active_coding_dataset.das_cfg.ulfm_cfg.ulfc_blend_out.sgw_blend_out_cfg.ARRAY_1 */
                            0, /* phys_sens_id_rx */
                            0, /* phys_sens_id_tx */
                            0, /* distance_min_cm */
                            0 /* distance_max_cm */
                        },
                        { /* active_coding_dataset.das_cfg.ulfm_cfg.ulfc_blend_out.sgw_blend_out_cfg.ARRAY_2 */
                            0, /* phys_sens_id_rx */
                            0, /* phys_sens_id_tx */
                            0, /* distance_min_cm */
                            0 /* distance_max_cm */
                        },
                        { /* active_coding_dataset.das_cfg.ulfm_cfg.ulfc_blend_out.sgw_blend_out_cfg.ARRAY_3 */
                            0, /* phys_sens_id_rx */
                            0, /* phys_sens_id_tx */
                            0, /* distance_min_cm */
                            0 /* distance_max_cm */
                        }
                    },
                    0, /* trailer_hitch_min_dist_cm */
                    0 /* trailer_hitch_max_dist_cm */
                }
            },
            { /* active_coding_dataset.das_cfg.ulsm_cfg */
#ifdef CFGD_ULSD_AK
                { /* active_coding_dataset.das_cfg.ulsm_cfg.thresholds_cfg */
                    {0x7B,0xDF,0x2A,0x5A,0xD2,0x7B,0x14,0x9B}, /* rear_side_psm_upa_t1 */
                    {0x7B,0xDF,0x4B,0x63,0x14,0x7B,0x54,0x9B}, /* rear_side_psm_upa_t2 */
                    {0x7B,0xDF,0x4B,0x63,0x14,0x7B,0x14,0x9B}, /* rear_side_psm_upa_t3 */
                    {0x7B,0xDF,0x4B,0x63,0x13,0x7A,0x8E,0x5B}, /* rear_side_psm_upa_t4 */
                    {0x7B,0xDF,0x3A,0xDE,0xF2,0x7A,0x0A,0x3B}, /* rear_side_psm_upa_t5 */
                    {0x7B,0xDF,0x09,0x52,0xB0,0x7B,0x14,0x9B}, /* rear_outer_upa_t1 */
                    {0x7B,0xDF,0x2A,0x5A,0xF2,0x7B,0x54,0x9B}, /* rear_outer_upa_t2 */
                    {0x7B,0xDF,0x2A,0x5A,0xF2,0x7B,0x14,0x9B}, /* rear_outer_upa_t3 */
                    {0x7B,0xDF,0x2A,0x5A,0xF1,0x7A,0x8E,0x5B}, /* rear_outer_upa_t4 */
                    {0x7B,0xDF,0x19,0xD6,0xD1,0x7A,0x0A,0x3B}, /* rear_outer_upa_t5 */
                    {0x7D,0xAF,0x7B,0xDA,0xB1,0x7B,0x14,0x9B}, /* rear_inner_upa_t1 */
                    {0x7E,0x33,0x9C,0xE2,0xF3,0x7B,0x54,0x9B}, /* rear_inner_upa_t2 */
                    {0x7E,0x33,0x9C,0xE2,0xF3,0x7B,0x14,0x9B}, /* rear_inner_upa_t3 */
                    {0x7E,0x33,0x9C,0xE2,0xF2,0x7A,0x8E,0x5B}, /* rear_inner_upa_t4 */
                    {0x7E,0x31,0x8C,0x5E,0xD2,0x7A,0x0A,0x3B}, /* rear_inner_upa_t5 */
                    {0x94,0xE7,0x7C,0x62,0xF5,0x9B,0x0E,0x5B}, /* front_side_psm_upa_t1 */
                    {0xA5,0x6B,0x9D,0x6B,0x37,0x82,0xD0,0x6B}, /* front_side_psm_upa_t2 */
                    {0xAD,0x6B,0x9D,0x6B,0x37,0x82,0xD0,0x6B}, /* front_side_psm_upa_t3 */
                    {0xA5,0x29,0x8C,0xE6,0xF5,0x91,0xC4,0x2B}, /* front_side_psm_upa_t4 */
                    {0x9C,0xE7,0x7C,0x62,0xD4,0x79,0x44,0x2B}, /* front_side_psm_upa_t5 */
                    {0x7D,0x6D,0x6A,0xDA,0x91,0x83,0x14,0x9B}, /* front_outer_upa_t1 */
                    {0x7D,0xAF,0x7B,0x62,0xD2,0x8B,0x54,0x9B}, /* front_outer_upa_t2 */
                    {0x7D,0xAF,0x8B,0xE2,0xD2,0x8B,0x14,0x9B}, /* front_outer_upa_t3 */
                    {0x7D,0xAF,0x7B,0x5E,0xB1,0x7A,0x8E,0x5B}, /* front_outer_upa_t4 */
                    {0x7D,0x6D,0x6A,0xDA,0x8F,0x7A,0x0A,0x3B}, /* front_outer_upa_t5 */
                    {0x7B,0xDF,0x09,0x4E,0x72,0x83,0x14,0x9B}, /* front_inner_upa_t1 */
                    {0x7B,0xDF,0x19,0xD6,0xB3,0x8B,0x54,0x9B}, /* front_inner_upa_t2 */
                    {0x7B,0xDF,0x2A,0x56,0xB3,0x8B,0x14,0x9B}, /* front_inner_upa_t3 */
                    {0x7B,0xDF,0x19,0xD2,0x92,0x7A,0x8E,0x5B}, /* front_inner_upa_t4 */
                    {0x7B,0xDF,0x09,0x4E,0x70,0x7A,0x0A,0x3B}, /* front_inner_upa_t5 */
#ifdef CFGD_ULSD_BSD
                    {0x6B,0x5A,0xD7,0x46,0xF7,0x82,0x4E,0x39}, /* front_side_bsd_t1 */
#endif
#ifdef CFGD_ULSD_BSD
                    {0x6B,0x5A,0xD7,0x46,0xF7,0x82,0x4E,0x39}, /* front_side_bsd_t2 */
#endif
#ifdef CFGD_ULSD_BSD
                    {0x6B,0x5A,0xD7,0x46,0xF7,0x82,0x4E,0x39}, /* front_side_bsd_t3 */
#endif
#ifdef CFGD_ULSD_BSD
                    {0x6B,0x5A,0xD7,0x46,0xF7,0x82,0x4A,0x39}, /* front_side_bsd_t4 */
#endif
#ifdef CFGD_ULSD_BSD
                    {0x6B,0x5A,0xD7,0x46,0xF7,0x82,0x4A,0x39}, /* front_side_bsd_t5 */
#endif
#ifdef CFGD_ULSD_BSD
                    {0x6B,0x5A,0xD7,0x42,0x52,0x6A,0x90,0x39}, /* rear_outer_bsd_t1 */
#endif
#ifdef CFGD_ULSD_BSD
                    {0x6B,0x5A,0xD7,0x42,0x52,0x6A,0x90,0x39}, /* rear_outer_bsd_t2 */
#endif
#ifdef CFGD_ULSD_BSD
                    {0x6B,0x5A,0xD7,0x42,0x52,0x6A,0x90,0x39}, /* rear_outer_bsd_t3 */
#endif
#ifdef CFGD_ULSD_BSD
                    {0x6B,0x5A,0xD7,0x42,0x52,0x6A,0x90,0x39}, /* rear_outer_bsd_t4 */
#endif
#ifdef CFGD_ULSD_BSD
                    {0x6B,0x5A,0xD7,0x42,0x52,0x6A,0x90,0x39} /* rear_outer_bsd_t5 */
#endif
                },
#endif
#ifdef CFGD_ULSD_HP
                { /* active_coding_dataset.das_cfg.ulsm_cfg.thresholds_hp_cfg */
                    {0xB1,0x4E,0x55,0x56,0x57,0x77,0xB7,0xB1,0xB0,0xAC,0xC8,0xE4,0x02}, /* rear_side_psm_upa_hp_t1 */
                    {0xB1,0x4E,0x55,0x56,0x57,0x77,0xB7,0xB1,0xB0,0xAC,0xC8,0xE4,0x02}, /* rear_side_psm_upa_hp_t2 */
                    {0xB1,0x4E,0x55,0x56,0x57,0x77,0xB7,0xB1,0xB0,0xAC,0xC8,0xE4,0x02}, /* rear_side_psm_upa_hp_t3 */
                    {0xB1,0x4E,0x55,0x56,0x57,0x77,0xB7,0xB1,0xB0,0xAC,0xC8,0xE4,0x02}, /* rear_side_psm_upa_hp_t4 */
                    {0xB1,0x4E,0x55,0x56,0x57,0x77,0xB7,0xB1,0xB0,0xAC,0xC8,0xE4,0x02}, /* rear_side_psm_upa_hp_t5 */
                    {0xB1,0x4E,0x4E,0x51,0x54,0x75,0xB4,0xB2,0xB0,0xAC,0xC5,0xE4,0x02}, /* rear_outer_upa_hp_t1 */
                    {0xB1,0x4E,0x4E,0x51,0x54,0x75,0xB4,0xB2,0xB0,0xAC,0xC5,0xE4,0x02}, /* rear_outer_upa_hp_t2 */
                    {0xB1,0x4E,0x4E,0x51,0x54,0x75,0xB4,0xB2,0xB0,0xAC,0xC5,0xE4,0x02}, /* rear_outer_upa_hp_t3 */
                    {0xB1,0x4E,0x4E,0x51,0x54,0x75,0xB4,0xB2,0xB0,0xAC,0xC5,0xE4,0x02}, /* rear_outer_upa_hp_t4 */
                    {0xB1,0x4E,0x4E,0x51,0x54,0x75,0xB4,0xB2,0xB0,0xAC,0xC5,0xE4,0x02}, /* rear_outer_upa_hp_t5 */
                    {0xB1,0x4F,0x53,0x55,0x57,0x77,0xB5,0xB2,0xAF,0xAE,0xC7,0xE4,0x02}, /* rear_inner_upa_hp_t1 */
                    {0xB1,0x4F,0x53,0x55,0x57,0x77,0xB5,0xB2,0xAF,0xAE,0xC7,0xE4,0x02}, /* rear_inner_upa_hp_t2 */
                    {0xB1,0x4F,0x53,0x55,0x57,0x77,0xB5,0xB2,0xAF,0xAE,0xC7,0xE4,0x02}, /* rear_inner_upa_hp_t3 */
                    {0xB1,0x4F,0x53,0x55,0x57,0x77,0xB5,0xB2,0xAF,0xAE,0xC7,0xE4,0x02}, /* rear_inner_upa_hp_t4 */
                    {0xB1,0x4F,0x53,0x55,0x57,0x77,0xB5,0xB2,0xAF,0xAE,0xC7,0xE4,0x02}, /* rear_inner_upa_hp_t5 */
                    {0xB1,0x51,0x53,0x53,0x73,0xB2,0xAF,0xCB,0xC6,0xC5,0xC5,0xE4,0x02}, /* front_side_psm_upa_hp_t1 */
                    {0xB1,0x51,0x53,0x53,0x73,0xB2,0xAF,0xCB,0xC6,0xC5,0xC5,0xE4,0x02}, /* front_side_psm_upa_hp_t2 */
                    {0xB1,0x51,0x53,0x53,0x73,0xB2,0xAF,0xCB,0xC6,0xC5,0xC5,0xE4,0x02}, /* front_side_psm_upa_hp_t3 */
                    {0xB1,0x51,0x53,0x53,0x73,0xB2,0xAF,0xCB,0xC6,0xC5,0xC5,0xE4,0x02}, /* front_side_psm_upa_hp_t4 */
                    {0xB1,0x51,0x53,0x53,0x73,0xB2,0xAF,0xCB,0xC6,0xC5,0xC5,0xE4,0x02}, /* front_side_psm_upa_hp_t5 */
                    {0xB1,0x51,0x53,0x53,0x75,0x95,0x93,0xB1,0xB0,0xAD,0xC4,0xE2,0x02}, /* front_outer_upa_hp_t1 */
                    {0xB1,0x51,0x53,0x53,0x75,0x95,0x93,0xB1,0xB0,0xAD,0xC4,0xE2,0x02}, /* front_outer_upa_hp_t2 */
                    {0xB1,0x51,0x53,0x53,0x75,0x95,0x93,0xB1,0xB0,0xAD,0xC4,0xE2,0x02}, /* front_outer_upa_hp_t3 */
                    {0xB1,0x51,0x53,0x53,0x75,0x95,0x93,0xB1,0xB0,0xAD,0xC4,0xE2,0x02}, /* front_outer_upa_hp_t4 */
                    {0xB1,0x51,0x53,0x53,0x75,0x95,0x93,0xB1,0xB0,0xAD,0xC4,0xE2,0x02}, /* front_outer_upa_hp_t5 */
                    {0xB1,0x52,0x57,0x58,0x77,0x95,0x92,0xD0,0xCC,0xCC,0xCA,0xE4,0x02}, /* front_inner_upa_hp_t1 */
                    {0xB1,0x52,0x57,0x58,0x77,0x95,0x92,0xD0,0xCC,0xCC,0xCA,0xE4,0x02}, /* front_inner_upa_hp_t2 */
                    {0xB1,0x52,0x57,0x58,0x77,0x95,0x92,0xD0,0xCC,0xCC,0xCA,0xE4,0x02}, /* front_inner_upa_hp_t3 */
                    {0xB1,0x52,0x57,0x58,0x77,0x95,0x92,0xD0,0xCC,0xCC,0xCA,0xE4,0x02}, /* front_inner_upa_hp_t4 */
                    {0xB1,0x52,0x57,0x58,0x77,0x95,0x92,0xD0,0xCC,0xCC,0xCA,0xE4,0x02} /* front_inner_upa_hp_t5 */
                },
#endif
                { /* active_coding_dataset.das_cfg.ulsm_cfg.temp_hyst_cfg */
                    -20, /* threshold_1 */
                    -7, /* threshold_2 */
                    10, /* threshold_3 */
                    20, /* threshold_4 */
                    3 /* hysteresis */
                },
#ifdef CFGD_ULSD_HP_360_SEQUENCES
                { /* active_coding_dataset.das_cfg.ulsm_cfg.speed_hyst_360_cfg */
                    6, /* slow_to_fast_kmh */
                    17, /* fast_to_psm_kmh */
                    26, /* psm_to_full_kmh */
                    24, /* full_to_psm_kmh */
                    15, /* psm_to_fast_kmh */
                    4 /* fast_to_slow_kmh */
                },
#endif
#ifdef CFGD_ULSD_FMS_SEQUENCES
                { /* active_coding_dataset.das_cfg.ulsm_cfg.speed_hyst_fms_cfg */
                    6, /* slow_to_fast_kmh */
                    17, /* fast_to_full_kmh */
                    15, /* full_to_fast_kmh */
                    4 /* fast_to_slow_kmh */
                }
#endif
            },
            { /* active_coding_dataset.das_cfg.objm_cfg */
                { /* active_coding_dataset.das_cfg.objm_cfg.fusl_cfg */
                    40, /* submerging_zone_front_cm */
                    40, /* submerging_zone_rear_cm */
                    20, /* submerging_zone_sides_cm */
                    40, /* submerging_zone_corner_front_cm */
                    40, /* submerging_zone_corner_rear_cm */
                    75, /* submerging_tracing_threshold_cm */
                    0 /* guard_zone_under_vehicle_cm */
                }
            },
            { /* active_coding_dataset.das_cfg.odom_cfg */
#ifdef CFGD_ODOMETRY
                { /* active_coding_dataset.das_cfg.odom_cfg.drdi_cfg */
                    0, /* wheel_cnt_for_sure */
                    0 /* reserve1 */
                },
#endif
                { /* active_coding_dataset.das_cfg.odom_cfg.cast_cfg */
                    400, /* fast_standstill_time_ms */
                    700, /* slow_standstill_time_ms */
#ifdef CFGD_ODOMETRY
                    2, /* nb_pulses_release_fast_standstill */
#endif
#ifdef CFGD_ODOMETRY
                    1 /* nb_pulses_release_slow_standstill */
#endif
                },
                { /* active_coding_dataset.das_cfg.odom_cfg.ooca_cfg */
#ifdef CFGD_SWOF
                    { /* active_coding_dataset.das_cfg.odom_cfg.ooca_cfg.swof_cfg */
                        5, /* max_steer_moment_10th_nm */
                        5460, /* max_wheel_angle_u16f */
                        1138, /* max_wheel_angle_vel_u16f_s */
                        114, /* max_wheel_angle_offset_u16f */
                        10, /* min_delta_offset_to_store_u16f */
                        10, /* min_veh_speed_kmh */
                        17, /* min_veh_speed_to_store_kmh */
                        40, /* max_veh_speed_to_store_kmh */
                        0 /* reserve1 */
                    },
#endif
#ifdef CFGD_WCCA
                    { /* active_coding_dataset.das_cfg.odom_cfg.ooca_cfg.wcca_cfg */
                        { /* active_coding_dataset.das_cfg.odom_cfg.ooca_cfg.wcca_cfg.cfg_x1 */
                            12800, /* min_value */
                            19200, /* max_value */
                            15, /* min_driven_way_m */
                            1, /* max_yaw_rate_grad */
                            2, /* max_wheel_angle_grad */
                            5, /* min_speed_kmh */
                            100 /* max_speed_kmh */
                        },
                        { /* active_coding_dataset.das_cfg.odom_cfg.ooca_cfg.wcca_cfg.cfg_x2 */
                            12800, /* min_value */
                            19200, /* max_value */
                            15, /* min_yaw_rate_grad */
                            40, /* max_yaw_rate_grad */
                            500, /* min_calc_steps */
                            300, /* max_force_100th_m_rad_s_2 */
                            10, /* max_delta_speed_100th_m_s */
                            0, /* speed_factor */
                            10, /* min_speed_kmh */
                            50, /* max_speed_kmh */
                            1 /* min_bins */
                        },
                        { /* active_coding_dataset.das_cfg.odom_cfg.ooca_cfg.wcca_cfg.cfg_avg */
                            32, /* req_diff_reset_mm */
                            5, /* cnt_wheel_circum_min */
                            50, /* cnt_wheel_circum_max */
                            5, /* safety_10thpercent */
                            2, /* min_wheel_circum_deviation_mm */
                            4 /* req_cnt_reset */
                        },
                        -15, /* track_width_offset_mm */
                        1, /* enable_avg_calc */
                        1 /* calc_weight */
                    },
#endif
                    5, /* ooca_estimation_mode */
                    0 /* reserve1 */
                },
#ifdef CFGD_ODOMETRY
                { /* active_coding_dataset.das_cfg.odom_cfg.adj_cfg */
                    530, /* max_angle_allowed */
                    350, /* adj_min_angle_allowed */
                    200, /* adj_min_velocity_time_ms */
                    200, /* adj_task_cnt_sample_time_ms */
                    1000, /* adj_max_task_cnt_time_ms */
                    3, /* adj_min_pulses */
                    0 /* reserve1 */
                },
#endif
#ifdef CFGD_ODOMETRY
                { /* active_coding_dataset.das_cfg.odom_cfg.int_veh_cfg */
                    254, /* odom_max_wheel_pulse_front */
                    254, /* odom_max_wheel_pulse_rear */
                    -8, /* track_width_offset_rear_mm */
                    -65, /* max_veh_ang_correction_si16f */
                    96, /* odom_pulses_per_rotation_front */
                    96, /* odom_pulses_per_rotation_rear */
                    1800, /* min_wheel_circum_mm */ /* KOOL config limits */
                    2160, /* max_wheel_circum_mm */
                    0, /* motion_model */
                    120 /* wheel_angle_delay_ms */
                },
#endif
#ifdef CFGD_EXTERNAL_ODOMETRY
                { /* active_coding_dataset.das_cfg.odom_cfg.ext_veh_cfg */
                    { /* active_coding_dataset.das_cfg.odom_cfg.ext_veh_cfg.ext_veh_pos_cfg */
                        -32768, /* min_val_x_mm */
                        -32768, /* min_val_y_mm */
                        32767, /* max_val_x_mm */
                        32767, /* max_val_y_mm */
                        5, /* pos_noise_at_standstill_mm */
                        1 /* handle_overflow */
                    },
                    0, /* is_dr_dir_available */
                    0 /* is_standstill_available */
                }
#endif
            },
#ifdef CFGD_SLOT_MANAGER
            { /* active_coding_dataset.das_cfg.film_cfg */
                { /* active_coding_dataset.das_cfg.film_cfg.sime_cfg */
                    200, /* par_min_length_cumulated_cm */
                    100, /* perp_min_length_cumulated_cm */
                    150, /* height_cl_dist_x_offset_cm */
                    50, /* height_cl_dist_x_offset_hd_cm */
                    7281, /* ang_to_detect_perp_parking */
                    1820, /* ang_max_orient_passing_u16f */
                    40, /* max_sigma_mm */
                    0, /* min_ratio_length_high */
                    1, /* treat_unknown_as_low */
                    1, /* is_extended_curved_curb_active */
                    0 /* is_beside_low_all_feat_curb */
                },
                { /* active_coding_dataset.das_cfg.film_cfg.sdet_cfg */
                    1200, /* max_allowed_slot_length_cm */
                    400, /* virtual_slot_length_offset_cm */
                    1500, /* search_area_x_max_offset_mm */
                    -1000, /* front_search_area_x_min_mm */
                    3000, /* front_search_area_y_abs_mm */
                    450, /* front_max_allowed_slot_len_cm */
                    4550, /* max_line_orient_error_u16f */
                    5040, /* curve_detection_delta_ang_u16f */
                    6371, /* d_ang_max_pass_ang_relia_u16f */
                    0, /* par_os_to_delay_report_slot_cm */
                    0, /* perp_os_to_delay_report_slot_cm */
                    40, /* max_velocity_slot_det_par */
                    20, /* max_velocity_slot_det_per */
                    70, /* side_search_area_y_abs_dm */
                    10, /* min_len_of_slot_obj_dm */
                    21, /* max_passing_dist_dm */
                    2, /* min_passing_dist_dm */
                    10, /* min_obj_len_orient_relia_dm */
                    20, /* max_obj_len_orient_relia_per_dm */
                    60, /* max_sigma_for_orient_relia_mm */
                    50, /* curve_detection_distance_dm */
                    10, /* max_driven_way_backwards_dm */
                    0 /* is_slot_suppressed_on_low_obj */
                },
                { /* active_coding_dataset.das_cfg.film_cfg.srem_cfg */
                    0, /* max_dist_x_rem_mm */
                    1600, /* boundary_par_max_dist_obj_mm */
                    1500, /* offset_slot_opp_side_y_max_mm */
                    8192, /* max_dang_seg_pal_u16f */
                    8192, /* max_dang_seg_face_u16f */
                    5461, /* max_ang_opp_start_u16f */
                    2730, /* orient_uncertainty_u16f */
                    1800, /* classify_obj_small_max_len_mm */
                    1000, /* seg_len_face_mm */
                    40, /* end_offset_tiltout_end_dm */
                    -5, /* start_offset_tiltout_end_dm */
                    10, /* end_offset_tiltout_start_dm */
                    -50, /* start_offset_tiltout_start_dm */
                    10, /* max_y_dist_rem_dm */
                    21, /* offset_slot_start_uls_cm */
                    0, /* offset_slot_end_uls_cm */
                    20, /* rem_dist_horizontal_dm */
                    10, /* front_sens_clear_dist_cm */
                    10, /* back_sens_clear_dist_cm */
                    15, /* seg_len_confirm_pal_dm */
                    15, /* seg_len_probable_pal_dm */
                    8, /* edg_clear_dist_dm */
                    20, /* max_seg_len_uls_offset_cm */
                    20, /* default_depth_start_dm */
                    20, /* default_depth_end_dm */
                    5, /* depth_orient_check_dm */
                    30, /* quality_depth_line_mm */
                    5, /* num_pts_lin_depth */
                    23, /* length_to_reduce_quality_dm */ /* Changed by Mathieu/Jeff from 30 (3000 mm) */
                    100, /* reduced_quality_depth_line_mm */ /* Changed from 60 as suggested by Mathieu - 05.04.2017 */
                    8, /* max_dx_vseg_dm */
                    5, /* exclude_thresh_corner_dm */
                    0, /* face_uncertainty_cm */
                    31, /* edge_uncertainty_cm */
                    80, /* len_open_orient_cm */
                    100, /* min_dist_to_contour_cm */ /* Changed from 60 by Jeff - 25.04.2017 */
                    1, /* treat_opp_low_as_high */
                    75 /* min_obj_conf_big_rem */
                },
#ifdef CFGD_SLOT_DET_OUTPARKING
                { /* active_coding_dataset.das_cfg.film_cfg.sout_cfg */
                    450, /* pa_min_length_offset_offer_mm */
                    1500, /* pe_sa_width_mm */
                    2000, /* pe_sa_y_offset_tobj_mm */
                    2000, /* max_driven_dist_bef_active_mm */
                    25, /* pe_veh_cont_filter_seg_dist_cm */
                    50, /* pe_width_offset_cm */
                    41, /* pe_min_dist_no_unscanned_dm */
                    30, /* min_dist_opp_dm */
                    25, /* pa_edg_det_debounce_cnt */
                    15, /* pa_offset_slot_extend_cm */
                    70, /* pa_search_height_unsharp_cm */
                    20, /* pa_max_detect_dist_f_dm */
                    20, /* pa_max_detect_dist_r_dm */
                    15, /* pa_max_stable_detect_dist_dm */
                    1, /* chk_low_height_opp_side */
                    0, /* is_slot_suppress_oppo_side */
                    0, /* is_par_slot_oppo_side_unsharp */
                    0 /* reserve1 */
                },
#endif
                { /* active_coding_dataset.das_cfg.film_cfg.past_cfg */
                    0, /* perp_target_depth_offset_cm */
                    1000, /* out_goal_y_offset_flank_mm */
                    2500, /* max_depth_park_mark_par_mm */
                    4500, /* min_length_mark_slot_par_mm */
                    8000, /* max_length_mark_slot_par_mm */
                    2100, /* min_length_mark_slot_pe_mm */
                    4000, /* max_length_mark_slot_pe_mm */
                    12000, /* max_slot_length_perp_mm */
                    200, /* mark_depth_pe_mm */
                    300, /* per_min_dist_to_mark_mm */
                    200, /* par_min_dist_to_mark_mm */
                    12000, /* min_sl_to_ignore_obj_start_mm */
                    4200, /* sl_to_align_to_object_end_mm */
                    400, /* max_allowed_lat_offset_cm */
                    1500, /* max_off_front_sl_corners_mm */
                    600, /* updated_sl_offset_mm */
                    400, /* per_min_dist_to_obj_mm */
                    500, /* max_offset_l_marks_mm */
                    1000, /* max_offset_per_mm */
                    1500, /* perp_max_update_offset_l_mm */
                    910, /* max_angle_mark_side_to_uls_target */
                    500, /* max_dist_lane_side_prim_mark_per_mm */
                    300, /* distance_to_wall_short_slot_mm */
                    1000, /* offset_thres_slot_len_short_mm */
                    60, /* min_sl_offset_par_cm */
                    40, /* max_offset_ps_cm */
                    60, /* max_offset_l_cm */
                    15, /* par_distance_to_curb_cm */
                    30, /* distance_to_wall_cm */
                    30, /* distance_to_wall_driver_par_cm */
                    15, /* perp_distance_to_curb_cm */
                    50, /* tire_safety_distance_cm */
                    10, /* limit_dist_on_object_side_cm */
                    40, /* limit_dist_to_conf_objects_cm */
                    60, /* min_sl_offset_lat_cm */
                    75, /* dist_to_align_to_obj_cm */
                    0, /* max_lat_offset_middle_park_cm */
                    255, /* max_per_offset_middle_park_cm */
                    50, /* dist_left_side_pct */
                    10, /* par_align_offset_no_curb_cm */
                    0, /* par_min_dist_to_obj_cm */
                    0, /* par_min_dist_valid_front */
                    120, /* par_max_dist_to_obj_cm */
                    1, /* par_is_align_front */
                    50, /* par_longi_align_rear_percent */
                    0, /* is_slot_without_start_enabled */
                    0, /* is_slot_without_end_enabled */ /* Changed from 1 by Jeff - 25.04.2017 */
                    30, /* lat_sl_align_hysteresis_cm */
                    1, /* is_object_line_slot_enabled */
                    0, /* is_wide_slot_align_to_start */
                    0, /* is_overcurb_chassis_front_allow */
                    0, /* is_overcurb_chassis_back_allow */
                    0, /* is_overcurb_wheel_allow */
                    0 /* gar_disable_middle_alignment */
                },
                { /* active_coding_dataset.das_cfg.film_cfg.psma_cfg */
                    -1000, /* min_depth_veh_in_front_slot_mm */
                    1500, /* max_dist_para_af_sl_end_cm */
                    900, /* max_dist_perp_af_sl_end_cm */
                    8192, /* max_delta_veh_angle_para_u16f */
                    8192, /* max_delta_veh_angle_perp_u16f */
                    0, /* allow_slots_at_standstill */
                    1, /* allow_slot_objstart_short */
                    1, /* allow_slot_objend_short */
                    0 /* reserve1 */
                },
                0, /* charging_pad_pos_x_mm */
                0, /* charging_pad_pos_y_mm */
                30, /* temp_threshold_low2high */
                0 /* enable_front_slot_on_laser */
            },
#endif
#ifdef CFGD_VCTL_PLATFORM
            { /* active_coding_dataset.das_cfg.vctm_cfg */
#ifdef CFGD_PARKING_PATH_IPO
                { /* active_coding_dataset.das_cfg.vctm_cfg.pipo_cfg */
                    910, /* min_wheel_ang_change_u16_m */
                    255, /* forecast_time_ms */
                    5460, /* max_wheel_ang_vel_u16f_s */
                    22750, /* max_wheel_ang_acc_u16f_s2 */
                    500, /* max_lateral_acc_mm_s2 */
                    100, /* curv2_ipo_width_v1_100th_km */
                    800, /* curv2_ipo_width_v2_100th_km */
                    55, /* curv2_ipo_width_b1_cm */
                    200 /* curv2_ipo_width_b2_cm */
                },
#endif
#ifdef CFGD_LATERAL_CONTROLLER
                { /* active_coding_dataset.das_cfg.vctm_cfg.paco_cfg */
                    {0,100,300,500,800}, /* veh_vel_tbl_100th_kmh */
                    {-16,-13,-10,-8,-7}, /* pole_tbl1 */
                    {-16,-13,-10,-8,-7} /* pole_tbl2 */
                },
#endif
#ifdef CFGD_LATERAL_CONTROLLER
                { /* active_coding_dataset.das_cfg.vctm_cfg.strf_cfg */
                    {7700,7700,7700,7700,7700}, /* limit_wheel_ang_vel_u16f_s_tbl */
                    {28526,28526,28526,28526,28526}, /* limit_wheel_ang_acc_u16f_s2_tbl */
                    3840, /* gain_p_1024th_s */
                    5000, /* time_to_reach_target_ms */
                    1024, /* rfac_limit_wheel_ang_vel_1024th */
                    1250, /* tol_tgt_wheel_ang_vel_si16_s */
                    4000, /* rfac_wheel_ang_gate_u16f */
                    4000, /* limit_dev_wheel_ang_u16f */
                    650, /* overshoot_wheel_ang_u16f */
                    137 /* tol_tgt_wheel_ang_u16f */
                },
#endif
#ifdef CFGD_TORQUE_CONTROLLER
                { /* active_coding_dataset.das_cfg.vctm_cfg.strc_cfg */
                    796, /* max_diff_veh_wheel_ang_u16f */
                    3413, /* max_veh_wheel_ang_vel_u16f_s */
                    5, /* ang_ctrl_gain_p */
                    5, /* ang_ctrl_gain_i */
                    0, /* ang_ctrl_gain_d */
                    0, /* ang_ctrl_gain_v */
                    1500, /* vel_ctrl_gain_p */
                    140, /* vel_ctrl_gain_i */
                    0, /* vel_ctrl_gain_d */
                    57, /* tol_veh_wheel_ang_u16f */
                    57 /* tol_veh_wheel_ang_vel_u16f_s */
                },
#endif
#ifdef CFGD_WAYPOINT_PATH_IPO
                { /* active_coding_dataset.das_cfg.vctm_cfg.tipo_cfg */
                    7181, /* min_limit_wheel_ang_vel_u16f_s */
                    255 /* forecast_ms */
                },
#endif
#ifdef CFGD_LONG_ACC_CTRL
                { /* active_coding_dataset.das_cfg.vctm_cfg.velc_cfg */
                    10000, /* max_vel_mm_s */
                    10000, /* max_acc_mm_s2 */
                    511, /* vel_ctrl_gain_p */
                    0, /* vel_ctrl_gain_i */
                    0, /* vel_ctrl_gain_d */
                    500, /* pos_ctrl_gain_p */
                    0, /* pos_ctrl_gain_d */
                    1, /* tol_pos_mm */
                    1, /* tol_vel_mm_s */
                    1, /* vdyn_type */
                    0 /* reserve1 */
                },
#endif
#ifdef CFGD_LONGI_PROFILE
                { /* active_coding_dataset.das_cfg.vctm_cfg.brake_cfg */
                    2000, /* min_abs_dec_comf_brk_mm_s2 */
                    12000, /* max_abs_dec_em_brk_mm_s2 */
                    500, /* max_abs_dec_sp_brk_mm_s2 */
                    100, /* comf_speed_threshold_mm_s */
                    100, /* comf_dtc_threshold_mm */
                    1, /* braking_actions_per_step */
                    20, /* emergency_brake_pressure_perc */
                    4, /* min_time_to_release_brake_100ms */
                    0 /* reserve1 */
                },
#endif
#ifdef CFGD_LONGI_PROFILE
                { /* active_coding_dataset.das_cfg.vctm_cfg.vpex_cfg */
                    100, /* tol_to_stop_mm */
                    0, /* forecast_time_ms */
                    300, /* a_max_mm_s2 */
                    300, /* j_max_mm_s3 */
                    3000, /* v_max_mm_s */
                    0, /* a_start_mm_s2 */
                    20 /* min_standstill_time_100th_s */
                },
#endif
#ifdef CFGD_CURB_LIFTER
                { /* active_coding_dataset.das_cfg.vctm_cfg.vclf_cfg */
                    500, /* stop_wh_update_dist_mm */
                    40, /* low_vel_100th_kph */
                    800, /* clf_finished_dist_mm */
                    45, /* time_dbnce_aresist_10th_s */
                    1, /* use_int_resistance */
                    1, /* stop_front_all_whs */
                    0 /* reserve1 */
                },
#endif
                {0,25,95,200,420,630,850,1200}, /* default_braking_table_dist_fwd_mm */
                {0,25,95,200,420,630,850,1200}, /* default_braking_table_dist_bwd_mm */
                {250,500,700,900,1400,1750,2200,2700}, /* freedrive_braking_table_dist_fwd_mm */
                {250,500,700,900,1400,1750,2200,2700}, /* freedrive_braking_table_dist_bwd_mm */
                {0,150,250,350,550,650,800,1000}, /* default_brake_table_speed_fwd_100thkmh */
                {0,150,250,350,550,650,800,1000}, /* default_brake_table_speed_bwd_100thkmh */
                {0,250,350,550,650,800,1000,1200}, /* freedrive_brake_table_speed_fwd_100thkmh */
                {0,250,350,550,650,800,1000,1200}, /* freedrive_brake_table_speed_bwd_100thkmh */
#ifdef CFGD_LONGI_PROFILE
                360, /* max_wa_diff_for_acc_u16f */
#endif
                10000, /* max_pos_ctrl_lag_mm */
                3, /* debounce_steer_torque_100_ms */
                255 /* max_steering_torque_10th_nm */
            },
#endif
#ifdef CFGD_MOPL_PLATFORM
            { /* active_coding_dataset.das_cfg.mopm_cfg */
#ifdef CFGD_PARK_PARA_BW
                { /* active_coding_dataset.das_cfg.mopm_cfg.mpab_cfg */
                    {8989,4759,4045}, /* mul_moves_radius_tbl_mm */
                    {364,1092,6189}, /* mul_moves_delta_theta_tbl_u16f */
                    {5123,5562,6000,6600,7600}, /* ps_length_tbl_mm */
                    {5100,5100,5100,5100,6100}, /* traj_radius_tbl_mm */
                    {6189,5825,5750,5700,5461}, /* traj_theta_tbl_u16f */
                    57344, /* theta_pos_ok_min_u16f */
                    8192, /* theta_pos_ok_max_u16f */
                    300, /* corner_pt_offset_min_x */
                    300, /* corner_pt_offset_max_x */
                    1200, /* slot_length_offset_min */
                    2000, /* slot_length_offset_max */
                    2550, /* max_slot_depth_mm */
                    91, /* tolerance_angle_u16f */ /* Changed from 182 - Romain Guerre (10.07.2017) */
                    15, /* tolerance_x_cm */
                    2, /* tolerance_y_cm */ /* Changed from 10 - Romain Guerre suggested 5, Jeff suggested 2 (19.7.2017) */
                    90, /* min_intermediate_length_cm */
                    {44,41,16,1,1}, /* limit_y_target_offset_tbl_cm */
                    25, /* percent_min_radius */
                    50, /* vsector_approaching_10thkmh */
                    40, /* vsector_segment_at_obj_10thkmh */
                    40, /* vsector_free_segment_10thkmh */
                    30, /* vsector_segment_at_end_10thkmh */
                    15, /* offset_goal_to_virt_curb_cm */ /* Changed from 10 cm by Romain (28.7.2017) */
                    1, /* enable_early_pos_ok_strategy */
                    0, /* limit_fwd_move_lateral */
                    10, /* slot_end_corner_safety_x_cm */
                    20, /* par_min_step_length_cm */
                    250, /* par_max_step_length_align_cm */
                    0 /* reserve1 */
                },
#endif
#ifdef CFGD_PARK_PERP_BW
                { /* active_coding_dataset.das_cfg.mopm_cfg.mpeb_cfg */
                    30, /* offset_goal_cm */
                    100, /* min_dist_cc_cm */
                    10, /* offset_start_cm */
                    25, /* gate_dist_for_min_radius_cm */
                    255, /* max_dist_scs_cm */
                    1820, /* gate_ang_for_min_radius_u16f */
                    7000, /* max_dist_out_of_slot_mm */
                    500, /* shift_dist_mm */
                    1000, /* min_dist_opp_side_mm */
                    546, /* tolerance_angle_u16f */
                    200, /* ext_dist_fwd_step_mm */
                    50, /* goal_shift_while_planning_mm */
                    30, /* tolerance_x_cm */
                    12, /* tolerance_y_cm */
                    20, /* percent_min_radius */ /* Changed from 20 - Jeff (19.04.2017), 13 - Jeff (20.04.2017), from 7 - (8.5.2017) */
                    35, /* vsector_path_10thkmh */
                    0, /* slot_end_corner_safety_x_cm */ /* Changed from 26 - Jeff (19.04.2017) */
                    0, /* slot_end_corner_safety_y_cm */ /* Changed from 5 - Jeff (19.04.2017) */
                    60, /* max_dist_obj_goal_reached_cm */
                    10, /* tol_dist_obj_goal_reached_cm */
                    90, /* max_dive_in_dist_dm */
                    0 /* reserve1 */
                },
#endif
#ifdef CFGD_PARK_PERP_FW
                { /* active_coding_dataset.das_cfg.mopm_cfg.mpef_cfg */
                    30, /* offset_goal_cm */
                    160, /* min_dist_cc_cm */
                    5, /* offset_start_cm */
                    0, /* gate_dist_for_min_radius_cm */
                    3000, /* max_dist_scs_cm */
                    2000, /* pass_dist_dirigent_mm */
                    -500, /* pass_dist_half_dirigent_mm */
                    0, /* min_out_dist_to_street_mm */
                    1000, /* max_out_dist_to_street_mm */
                    0, /* max_delta_x_slot_end_posok_mm */
                    16384, /* gate_ang_for_min_radius_u16f */
                    7000, /* max_dist_to_opp_side_mm */
                    4000, /* dist_slot_start_to_path_end_mm */
                    65535, /* max_veh_angle_during_dirig_u16f */
                    182, /* min_delta_angle_dirigent_u16f */
                    182, /* min_delta_angle_half_dirig_u16f */
                    3641, /* min_delta_angle_for_c_end_u16f */
                    3641, /* min_delta_angle_last_arc_u16f */
                    3276, /* min_delta_angle_add_safety_u16f */
                    14563, /* min_delta_ang_goal_posok_u16f */
                    364, /* tolerance_angle_u16f */
                    30, /* tolerance_x_cm */
                    12, /* tolerance_y_cm */
                    20, /* safety_slot_end_big_cm */ /* Changed from 39 - Jeff (19.04.2017) */
                    15, /* safety_slot_end_small_cm */
                    20, /* percent_min_radius */ /* Changed from 20 - Jeff (19.04.2017), from 13 - Jeff (20.04.2017), from 7 - (8.5.2017) */
                    15, /* hint_hysteresis_slot_end_cm */
                    20, /* safety_dist_lane_end_cm */ /* Changed from 30 - Jeff (19.04.2017), from 10 - Jeff (20.04.2017) */
                    15, /* small_steering_reserve_percent */
                    40, /* vsector_fwd_path_10thkmh */
                    35, /* vsector_bwd_path_10thkmh */
                    0, /* max_ds_to_ideal_path_pos_ok_cm */
                    6, /* max_step_number_for_posok */
                    60, /* max_dist_obj_goal_reached_cm */
                    10, /* tol_dist_obj_goal_reached_cm */
                    1 /* is_always_return_pos_ok */
                },
#endif
#ifdef CFGD_PARK_OUT_PARA
                { /* active_coding_dataset.das_cfg.mopm_cfg.moaf_cfg */
                    8192, /* max_vehicle_orientation_u16f */
                    0, /* tgt_wa_to_deactivate_u16f */
                    200, /* off_cp_bwd_mv_fwd_hint_mm */
                    200, /* safety_dist_to_finish_mm */
                    {91,182,455}, /* fwd_move_delta_theta_tbl_u16f */
                    300, /* vsector_path_100thkmh */
                    546, /* tolerance_angle_u16f */
                    40, /* tolerance_x_cm */
                    200, /* tolerance_y_cm */
                    {1,1,1}, /* fwd_move_radius_tbl_pct */
                    10, /* min_dist_scene_cm */
                    1, /* enable_steer_1st_step_back */
                    10, /* assumed_dist_to_curb_cm */
                    1, /* bwd_move_radius_pct */
                    0 /* min_dist_deac_unpark_par */
                },
#endif
#ifdef CFGD_PARK_OUT_PERP_BW
                { /* active_coding_dataset.das_cfg.mopm_cfg.moeb_cfg */
                    15000, /* limit_distance_x_mm */
                    7000, /* limit_distance_y_mm */
                    300, /* vsector_path_100thkmh */
                    546, /* tolerance_angle_u16f */
                    40, /* tolerance_x_cm */
                    200, /* tolerance_y_cm */
                    20, /* percent_min_radius */
                    40, /* street_line_coll_safety_cm */
                    35 /* pct_min_radius_final */
                },
#endif
#ifdef CFGD_PARK_OUT_PERP_FW
                { /* active_coding_dataset.das_cfg.mopm_cfg.moef_cfg */
                    15000, /* limit_distance_x_mm */
                    7000, /* limit_distance_y_mm */
                    300, /* vsector_path_100thkmh */
                    8192, /* min_ang_deactivation */
                    546, /* tolerance_angle_u16f */
                    40, /* tolerance_x_cm */
                    200, /* tolerance_y_cm */
                    20 /* percent_min_radius */
                },
#endif
#ifdef CFGD_PATH_V_PROF_OBJ_DIST
                { /* active_coding_dataset.das_cfg.mopm_cfg.mpdc_cfg */
                    2000, /* max_dist_mm */
                    2222, /* max_speed_mmps */
                    100, /* min_dist_mm */
                    556, /* min_speed_mmps */
                    50 /* offset_dist_mm */
                },
#endif
                { /* active_coding_dataset.das_cfg.mopm_cfg.mclf_cfg */
                    244, /* tire_width_mm */
                    60 /* standstill_before_curb_dist_mm */ /* Changed from 150 - 6.7.2017 (Romain), from 20 - 27.7.2017 (Romain) */
                },
                { /* active_coding_dataset.das_cfg.mopm_cfg.auto_choose_cfg */
                    1092, /* min_wheel_ang_for_pef_u16f */
                    200, /* max_dx_midf_to_sl_for_pef_cm */
                    200, /* offset_depth_for_coll_check_cm */
                    0 /* reserve1 */
                },
                { /* active_coding_dataset.das_cfg.mopm_cfg.safety_dist_cfg */
                    1200, /* add_length_short_slot_mm */
                    1200, /* add_width_short_slot_mm */
                    10, /* reduce_upa_collision_area_cm */
                    20, /* vehicle_safety_side_cm */
                    30, /* vehicle_safety_rear_front_cm */
                    20, /* min_vehicle_safety_cm */
                    40, /* safety_contour_rear_mm */
                    40 /* safety_contour_front_mm */
                },
                { /* active_coding_dataset.das_cfg.mopm_cfg.mopm_intern_cfg */
                    24570, /* per_max_delta_goal_veh_u16f */
                    0, /* min_dist_opp_object_mm */
                    2000, /* min_dtc_park_out_finish */
                    65535, /* max_steering_at_standstill */
                    {4000,4500,5000,5500,6000}, /* steer_back_ps_length_mm */
                    {0,0,0,0,0}, /* steer_back_offset_par_mm */
                    0, /* steer_back_offset_perp_mm */
                    2000, /* max_dth_park_out_mm */
                    100, /* min_dist_emergency_brake_mm */
                    0, /* min_wheel_curb_contact_angle */
                    0, /* min_dist_wheel_over_curb_mm */
                    2730, /* delta_target_ang_u16 */
                    200, /* min_dist_to_hint_mm */
                    1000, /* optimal_dist_to_hint_mm */
                    1000, /* delta_target_longi_mm */
                    20, /* target_dist_offset_longi_mm */
                    500, /* delta_target_lat_mm */
                    0, /* limit_perp_bwd_with_curb */
                    0, /* is_pmo_finish_at_standstill */
                    0, /* ponr_x_offset_cm */
                    100, /* ponr_perp_x_offset_cm */
                    20, /* min_dtc_out_perp_cm */
                    10, /* min_lat_slot_length_offset_cm */
                    10, /* min_par_slot_length_offset_cm */
                    50, /* moaf_max_num_steps */
                    50, /* moeb_max_num_steps */
                    50, /* moef_max_num_steps */
                    13, /* mpab_max_num_steps */
                    13, /* mpeb_max_num_steps */
                    14, /* mpef_max_num_steps */
                    105, /* max_vel_approaching_10th_kph */
                    105, /* max_vel_park_parallel_10th_kph */
                    105, /* max_vel_park_perp_10th_kph */
                    105, /* max_vel_park_me_out_10th_kph */
                    0, /* disable_stt_for_PAB */
                    0, /* disable_stt_for_PEF */
                    0, /* disable_stt_for_PEB */
                    0, /* disable_stt_for_OAF */
                    0, /* disable_stt_for_OEF */
                    0 /* disable_stt_for_OEB */
                }
            },
#endif
#ifdef CFGD_THREAT_ASSESSMENT
            { /* active_coding_dataset.das_cfg.colm_cfg */
#ifdef CFGD_EXPLORATION_MODE
                { /* active_coding_dataset.das_cfg.colm_cfg.mexp_cfg */
                    450, /* min_dist_to_wall_fwd_mm */
                    450, /* min_dist_to_wall_bwd_mm */
                    1500, /* min_wall_length_mm */
                    2730, /* wall_delta_angle_neg_u16f */
                    364, /* wall_delta_angle_pos_u16f */
                    2500, /* max_dist_to_obj_to_steer_mm */
                    300, /* driving_tube_lat_offset_mm */
                    350, /* dist_lat_to_object_mm */
                    100, /* min_dist_lat_to_object_mm */
                    200, /* dist_longi_to_object_mm */
                    200, /* incr_drv_dist_to_steer_mm */
                    5460, /* max_wheel_angle_u16f */
                    3000, /* min_straight_dist_opposite_mm */
                    500, /* min_dist_to_wall_on_driver_sid */
                    400, /* max_spatial_uncertainty_mm */
                    0 /* is_driver_on_right */
                },
#endif
#ifdef CFGD_EMGY_BRAKE_FREE_DRIVE
                { /* active_coding_dataset.das_cfg.colm_cfg.iebm_cfg */
                    0, /* max_acc_for_pred_brak_mms2 */
                    2500, /* max_speed_100thkmh */
                    0, /* time_for_hard_to_avoid_area_ms */ /* Changed from 1400 to 0 - Ahmed 22.06.2017 */
                    200, /* time_for_prefill_ms */
                    0, /* time_for_pred_brake_ms */
                    0, /* ped_dtc_offset_mm */
                    1, /* min_alerts_to_brake */
                    1, /* min_num_points_in_dr_tube_fwd */
                    1, /* min_num_points_in_dr_tube_bwd */
                    0 /* avoid_obst_percent_brk_dist */
                },
#endif
#ifdef CFGD_COMF_BRAKE_FREE_DRIVE
                { /* active_coding_dataset.das_cfg.colm_cfg.cobr_cfg */
                    1800, /* cb_max_speed_100thkmh */
                    1400, /* cb_time_for_hard_to_avoid_area_ms */
                    1 /* cb_obstacle_hard_to_avoid */
                },
#endif
                { /* active_coding_dataset.das_cfg.colm_cfg.thas_cfg */
                    { /* active_coding_dataset.das_cfg.colm_cfg.thas_cfg.thas_mod_cfg */
                        { /* active_coding_dataset.das_cfg.colm_cfg.thas_cfg.thas_mod_cfg.thas_iebm_cfg */
                            0, /* min_first_det_dist_mm */
                            {1400,2800,2800}, /* criticality_tbl_ttc_ms */
                            65535, /* max_dist_outside_uncertainty_mm */
                            0, /* bumper_exclude_area_offset_mm */
                            0, /* sides_exclude_area_offset_mm */
                            0, /* min_drv_dist_check_source_mm */
                            65535, /* max_obst_age_ms */
                            80, /* min_confidence_level_front_rear */
                            80, /* min_confidence_level_side */
                            1, /* obst_heights_to_be_considered */
                            70, /* min_confidence_level_pedestrian */
                            7, /* obst_types_to_be_considered */
                            1, /* tracking_status_front_rear */
                            1, /* tracking_status_sides */
                            0, /* source_of_info_mask */
                            2 /* min_num_sources */
                        },
                        { /* active_coding_dataset.das_cfg.colm_cfg.thas_cfg.thas_mod_cfg.thas_hapt_cfg */
                            0, /* min_first_det_dist_mm */
                            {500,500,500}, /* criticality_tbl_ttc_ms */
                            65535, /* max_dist_outside_uncertainty_mm */
                            0, /* bumper_exclude_area_offset_mm */
                            0, /* sides_exclude_area_offset_mm */
                            0, /* min_drv_dist_check_source_mm */
                            65535, /* max_obst_age_ms */
                            50, /* min_confidence_level_front_rear */
                            50, /* min_confidence_level_side */
                            1, /* obst_heights_to_be_considered */
                            70, /* min_confidence_level_pedestrian */
                            1, /* obst_types_to_be_considered */
                            1, /* tracking_status_front_rear */
                            1, /* tracking_status_sides */
                            0, /* source_of_info_mask */
                            1 /* min_num_sources */
                        },
                        { /* active_coding_dataset.das_cfg.colm_cfg.thas_cfg.thas_mod_cfg.thas_cobr_cfg */
                            0, /* min_first_det_dist_mm */
                            {1400,2800,2800}, /* criticality_tbl_ttc_ms */
                            65535, /* max_dist_outside_uncertainty_mm */
                            0, /* bumper_exclude_area_offset_mm */
                            0, /* sides_exclude_area_offset_mm */
                            0, /* min_drv_dist_check_source_mm */
                            65535, /* max_obst_age_ms */
                            80, /* min_confidence_level_front_rear */
                            80, /* min_confidence_level_side */
                            1, /* obst_heights_to_be_considered */
                            70, /* min_confidence_level_pedestrian */
                            7, /* obst_types_to_be_considered */
                            1, /* tracking_status_front_rear */
                            1, /* tracking_status_sides */
                            0, /* source_of_info_mask */
                            1 /* min_num_sources */
                        }
                    },
                    100, /* tol_for_dyn_brake_coll_area_mm */
                    0, /* driving_direction_strategy */
                    0 /* reserve1 */
                },
                { /* active_coding_dataset.das_cfg.colm_cfg.colm_intern_cfg */
                    0, /* drv_dist_to_activate_exp_mm */
                    15000, /* max_driven_dist_for_exp_mm */
                    0, /* safety_dist_front_rear_mm */
                    0, /* safety_dist_side_mm */ /*  Changed from 0 to 150 - Ahmed 22.6.2017 */
                    0, /* safety_distance_mirrors_mm */
                    1, /* use_mirrors */
                    50 /* max_speed_for_exp_10thkph */
                }
            }
#endif
        },
        { /* active_coding_dataset.logic_cfg */
            37, /* v_on_parallel_kmh */
            17, /* v_on_perpend_kmh */
            50, /* v_off_2_kmh */
            30, /* timer_sensors_off_1s */
            36, /* timer_hmi_global_10s */
            5, /* timer_hmi_text_message_can_1s */
            0, /* blinker_mode */
            7, /* hmi_flags */
            3, /* display_status_count */
            50, /* button_long_press_time_20ms */
            15, /* brake_req_min_time1_100ms */
            8, /* brake_req_min_time2_100ms */
            100, /* bg_max_sync_dist_cm */
            3 /* bg_min_add_segments */
        }
    }

};
#endif
#endif
