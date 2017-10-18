// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 CHIPRECGN_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// CHIPRECGN_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

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

// 此类是从 ChipRecgn.dll 导出的
class CHIPRECGN_API CSegedChip 
{
public:
	CSegedChip(CSingleViewImages * pParent = NULL, unsigned int chipIdx=0);
	CSegedChip(const CSegedChip &chip);
	CSegedChip & operator = (const CSegedChip &chip );

	// TODO: 在此添加您的方法。
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

