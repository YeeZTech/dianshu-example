#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(sjdx_user_number, std::string);
define_nt(sjdx_relate_number, std::string);
define_nt(sjdx_time, int);
define_nt(sjdx_date, std::string);
typedef ff::util::ntobject<sjdx_user_number, sjdx_relate_number, sjdx_time,
                           sjdx_date>
    sjdx_log_item_t;

define_nt(sjhd_user_number, std::string);
define_nt(sjhd_relate_number, std::string);
define_nt(sjhd_start_time, int);
define_nt(sjhd_end_time, int);
define_nt(sjhd_date, std::string);
typedef ff::util::ntobject<sjhd_user_number, sjhd_relate_number,
                           sjhd_start_time, sjhd_end_time, sjhd_date>
    sjhd_log_item_t;

define_nt(sjsw_user_number, std::string);
define_nt(sjsw_start_time, int);
define_nt(sjsw_end_time, int);
define_nt(sjsw_app_id, int);
define_nt(sjsw_tx_pkts, int);
define_nt(sjsw_tx_bytes, int);
define_nt(sjsw_rx_pkts, int);
define_nt(sjsw_rx_bytes, int);
typedef ff::util::ntobject<sjsw_user_number, sjsw_start_time, sjsw_end_time,
                           sjsw_app_id, sjsw_tx_pkts, sjsw_tx_bytes,
                           sjsw_rx_pkts, sjsw_rx_bytes>
    sjsw_log_item_t;

define_nt(xlgj_user_number, std::string);
define_nt(xlgj_uli, std::string);
define_nt(xlgj_uli_addr, std::string);
define_nt(xlgj_lon, float);
define_nt(xlgj_lat, float);
define_nt(xlgj_geohash, std::string);
define_nt(xlgj_areacode, std::string);
define_nt(xlgj_timestamp, int);
define_nt(xlgj_date, std::string);
define_nt(xlgj_hour, int);
define_nt(xlgj_duration, int);
typedef ff::util::ntobject<xlgj_user_number, xlgj_uli, xlgj_uli_addr, xlgj_lon,
                           xlgj_lat, xlgj_geohash, xlgj_areacode,
                           xlgj_timestamp, xlgj_date, xlgj_hour, xlgj_duration>
    xlgj_item_t;
