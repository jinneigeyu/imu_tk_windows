#include <iostream>

#include "imu_tk/io_utils.h"
#include "imu_tk/calibration.h"
#include "imu_tk/filters.h"
#include "imu_tk/integration.h"
#include "imu_tk/visualization.h"
#include<string>
#include "Param.hpp"
#include "cmdparser.hpp"
#include <nlohmann/json.hpp>
#include <nlohmann/byte_container_with_subtype.hpp>
#include <boost/filesystem.hpp> 

using namespace std;
using namespace imu_tk;
using namespace Eigen;
using namespace	nlohmann;

void sean()
{
	vector< TriadData > acc_data, gyro_data;
	string imu_data_file = "D:\\PojectRespos\\imu_cali\\imu_tk_win_sean\\data\\4\\imu.txt";
	importAsciiData(imu_data_file.c_str(), acc_data, gyro_data, imu_tk::TIMESTAMP_UNIT_MSEC, imu_tk::DATASET_SPACE_SEPARATED);
	CalibratedTriad init_acc_calib, init_gyro_calib;
	init_acc_calib.setBias(Vector3d(0, 0, 0));
	init_gyro_calib.setScale(Vector3d(1.0, 1.0, 1.0));
	MultiPosCalibration mp_calib;

	mp_calib.setInitStaticIntervalDuration((double)7.0);             //设置初始化时间50s

	mp_calib.setInitGyroCalibration(init_gyro_calib);       //设置标定 陀螺仪参数对象
	mp_calib.setInitAccCalibration(init_acc_calib);         //设置标定 加速度计参数对象
	mp_calib.setGravityMagnitude(9.81744);                    //设置重力常数
	mp_calib.enableVerboseOutput(true);                       //日志输出
	mp_calib.enableAccUseMeans(false);                        //不使用均值来标定加速度计
	//mp_calib.setGyroDataPeriod(0.01);
	mp_calib.calibrateAccGyro(acc_data, gyro_data);          //标定
	mp_calib.getAccCalib().save("test_imu_acc.calib");
	mp_calib.getGyroCalib().save("test_imu_gyro.calib");


}


string cfg_default = "D:\\PojectRespos\\imu_tk_app\\bin\\Release\\ClientTempRecv\\temp_file_16708235262568455\\config.json";

void configure_parser(cli::Parser& parser)
{
	AlgParam temp;
	json tj = temp;
	std::string description = "a json file path ,that for Algorithm input params";


	//parser.set_required<std::string>("cfg", "config", description);
	parser.set_optional<std::string>("cfg", "config", cfg_default, description);
	//parser.set_optional<std::string>("m", "mode ", "tick", "mode in ['async','tick,'fly']");
	//parser.set_optional<double>("v", "velocity ", 0.1, "robot move speed : default is 0.1 m/s");
	//parser.set_optional<double>("a", "acc ", 0.1, "robot move acceloration : default is 0.1 m/s^2");
	//parser.set_optional<double>("b", "blend ", 0.0, "robot move blend : default is 0.0 m/s");
	//parser.set_optional<uint8_t>("io", "ioID ", 0, "robot standard trigger io id ,default is 0 , span in [0,7]");

}

void resolveParam(string jsonFile,AlgParam& param)
{
	// json help https://json.nlohmann.me/api/basic_json/find/#notes
	// https://github.com/nlohmann/json#json-as-first-class-data-type

	std::ifstream f(jsonFile);
	if (f.good())
	{
		std::cout <<"read config at: " << jsonFile << std::endl;
	}
	json js = json::parse(f);
	param = js.get<AlgParam>();		
	std::cout << "param info" << std::endl;
	std::cout << js << std::endl;
}

bool  RunAlg( const AlgParam& param , const boost::filesystem::path cfgfolder)
{
	// json input imu path is relative
	// convert to real imu file path
	string imu_file = (cfgfolder/ param.inputImuFile).string();

	vector< TriadData > acc_data, gyro_data;

	importAsciiDataTimeOffset(imu_file.c_str(), acc_data, gyro_data, imu_tk::TIMESTAMP_UNIT_MSEC, imu_tk::DATASET_SPACE_SEPARATED,param.startTimeSecondsOffset);

	CalibratedTriad init_acc_calib, init_gyro_calib;

	init_acc_calib.setBias( Vector3d(param.baise[0], param.baise[1], param.baise[2]));
	init_gyro_calib.setScale(Vector3d(param.scale[0], param.scale[1], param.scale[2]));
	MultiPosCalibration mp_calib;

	mp_calib.setShowPlot(param.showplot);
	mp_calib.setInitStaticIntervalDuration(param.initTimeDurationSeconds);
	mp_calib.setintervalVarianceStarEnd(param.intervalVarianceStart, param.intervalVarianceEnd);
	mp_calib.setInitAccCalibration(init_acc_calib);
	mp_calib.setInitGyroCalibration(init_gyro_calib);
	mp_calib.setGravityMagnitude(param.gravityMagnitude);
	mp_calib.enableVerboseOutput(true);
	mp_calib.enableAccUseMeans(false);
	mp_calib.calibrateAccGyro(acc_data, gyro_data);
	auto accrcali_result = mp_calib.getAccCalib();
	auto gyrocali_result = mp_calib.getGyroCalib();

	accrcali_result.save((cfgfolder / param.accResultFilePath).string());
	gyrocali_result.save((cfgfolder / param.gyroResultFilePath).string());
}




int main(int argc, char** argv)
{
	try
	{
		cli::Parser parser(argc, argv);
		configure_parser(parser);
		bool hasHelp = parser.has_help();
		parser.run_and_exit_if_error();


		string jsf = parser.get<std::string>("cfg");
		if (jsf.empty())
		{
			AlgParam temp;
			json tj = temp;
			std::string description = "a json file path ,that for AlgParam like : \n" + tj.dump();
			throw  std::exception(description.c_str());
		}

		AlgParam param;
		resolveParam(jsf, param);
		//param.showplot = true;
		boost::filesystem::path parent_dir = boost::filesystem::path(jsf).parent_path();
		RunAlg(param, parent_dir);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		return 0;
	}
	
	return 1;

	//string imu_file = "D:/PojectRespos/imu_cali/imu_tk/bin/test_data/data/10_kim/imu.txt";

	//vector< TriadData > acc_data, gyro_data;
	////vector< TriadData > acc_datacp, gyro_datacp;
	//importAsciiData(imu_file.c_str(), acc_data, gyro_data, imu_tk::TIMESTAMP_UNIT_MSEC, imu_tk::DATASET_SPACE_SEPARATED);
	////Plot plot;
	////plot.plotSamples(acc_data);
	//CalibratedTriad init_acc_calib, init_gyro_calib;

	////init_acc_calib.setBias( Vector3d(32768, 32768, 32768) );
	////init_gyro_calib.setScale( Vector3d(1.0/6258.0, 1.0/6258.0, 1.0/6258.0) );

	//init_acc_calib.setBias(Vector3d(0, 0, 0));
	//init_gyro_calib.setScale(Vector3d(1, 1, 1));


	//MultiPosCalibration mp_calib;

	//mp_calib.setInitStaticIntervalDuration((double)12);
	//mp_calib.setInitAccCalibration(init_acc_calib);
	//mp_calib.setInitGyroCalibration(init_gyro_calib);
	//mp_calib.setGravityMagnitude(9.81744);
	//mp_calib.enableVerboseOutput(true);
	//mp_calib.enableAccUseMeans(false);
	////mp_calib.setGyroDataPeriod(0.01);
	//mp_calib.calibrateAccGyro(acc_data, gyro_data);

	//mp_calib.getAccCalib().save("test_imu_acc.calib");
	//mp_calib.getGyroCalib().save("test_imu_gyro.calib");


	return 0;
}