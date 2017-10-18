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

	// TODO: �ڴ�������ķ�����
	void setParent(CScanProc *pParent);
	void setViewCnt(unsigned int viewPerWell);
	void setWellID(unsigned int wellxid, unsigned int wellyid);
	void ClearContents();

	//readID��ʾͬһ��λ�ö�������������������ڱ�����£�ÿ�����㶼���ɶ�����CSingleViewImages����
	//����readid����well���棬��singleimage���治�ù�readid
	//�˰汾���漰readid��ʹ�ã����m_ViewImages��һά��̬���鼴�ɣ�
	//�������readid���͸ĳɶ�ά��̬���飬������������η���������ֵ��Ԥ�⻹��ÿ��ɨ�裬ÿ��ͳ�ơ�
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

