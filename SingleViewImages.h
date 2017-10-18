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

	// TODO: �ڴ�������ķ�����
	void SetParent(CWellImages *pParent);
	void SetViewID(unsigned int viewID);
	
	void ClearAll();
	unsigned int AddImageFile(CString imgFileName,ImgChannelType ctype);
	unsigned int GetImageListNumber();

	unsigned int LoadImages();
	unsigned int GetLoadedImageNumber();

	
	int ChipRecgn();//оƬʶ�𣬰���������ֵͼ���ָ�оƬ��оƬʶ��������
	void DataAnalysis();
	void ReleaseImages();

	CWellImages * m_pParent;

	unsigned int m_ViewID;

private:
	int SegChips(Mat thresImg);//��ChipRecgn()���ã�����ֵͼ�зָ�õ�����оƬ������ȷ�Ϻ�ѹ��m_SegedChips
	int VerifyChip(Mat rChip);//��SegChips()���ã�ȷ�Ϸָ����ǲ���оƬ

private:
	vector<Mat> m_ScannedImgs;
	vector<ImgChannelType> m_ImgTypes;
	vector<CString> m_ImgNames;
	vector<CSegedChip> m_SegedChips;

	
	
};