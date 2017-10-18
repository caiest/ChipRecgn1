#include "stdafx.h"
#include "WellImages.h"

CWellImages::CWellImages(CScanProc *pParent, unsigned int viewPerWell,  unsigned int wellxid, unsigned int wellyid)
{
	m_pParent=pParent; 
	m_ViewPerWell = viewPerWell;
	m_WellXIdx = wellxid;
	m_WellYIdx = wellyid;

	if(m_ViewPerWell==0)
		m_ViewImages = NULL;
	else
		m_ViewImages = new CSingleViewImages [m_ViewPerWell];
	m_ViewImgNames.clear();

	tmpSum = new Mat(24,39,CV_32FC1, Scalar(0));
}


CWellImages::CWellImages(const CWellImages &well)
{
	m_pParent=well.m_pParent; 
	m_ViewPerWell=well.m_ViewPerWell; 
	m_WellXIdx=well.m_WellXIdx; 
	m_WellYIdx=well.m_WellYIdx; 

	m_ViewImgNames=well.m_ViewImgNames;
	if(m_ViewPerWell==0)
	{
		m_ViewImages = NULL;
	}
	else
	{
		m_ViewImages = new CSingleViewImages [m_ViewPerWell];
		for(unsigned int i = 0;i<m_ViewPerWell;i++)
		{
			m_ViewImages[i]= well.m_ViewImages[i];
		}
	}
	
}

CWellImages & CWellImages::operator = (const CWellImages &well )
{
	m_pParent=well.m_pParent; 
	m_ViewPerWell=well.m_ViewPerWell; 
	m_WellXIdx=well.m_WellXIdx; 
	m_WellYIdx=well.m_WellYIdx; 

	m_ViewImgNames=well.m_ViewImgNames;
	if(m_ViewPerWell==0)
	{
		m_ViewImages = NULL;
	}
	else
	{
		m_ViewImages = new CSingleViewImages [m_ViewPerWell];
		for(unsigned int i = 0;i<m_ViewPerWell;i++)
		{
			m_ViewImages[i]= well.m_ViewImages[i];
		}
	}
	return *this;
}

CWellImages::~CWellImages()
{
	ClearContents();
}

void CWellImages::ClearContents()
{
	delete []m_ViewImages;
	m_ViewImages=NULL;
	m_ViewImgNames.clear();
	m_ViewImgNames.shrink_to_fit();

}

void CWellImages::setParent(CScanProc *pParent)
{
	m_pParent = pParent;
}

void CWellImages::setViewCnt(unsigned int viewPerWell)
{
	m_ViewPerWell = viewPerWell;
	if(m_ViewImages!=NULL)
		delete []m_ViewImages;
	unsigned int i;
	m_ViewImages = new CSingleViewImages [m_ViewPerWell];
	for(i=0;i<m_ViewPerWell;i++)
	{
		m_ViewImages[i].SetParent(this);
		m_ViewImages[i].SetViewID(i);
	}
}

void CWellImages::setWellID(unsigned int wellxid, unsigned int wellyid)
{
	m_WellXIdx = wellxid;
	m_WellYIdx = wellyid;
}

BOOL CWellImages::AddImageFile(CString fileNames, unsigned int viewID, ImgChannelType ctype, unsigned int readID)
{
	if(viewID>=m_ViewPerWell)
		return FALSE;

	if(m_ViewImages==NULL)
		return FALSE;

	m_ViewImages[viewID].AddImageFile(fileNames,ctype);
	m_ViewImgNames.push_back(fileNames);

	return TRUE;
}

int CWellImages::LoadImages()
{
	unsigned int i,j,ret=0;
	if(m_ViewImgNames.size()>0)
	{
		for(i=0;i<m_ViewPerWell;i++)
		{

			ret+=m_ViewImages[i].LoadImages();
		}
	}
	return ret;
}


int CWellImages::ChipRecgn()
{
	if(m_ViewImages==NULL)
	{
		return -1;
	}

	if(m_ViewImgNames.size()<1)
	{
		return 0;
	}

	int i,ret=0;

	for(i=0;i<m_ViewPerWell;i++)
	{
		ret += m_ViewImages[i].ChipRecgn();
	}

	FILE *fp;
	fopen_s(&fp, "histox01","wb+");
	if(fp)
	{
		fwrite(tmpSum->data,sizeof(float)*39*24,1,fp);
		fclose(fp);
	}

	return ret;	
}