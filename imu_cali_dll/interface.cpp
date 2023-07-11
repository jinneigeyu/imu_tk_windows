#include "interface.h"
//#include "Eigen/Eigen"

using namespace imu_tk;
using namespace std;
using namespace Eigen;

static  void  convertResult(const CalibratedTriad& acc, const  CalibratedTriad& gyro, CaliResult& des)
{
	memcpy(des.acc_result.mis_mat, acc.getMisalignmentMatrix().data(), sizeof(double) * 9);
	memcpy(des.acc_result.scale_mat, acc.getScaleMatrix().data(), sizeof(double) * 9);
	memcpy(des.acc_result.baise_vec, acc.getBiasVector().data(), sizeof(double) * 3);



	memcpy(des.gyro_result.mis_mat, gyro.getMisalignmentMatrix().data(), sizeof(double) * 9);
	memcpy(des.gyro_result.scale_mat, gyro.getScaleMatrix().data(), sizeof(double) * 9);
	memcpy(des.gyro_result.baise_vec, gyro.getBiasVector().data(), sizeof(double) * 3);

}

int cali_imu(const char* imuFile, CaliParam caliParam, CaliResult& caliResult)
{
	imu_tk::TimestampUnit  timeStampUnit = (imu_tk::TimestampUnit)(caliParam.timeStyle);

	vector< TriadData > acc_data, gyro_data;
	importAsciiDataTimeOffset(imuFile, acc_data, gyro_data,
		timeStampUnit,
		imu_tk::DATASET_SPACE_SEPARATED,
		caliParam.startOffsetSeconds);

	if (acc_data.size()==0)
	{
		return -1;
	}

	CalibratedTriad init_acc_calib, init_gyro_calib;
	init_acc_calib.setBias(Vector3d(caliParam.init_baise[0], caliParam.init_baise[1], caliParam.init_baise[2]));
	init_gyro_calib.setScale(Vector3d(caliParam.init_scale[0], caliParam.init_scale[1], caliParam.init_scale[2]));


	MultiPosCalibration mp_calib;
	mp_calib.setShowPlot(caliParam.show_plot);
	mp_calib.setInitStaticIntervalDuration(caliParam.staticIntervalDuration);
	mp_calib.setintervalVarianceStarEnd(35, 36);
	mp_calib.setInitAccCalibration(init_acc_calib);
	mp_calib.setInitGyroCalibration(init_gyro_calib);
	mp_calib.setGravityMagnitude(caliParam.gravityMagnitude);
	mp_calib.enableVerboseOutput(true);
	mp_calib.enableAccUseMeans(false);
	bool ok = mp_calib.calibrateAccGyro(acc_data, gyro_data);

	auto accrcali_result = mp_calib.getAccCalib();
	auto gyrocali_result = mp_calib.getGyroCalib();
	convertResult(accrcali_result, gyrocali_result, caliResult);

	if (!ok)
		return -1;

	return 0;
}
