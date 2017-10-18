#pragma once

#include "SegedChip.h"

using namespace std;

enum ImgChannelType
{
	BrightField=0,
	Phycoerthyrin,
	GFP,
	RFP,
	CY3,
	CY5,
	TexasRed,
	unknown,
};

class CHIPRECGN_API CSingleViewImages 
{
public:
	CSingleViewImages(CWellImages *pParent = NULL,unsigned int viewID =0);
	CSingleViewImages(const CSingleViewImages &singleview);
	CSingleViewImages & operator = (const CSingleViewImages &singleview);
	~CSingleViewImages();

	// TODO: 在此添加您的方法。
	void SetParent(CWellImages *pParent);
	void SetViewID(unsigned int viewID);
	
	void ClearAll();
	unsigned int AddImageFile(CString imgFileName,ImgChannelType ctype);
	unsigned int GetImageListNumber();

	unsigned int LoadImages();
	unsigned int GetLoadedImageNumber();

	
	int ChipRecgn();//芯片识别，包含制作阈值图，分割芯片和芯片识别三部分
	void DataAnalysis();
	void ReleaseImages();

	CWellImages * m_pParent;

	unsigned int m_ViewID;

private:
	int SegChips(Mat thresImg);//由ChipRecgn()调用，从阈值图中分割得到单个芯片，经过确认后压入m_SegedChips
	int VerifyChip(Mat rChip);//由SegChips()调用，确认分割结果是不是芯片

private:
	vector<Mat> m_ScannedImgs;
	vector<ImgChannelType> m_ImgTypes;
	vector<CString> m_ImgNames;
	vector<CSegedChip> m_SegedChips;

	
	
};