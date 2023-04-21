#pragma once
#include <iostream>
#include "imu_tk/io_utils.h"
#include "imu_tk/calibration.h"
#include "imu_tk/filters.h"
#include "imu_tk/integration.h"
#include "imu_tk/visualization.h"

enum class TimeStyle
{
	UNIT_S = 1,
	UNIT_MS = 1000,
	UNIT_US = 1000000,
	UNIT_NS = 1000000000,
};


struct CaliParam
{
	TimeStyle timeStyle = TimeStyle::UNIT_MS;
	double init_baise[3] = { 0,0,0 };
	double init_scale[3] = { 0,0,0 };
	double startOffsetSeconds = 3.75;
	double staticIntervalDuration = 11.25;  // unit is seconds
	bool show_plot = false;
	double gravityMagnitude = 9.8;  // g 
};

struct ReusltStruct
{
	double mis_mat[9] = { 0 };
	double scale_mat[9] = { 0 };
	double baise_vec[3] = { 0 };
};

struct CaliResult
{
	ReusltStruct acc_result;
	ReusltStruct gyro_result;
};

int cali_imu(const char* imuFile, CaliParam caliParam, CaliResult& caliResult);
