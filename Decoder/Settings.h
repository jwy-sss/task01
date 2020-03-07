#pragma once
#include <string>

// 方便输出部分
#define span " "

// 方块边长定义部分
#define G_XSPLIT G_xsplit
#define G_YSPLIT G_ysplit
#define G_BORDERWIDE G_border
#define G_CELLSIZE G_cellsize

#define G_BLOCK_X G_xlocation
#define G_BLOCK_Y G_ylocation

// 路径设定区
// Area Of Paths
#define PATHPrefix		string("E:\\NET-Program-Output\\v1.0\\")
#define DEC_INPUTPATH	(PATHPrefix + "DecImg\\")
#define DEC_PROC_PNG	(DEC_INPUTPATH + "Dec%d.jpg")
#define DEC_OUTFILENAME	(PATHPrefix + "NetOut.G24")
#define DEC_OUTPUTPATH	(PATHPrefix + "DecOut\\")
#define FFMPEGPATH		string("D:\\Program-Folders\\ffmpeg\\bin\\ffmpeg.exe")

// 视频参数常量设置区
// Area Of Video Consts
#define G_preframe 25
#define G_tailframe 5
#define G_randomseed 24 // 二十四组专用随机数！乌拉！

// 单帧参数常量设定区
// Area Of Single Frame Consts
/**每个Cell格子的边长*/
#define G_cellsize 10

/**每个Block方块的边框宽度*/
#define G_border 5

/**每个Block在X方向的列数*/
#define G_xsplit 30
/**每个Block在Y方向的行数*/
#define G_ysplit 34

/**第一个Block方块左上角的X坐标*/
#define G_xlocation 15
/**第一个Block方块左上角的Y坐标*/
#define G_ylocation 9

/**各个Block方块左上角坐标的X差值*/
#define G_xspan (2 * G_border + G_xsplit * G_cellsize + 6)
/**各个Block方块左上角坐标的Y差值*/
#define G_yspan (2 * G_border + G_ysplit * G_cellsize + 6)

/**4字节容量 表示文件长度（按字节计）的偏移量*/
#define G_offset 16
/**checksum size 校验和长度	4字节容量 共计16*2bit格子*/
#define G_csSize 16
/**frame id size 帧编号 3个一循环 2字节容量 共计8*2bit格子*/
#define G_frmIDSize 8
//帧编号具体细节：
//public static int[] frmidlist = { 0x0, 0xAAAA, 0xFFFF };

/**block capacity 方块容量 每帧能够容纳多少个2bit格子*/
#define G_blockCap (G_xsplit * G_ysplit)
/**frame capacity 帧容量 每帧能够容纳多少个2bit格子*/
#define G_frameCap (18 * G_blockCap)

namespace PATH {
}
