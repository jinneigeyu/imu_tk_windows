using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace CS_TEST
{
    class Program
    {
        static void Main(string[] args)
        {

            RunImu720(@"D:\PojectRespos\imu_cali_windows\data\data_20230705\IMU_RAW\ref_imudata.txt");
            Console.ReadKey();
            //RunImu720(@"D:\PojectRespos\imu_cali_windows\data\data_20230616_2_new_pos\ref_imudata_143544.txt");
            //RunImu720(@"D:\PojectRespos\imu_cali_windows\data\data_20230621_imu_all\ref_imu\20230621\140927ref_imudata.txt");

            //RunImuServic(@"D:\PojectRespos\imu_cali_windows\data\data_20230621_imu_all\ref_imu\20230621\140927ref_imudata.txt");

            //RumTONLYDir(@"D:\PojectRespos\imu_cali_windows\data\data_20230621_imu_all\GT23140002020_left\OnLine\20230621");


            //RunImuTonly(@"D:\PojectRespos\imu_cali_windows\data\data_20230621_imu_all\GT23140002020_right\OnLine\20230621\140927right_imudata.txt");

            //Console.WriteLine("input need cali dir or path");
            //string path = Console.ReadLine();
            //if (File.Exists(path))
            //{
            //    Console.WriteLine($"{path} is a file");
            //    RumTONLYDir(path);
            //}
            //else if (Directory.Exists(path))
            //{
            //    Console.WriteLine($"{path} is a folder");
            //    RumTONLYDir(path);
            //}
            //else
            //{
            //    Console.WriteLine("path not exits");
            //}

        }



        static void RunImu720_Dir(string imu_txt_dir, string filter = ".txt")
        {
            if (!Directory.Exists(imu_txt_dir))
            {
                Console.WriteLine($"not exit dir {imu_txt_dir}");
                return;
            }

            var imus = Directory.GetFiles(imu_txt_dir).ToList();

            foreach (var item in imus)
            {
                if (item.EndsWith(filter))
                {
                    RunImu720(item);
                    Console.ReadKey();
                }

            }
        }

        static void RunImu720(string imu_txt)
        {
            CaliParam caliParam = new CaliParam()
            {
                timeStyle = TimeStyle.UNIT_US,
                startOffsetSeconds = 0,
                staticIntervalDuration = 5.0,
                gravityMagnitude = 9.7833
            };

            CaliResult caliResult = new CaliResult()
            {
                acc_result = new ReusltStruct(),
                gyro_result = new ReusltStruct()
            };

            int ok = CPP_IMU.cali_imu(imu_txt, caliParam, ref caliResult);


            if (ok == 0)
            {
                string dir = Path.GetDirectoryName(imu_txt);
                string name = Path.GetFileNameWithoutExtension(imu_txt);
                string result = JsonConvert.SerializeObject(caliResult);
                SaveMyJson(Path.Combine(dir, name + "_cali_result.json"), result);
            }

            
        }


        private static void SaveMyJson(string desktopPath, string json)
        {
            using (FileStream fs = new FileStream(desktopPath, FileMode.Create))
            {
                //写入
                using (StreamWriter sw = new StreamWriter(fs))
                {
                    sw.WriteLine(json);
                }

            }
        }


        private static void RumTONLYDir(string imu_txt_dir, string filter = ".txt")
        {
            if (!Directory.Exists(imu_txt_dir))
            {
                Console.WriteLine($"not exit dir {imu_txt_dir}");
                return;
            }

            var imus = Directory.GetFiles(imu_txt_dir).ToList();

            foreach (var item in imus)
            {
                if (item.EndsWith(filter))
                {
                    RunImuTonly(item);
                    //Console.ReadKey();
                }

            }
        }

        static void RunImuTonly(string imu_txt)
        {
            CaliParam caliParam = new CaliParam()
            {
                timeStyle = TimeStyle.UNIT_NS,
                startOffsetSeconds = 0,
                staticIntervalDuration = 4.5,
                gravityMagnitude = 9.7833
            };

            CaliResult caliResult = new CaliResult()
            {
                acc_result = new ReusltStruct(),
                gyro_result = new ReusltStruct()
            };

            int ok = CPP_IMU.cali_imu(imu_txt, caliParam, ref caliResult);

            if (ok==0)
            {
                string dir = Path.GetDirectoryName(imu_txt);
                string name = Path.GetFileNameWithoutExtension(imu_txt);
                string result = JsonConvert.SerializeObject(caliResult);
                SaveMyJson(Path.Combine(dir, name + "_cali_result.json"), result);
            }

            
        }
    }
}
