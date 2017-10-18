#pragma once

#include "WellImages.h"

class CHIPRECGN_API CScanProc
{
public:
	CScanProc(unsigned int viewPerWell=20,
		unsigned int maxWellIDX = 8, 
		unsigned int maxWellIDY = 12);
	~CScanProc();

	unsigned int SetScanConfig(unsigned int maxWellIDX, unsigned int maxWellIDY, unsigned int viewPerWell);
	unsigned int LoadWellImageNames(CString pathName);
	int LoadScanImages(unsigned int wellxid, unsigned int wellyid);
	int ChipRecgn(unsigned int wellxid, unsigned int wellyid);
	int DataAnalysis(unsigned int wellxid, unsigned int wellyid);
	BOOL ParseImageName(CString pathFileName, unsigned char &XID,  unsigned char &YID, 
		unsigned int &viewID,ImgChannelType &channel, unsigned int &readID);

private:
	CWellImages ** m_WellImgs;
	vector<CString> m_ImgFileNames;
	unsigned int m_MaxWellIDX;
	unsigned int m_MaxWellIDY;
	unsigned int m_MaxWellCnt;
	unsigned int m_ViewPerWell;
};