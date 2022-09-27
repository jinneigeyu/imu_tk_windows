#include <iostream>

#include "imu_tk/io_utils.h"
#include "imu_tk/calibration.h"
#include "imu_tk/filters.h"
#include "imu_tk/integration.h"
#include "imu_tk/visualization.h"
#include<string>

using namespace std;
using namespace imu_tk;
using namespace Eigen;

void sean()
{
    vector< TriadData > acc_data, gyro_data;
    string imu_data_file = "D:\\PojectRespos\\imu_cali\\imu_tk_win_sean\\data\\4\\imu.txt";
    importAsciiData(imu_data_file.c_str() , acc_data, gyro_data, imu_tk::TIMESTAMP_UNIT_MSEC, imu_tk::DATASET_SPACE_SEPARATED);
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

int main(int argc, char** argv)
{
    string imu_file = "D:/PojectRespos/imu_cali/imu_tk/bin/test_data/data/10_kim/imu.txt";

    vector< TriadData > acc_data, gyro_data;
    //vector< TriadData > acc_datacp, gyro_datacp;
    importAsciiData(imu_file.c_str(), acc_data, gyro_data, imu_tk::TIMESTAMP_UNIT_MSEC, imu_tk::DATASET_SPACE_SEPARATED);

    //for (int i = 0; i < acc_datacp.size(); i++)
    //{
    //    acc_data.push_back(acc_datacp[i]);
    //    gyro_data.push_back(gyro_datacp[i]);
    //}
    //

    Plot plot;
    plot.plotSamples(acc_data);

  /*
  cout<<"Importing IMU data from the Matlab matrix file : "<< argv[1]<<endl;  
  importAsciiData(argv[1], acc_data, imu_tk::TIMESTAMP_UNIT_SEC);
  //importAsciiData( argv[1], acc_data, imu_tk::TIMESTAMP_UNIT_SEC );
  cout<<"Importing IMU data from the Matlab matrix file : "<< argv[2]<<endl;  
  importAsciiData( argv[2], gyro_data, imu_tk::TIMESTAMP_UNIT_SEC  ); */
  

  
  CalibratedTriad init_acc_calib, init_gyro_calib;
  //init_acc_calib.setBias( Vector3d(32768, 32768, 32768) );
  //init_gyro_calib.setScale( Vector3d(1.0/6258.0, 1.0/6258.0, 1.0/6258.0) );

  init_acc_calib.setBias(Vector3d(0, 0, 0));
  init_gyro_calib.setScale(Vector3d(1, 1, 1));

  
  MultiPosCalibration mp_calib;
  
  mp_calib.setInitStaticIntervalDuration((double)12);
  mp_calib.setInitAccCalibration( init_acc_calib );
  mp_calib.setInitGyroCalibration( init_gyro_calib );  
  mp_calib.setGravityMagnitude(9.81744);
  mp_calib.enableVerboseOutput(true);
  mp_calib.enableAccUseMeans(false);
  //mp_calib.setGyroDataPeriod(0.01);
  mp_calib.calibrateAccGyro(acc_data, gyro_data );
  mp_calib.getAccCalib().save("test_imu_acc.calib");
  mp_calib.getGyroCalib().save("test_imu_gyro.calib");
  
//   for( int i = 0; i < acc_data.size(); i++)
//   {
//     cout<<acc_data[i].timestamp()<<" "
  //         <<acc_data[i].x()<<" "<<acc_data[i].y()<<" "<<acc_data[i].z()<<" "
  //         <<gyro_data[i].x()<<" "<<gyro_data[i].y()<<" "<<gyro_data[i].z()<<endl;
//   }
//   cout<<"Read "<<acc_data.size()<<" tuples"<<endl;
  
  return 0;
}