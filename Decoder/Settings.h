#pragma once
#include <string>

// �����������
#define span " "

// ����߳����岿��
#define G_XSPLIT G_xsplit
#define G_YSPLIT G_ysplit
#define G_BORDERWIDE G_border
#define G_CELLSIZE G_cellsize

#define G_BLOCK_X G_xlocation
#define G_BLOCK_Y G_ylocation

// ·���趨��
// Area Of Paths
#define PATHPrefix		string("E:\\NET-Program-Output\\v1.0\\")
#define DEC_INPUTPATH	(PATHPrefix + "DecImg\\")
#define DEC_PROC_PNG	(DEC_INPUTPATH + "Dec%d.jpg")
#define DEC_OUTFILENAME	(PATHPrefix + "NetOut.G24")
#define DEC_OUTPUTPATH	(PATHPrefix + "DecOut\\")
#define FFMPEGPATH		string("D:\\Program-Folders\\ffmpeg\\bin\\ffmpeg.exe")

// ��Ƶ��������������
// Area Of Video Consts
#define G_preframe 25
#define G_tailframe 5
#define G_randomseed 24 // ��ʮ����ר���������������

// ��֡���������趨��
// Area Of Single Frame Consts
/**ÿ��Cell���ӵı߳�*/
#define G_cellsize 10

/**ÿ��Block����ı߿���*/
#define G_border 5

/**ÿ��Block��X���������*/
#define G_xsplit 30
/**ÿ��Block��Y���������*/
#define G_ysplit 34

/**��һ��Block�������Ͻǵ�X����*/
#define G_xlocation 15
/**��һ��Block�������Ͻǵ�Y����*/
#define G_ylocation 9

/**����Block�������Ͻ������X��ֵ*/
#define G_xspan (2 * G_border + G_xsplit * G_cellsize + 6)
/**����Block�������Ͻ������Y��ֵ*/
#define G_yspan (2 * G_border + G_ysplit * G_cellsize + 6)

/**4�ֽ����� ��ʾ�ļ����ȣ����ֽڼƣ���ƫ����*/
#define G_offset 16
/**checksum size У��ͳ���	4�ֽ����� ����16*2bit����*/
#define G_csSize 16
/**frame id size ֡��� 3��һѭ�� 2�ֽ����� ����8*2bit����*/
#define G_frmIDSize 8
//֡��ž���ϸ�ڣ�
//public static int[] frmidlist = { 0x0, 0xAAAA, 0xFFFF };

/**block capacity �������� ÿ֡�ܹ����ɶ��ٸ�2bit����*/
#define G_blockCap (G_xsplit * G_ysplit)
/**frame capacity ֡���� ÿ֡�ܹ����ɶ��ٸ�2bit����*/
#define G_frameCap (18 * G_blockCap)

namespace PATH {
}
