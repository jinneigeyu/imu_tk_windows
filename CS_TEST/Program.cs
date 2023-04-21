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
            string imu_txt = "D:/WechatFiles/WeChat Files/youxiangnidelian/FileStorage/File/2023-04/lo(1).txt";

            CaliParam caliParam = new CaliParam()
            {
                timeStyle = TimeStyle.UNIT_NS,
                startOffsetSeconds = 0,
                staticIntervalDuration = 5,
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
