#include "stdafx.h"
#include "ScanProc.h"

CScanProc::CScanProc(unsigned int viewPerWell,unsigned int maxWellIDX, unsigned int maxWellIDY)
{
	m_ImgFileNames.clear();
	m_ImgFileNames.shrink_to_fit();
	m_WellImgs=NULL;

	m_ViewPerWell = viewPerWell;
	m_MaxWellIDX = maxWellIDX;
	m_MaxWellIDY = maxWellIDY;
	m_MaxWellCnt = m_MaxWellIDX*m_MaxWellIDY;

	unsigned int i,j;
	m_WellImgs = new CWellImages * [m_MaxWellIDY];
	for(i=0;i<m_MaxWellIDY;i++)
	{
		m_WellImgs[i] = new CWellImages [m_MaxWellIDX];
		for(j=0;j<m_MaxWellIDX;j++)
		{
			m_WellImgs[i][j].setParent(this);
			m_WellImgs[i][j].setViewCnt(20);
			m_WellImgs[i][j].setWellID(j,i);
		}
	}

}

CScanProc::~CScanProc()
{
	m_ImgFileNames.clear();
	m_ImgFileNames.shrink_to_fit();
	
	unsigned int i;
	for(i=0;i<m_MaxWellIDY;i++)
	{
		delete  []m_WellImgs[i];
	}
	delete []m_WellImgs;

	return;
}

void getFolderDayFile(const CString rootPath, CString pathStr, vector<CString>& arrStrFile)
{
	CString myDataPath,fdPath;
	myDataPath=pathStr + "\\*.*";
	CString strTmp,subStr;

	CFileFind find;
	BOOL bf = find.FindFile(myDataPath);
	while(bf)
	{                        
		bf = find.FindNextFile();
		if(!find.IsDots())
		{
			fdPath=find.GetFilePath();
			if (find.IsDirectory())
			{
				getFolderDayFile(rootPath, fdPath, arrStrFile);
			}
			else
			{
				//如果是文件,判断是否是*.txt
				strTmp=fdPath.Right(4);
				strTmp.MakeLower();
				if ( strTmp==".tif" )
				{
					subStr = fdPath.Right(fdPath.GetLength()- rootPath.GetLength());
					arrStrFile.push_back(rootPath+subStr);
				}

			}
		}
	}
	find.Close();
}    


BOOL CScanProc::ParseImageName(CString pathFileName, unsigned char &XID,  unsigned char &YID, unsigned int &viewID,ImgChannelType &ctype, unsigned int &readID)
{
	int fspos = pathFileName.ReverseFind('\\');
	CString fName = pathFileName.Right(pathFileName.GetLength()-fspos-1);
	CString cName;//通道名称
	char c;
	unsigned int pos = 0;
	//第一个字符，X坐标：A to H
	XID = fName.GetAt(pos)-'A';
	if(XID<0||XID>25)
		return FALSE;
	pos++;

	//第二，三个字符，X坐标：0 to 9
	c = fName.GetAt(pos)-'0';
	if(c<0||c>9)
		return FALSE;
	YID = c*10;
	pos++;
	c = fName.GetAt(pos)-'0';
	if(c<0||c>9)
		return FALSE;
	YID += c;
	YID --;
	pos++;

	//跨过一个'_',提取viewID，位数未知
	pos++;
	fName=fName.Mid(pos);
	pos=0;
	viewID=0;
	while(fName.GetAt(pos)!='_')
	{
		c = fName.GetAt(pos)-'0';
		if(c<0||c>9)
			return FALSE;
		viewID = viewID*10+c;
		viewID--;
		pos++;
	}

	//提取后续
	pos++;
	fName=fName.Mid(pos);
	pos=0;
	while(fName.GetAt(pos)!='_')
	{
		pos++;
	}
	cName=fName.Left(pos);
	if(cName=="Bright Field")
	{
		ctype = BrightField;
	}
	else if(cName == "Phycoerythrin" )
	{
		ctype = Phycoerthyrin;
	}
	else if(cName == "GFP" )
	{
		ctype = GFP;
	}
	else if(cName == "RFP" )
	{
		ctype = RFP;
	}
	else if(cName == "CY3" )
	{
		ctype = CY3;
	}
	else if(cName == "CY5" )
	{
		ctype = CY5;
	}
	else if(cName == "TexasRed" )
	{
		ctype = TexasRed;
	}
	else
	{
		ctype = unknown;
	}

	pos++;
	fName=fName.Mid(pos);
	pos=0;
	readID=0;
	while(fName.GetAt(pos)!='.')
	{
		c = fName.GetAt(pos)-'0';
		if(c<0||c>9)
			return FALSE;
		readID = readID*10+c;
		pos++;
	}
	
	return TRUE;
}

unsigned int CScanProc::LoadWellImageNames(CString pathName)
{
	//Mat mat = imread("xxx.tif");
	getFolderDayFile(pathName,pathName,m_ImgFileNames);
	sort(m_ImgFileNames.begin(),m_ImgFileNames.end());

	unsigned int imgCnt = m_ImgFileNames.size();
	unsigned char xid,yid;
	unsigned int viewid,readid;
	ImgChannelType ctype;
	CString curImgName;
	for(unsigned i=0;i<imgCnt;i++)
	{
		curImgName= m_ImgFileNames.at(i);
		ParseImageName(curImgName,xid,yid,viewid,ctype,readid);
		m_WellImgs[yid][xid].AddImageFile(curImgName,viewid,ctype,readid);

	}

	return m_ImgFileNames.size();
}		


int CScanProc::LoadScanImages(unsigned int wellxid, unsigned int wellyid)
{
	if(m_ImgFileNames.size()<1)
		return 0;
	if(m_WellImgs==NULL)
		return -1;

	unsigned int i,j,ret=0;;
	
	ret+=m_WellImgs[wellyid][wellxid].LoadImages();

	return ret;
}

int CScanProc::ChipRecgn(unsigned int wellxid, unsigned int wellyid)
{
	return m_WellImgs[wellyid][wellxid].ChipRecgn();
}