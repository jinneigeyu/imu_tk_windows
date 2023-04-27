using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CS_TEST
{

    public enum TimeStyle
    {
        UNIT_S = 1,
        UNIT_MS = 1000,
        UNIT_US = 1000000,
        UNIT_NS = 1000000000,
    }

    [StructLayout(LayoutKind.Sequential)]
    public class CaliParam
    {
        
        public TimeStyle timeStyle = TimeStyle.UNIT_MS; //时间单位

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public double[] init_baise = new double[3] { 0, 0, 0 }; // 初始acc偏差 设为默认 [0,0,0]

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public double[] init_scale = new double[3] { 1, 1, 1 }; // 初始acc偏差 设为默认 [1,1,1]
        public double startOffsetSeconds = 0;// imu数据前  startOffsetSeconds 秒内的数据为丢去  单位 ：秒
        public double staticIntervalDuration = 5;  // 切除丢弃数据后的静止时间 单位 ：秒
        public bool show_plot = false; // 默认为false 不要开启
        public double gravityMagnitude = 9.81744;  // 当地重力加速度的值，精确值需要客户查表 然后写入
    }

    [StructLayout(LayoutKind.Sequential)]
    public class ReusltStruct
    {

        /// <summary>
        /// 轴偏
        ///  实际为 3x3 矩阵   每个3个值为 一列  排列到3*3
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst =9)]
        public double[] mis_mat = new double[9];

        /// <summary>
        /// 尺度 
        ///   实际为 3x3 矩阵   每个3个值为 一列  排列到3*3
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]
        public double[] scale_mat = new double[9];

        /// <summary>
        /// 偏置
        /// 3*1 向量
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public double[] baise_vec = new double[3];
    };

    [StructLayout(LayoutKind.Sequential)]
    public struct CaliResult
    {
        public ReusltStruct acc_result ;
        public ReusltStruct gyro_result;
    }

    public class CPP_IMU
    {
        private const string LIB_NAME = @"imu_cali.dll";
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public extern static int cali_imu(string imuFile, CaliParam caliParam, ref CaliResult caliResult);
    }
}
