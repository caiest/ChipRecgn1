#pragma once

#include "SingleViewImages.h"


class CHIPRECGN_API CWellImages 
{
public:
	CWellImages(CScanProc *pParent = NULL, unsigned int viewPerWell = 20,  
		unsigned int wellxid=0, unsigned int wellyid = 0);
	CWellImages(const CWellImages &well);
	CWellImages & operator = (const CWellImages &well );
	~CWellImages();

	// TODO: 在此添加您的方法。
	void setParent(CScanProc *pParent);
	void setViewCnt(unsigned int viewPerWell);
	void setWellID(unsigned int wellxid, unsigned int wellyid);
	void ClearContents();

	//readID表示同一个位置多次拍摄操作，这种情况在本框架下，每次拍摄都生成独立的CSingleViewImages对象，
	//所以readid放在well里面，到singleimage里面不用管readid
	//此版本不涉及readid的使用，因此m_ViewImages用一维动态数组即可，
	//如果考虑readid，就改成二维动态数组，但问题在于如何分析计算数值，预测还是每次扫描，每孔统计。
	BOOL AddImageFile(CString fileNames, unsigned int viewID, ImgChannelType ctype, unsigned int readID);
	int LoadImages();
	int ChipRecgn();
	int DataAnalysis();
	void Analysis();
	void ReleaseImages();

	unsigned int m_ViewPerWell;
	unsigned int m_WellXIdx;
	unsigned int m_WellYIdx;

	CScanProc * m_pParent;

	Mat *tmpSum;

private:
	CSingleViewImages *m_ViewImages;
	vector<CString> m_ViewImgNames;
	
};

