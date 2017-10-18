// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� CHIPRECGN_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// CHIPRECGN_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#pragma once

#include "publicfuncs.h"
#include "ChipRecgn.h"


#include "opencv2/core/core.hpp"  
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/nonfree/features2d.hpp"  
#include "opencv2/nonfree/nonfree.hpp"  

using namespace cv;
using cv::Mat;

class CSingleViewImages;
class CWellImages;
class CScanProc;

// �����Ǵ� ChipRecgn.dll ������
class CHIPRECGN_API CSegedChip 
{
public:
	CSegedChip(CSingleViewImages * pParent = NULL, unsigned int chipIdx=0);
	CSegedChip(const CSegedChip &chip);
	CSegedChip & operator = (const CSegedChip &chip );

	// TODO: �ڴ�������ķ�����
	unsigned int AddChipSegment(Mat chip);
	void AddChipMask(Mat mask);

	int ChipRecgn();

	float CalRotAngle();
	void ChipNormalize();
	void DataAnalysis();

private:
	vector<Mat> m_ChipSegments;
	Mat m_ChipMask;
	unsigned int m_ChipIdx;
	unsigned int m_Code;

	CSingleViewImages *m_pParent;
};

