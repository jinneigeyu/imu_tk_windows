using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CS_TEST
{
    class Program
    {
        static void Main(string[] args)
        {
            
            string  imu_txt = "D:\\PojectRespos\\imu_cali_windows\\data\\2023_04_27\\output\\ref_imu_data.txt";
            CaliParam caliParam = new CaliParam()
            {
                timeStyle = TimeStyle.UNIT_US,
                staticIntervalDuration = 5.0,
                gravityMagnitude = 9.7833
            };

            CaliResult caliResult = new CaliResult()
            {
                acc_result = new ReusltStruct(),
                gyro_result = new ReusltStruct()
            };

            CPP_IMU.cali_imu(imu_txt, caliParam, ref caliResult);

            Console.WriteLine(caliResult.acc_result);
            Console.ReadKey();
        }
    }
}
