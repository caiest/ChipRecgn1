#include "stdafx.h"
#include "SingleViewImages.h"

CSingleViewImages::CSingleViewImages(CWellImages * pParent,unsigned int viewID)
{
	m_pParent = pParent;
	m_ViewID = viewID;
	ClearAll();
}


CSingleViewImages::CSingleViewImages(const CSingleViewImages &singleview)
{
	m_pParent=singleview.m_pParent;
	m_ViewID=singleview.m_ViewID;
	m_ImgNames=singleview.m_ImgNames;
	m_ImgTypes=singleview.m_ImgTypes;
	m_SegedChips=singleview.m_SegedChips;
	if(singleview.m_ScannedImgs.size()>0)
	{
		for(vector<Mat>::const_iterator iter=singleview.m_ScannedImgs.begin(); iter!=singleview.m_ScannedImgs.end(); ++iter)
		{
			m_ScannedImgs.push_back((*iter).clone());
		}
	}
	else
	{
		m_ScannedImgs.clear();
		m_ScannedImgs.shrink_to_fit();
	}
}


CSingleViewImages & CSingleViewImages:: operator = (const CSingleViewImages &singleview)
{
	m_pParent=singleview.m_pParent;
	m_ViewID=singleview.m_ViewID;
	m_ImgNames=singleview.m_ImgNames;
	m_ImgTypes=singleview.m_ImgTypes;
	m_SegedChips=singleview.m_SegedChips;
	if(singleview.m_ScannedImgs.size()>0)
	{
		for(vector<Mat>::const_iterator iter=singleview.m_ScannedImgs.begin(); iter!=singleview.m_ScannedImgs.end(); ++iter)
		{
			m_ScannedImgs.push_back( (*iter).clone());
		}
	}
	else
	{
		m_ScannedImgs.clear();
		m_ScannedImgs.shrink_to_fit();
	}

	return *this;
}

CSingleViewImages::~CSingleViewImages()
{
	m_ImgTypes.clear();
	m_ImgTypes.shrink_to_fit();
	m_ImgNames.clear();
	m_ImgNames.shrink_to_fit();
	m_ScannedImgs.clear();
	m_ScannedImgs.shrink_to_fit();
}

void CSingleViewImages::SetParent(CWellImages * pParent)
{
	m_pParent = pParent;
}


void CSingleViewImages::SetViewID(unsigned int viewID)
{
	m_ViewID = viewID;
}

void CSingleViewImages::ClearAll()
{
	m_ImgTypes.clear();
	m_ImgTypes.shrink_to_fit();
	m_ImgNames.clear();
	m_ImgNames.shrink_to_fit();
	m_ScannedImgs.clear();
	m_ScannedImgs.shrink_to_fit();
}

unsigned int  CSingleViewImages::AddImageFile(CString imgFileName, ImgChannelType ctype)
{
	if(ctype==BrightField)
	{
		m_ImgNames.insert(m_ImgNames.begin(),imgFileName);
		m_ImgTypes.insert(m_ImgTypes.begin(),ctype);
	}
	else
	{
		m_ImgNames.push_back(imgFileName);
		m_ImgTypes.push_back(ctype);
	}
	return m_ImgNames.size();
}


unsigned int CSingleViewImages::GetImageListNumber()
{
	return m_ImgNames.size();
}

unsigned int CSingleViewImages::LoadImages()
{
	ReleaseImages();
	for(vector<CString>::iterator nameIter= m_ImgNames.begin(); nameIter !=m_ImgNames.end(); ++nameIter)
	{
		Mat tmp = imread((*nameIter).GetBuffer(),CV_LOAD_IMAGE_GRAYSCALE);//这里自动转为8位的图片
		if(!tmp.empty())
		{
			m_ScannedImgs.push_back(tmp.clone());
		}
		tmp.release();
	}
	return m_ScannedImgs.size();
}

unsigned int CSingleViewImages::GetLoadedImageNumber()
{
	return m_ScannedImgs.size();
}

void CSingleViewImages::ReleaseImages()
{
	m_ScannedImgs.clear();
	m_ScannedImgs.shrink_to_fit();
	return;
}



int CSingleViewImages::ChipRecgn()
{
	//现在按照明场+荧光的方式进行识别，所以如果检测到只有队列中只有一张图的话，就不做
	if(m_ScannedImgs.size()<=1)
		return -2;

	//默认的排序方式是第一张图是明场，所以检查一下m_ImgTypes看看是不是
	if(m_ImgTypes.at(0)!=BrightField)
		return -1;

	Mat &OriBF = m_ScannedImgs.at(0);
	Mat &OriPE = m_ScannedImgs.at(1);

	Mat t_ThresImg;

	threshold(OriPE,t_ThresImg, 20, 255, THRESH_BINARY);

	SegChips(t_ThresImg);

	/*for(vector<CSegedChip>::iterator iter=m_SegedChips.begin(); iter!=m_SegedChips.end(); ++iter)
	{
		(*iter).ChipRecgn();
	}*/

	/*CString savedName;
	savedName.Format("%2.2d.tif", m_ViewID);
	imwrite(savedName.GetBuffer(),t_ThresImg);*/

	return 0;
}

int CSingleViewImages::VerifyChip(Mat rChip)
{
	unsigned int i,j,k,cPixCnt;
	float cRatio,rPixCnt;
	unsigned int rows = rChip.rows;
	unsigned int cols = rChip.cols;
	unsigned char *pBufThre=rChip.data;
	rPixCnt= rows*cols;

	//计算前景像素个数
	cPixCnt=countNonZero(rChip);
	cRatio=cPixCnt/rPixCnt;

	Mat erodeStruct = getStructuringElement(MORPH_RECT,Size(3,3));
	Mat t_Chip = rChip.clone();
	erode(rChip,t_Chip,erodeStruct,Point(1,1));
	t_Chip=rChip-t_Chip;

	/*CString tmpfname;
	tmpfname.Format("xx11.tif");
	imwrite(tmpfname.GetBuffer(),t_Chip);
	tmpfname.Format("xx22.tif");
	imwrite(tmpfname.GetBuffer(),rChip-t_Chip);*/

	unsigned int arcLength = countNonZero(t_Chip);

	if(cPixCnt>800&&cPixCnt<950&&cRatio>0.45&&arcLength<160)//一个芯片
	{
		return 1;
	}
	else if(cPixCnt>1600&&cPixCnt<2000&&cRatio>0.23&&arcLength<300/*还有一个周长判断*/)//2个芯片
	{
		return 2;
	}
	else//不是芯片
	{
		return 0;
	}
}

int CSingleViewImages::SegChips(Mat thresImg)
{
	if(thresImg.channels()>1)
		return -1;

	if(thresImg.depth()!=0)
		return -2;

	Mat maskImg=Mat::zeros(thresImg.rows,thresImg.cols,CV_8UC1);

	unsigned char *pBufThre=thresImg.data;
	unsigned char *pBufMask=maskImg.data;

	unsigned int i,j,k,m,n,p;
	unsigned int rows = thresImg.rows;
	unsigned int cols = thresImg.cols;
	int x1,x2,y1,y2;
	int ret = 0;
	int chipIdx = 0;
	for(i=0;i<rows;i++)
	{
		for(j=0;j<cols;j++)
		{
			k=i*cols+j;
			if(pBufThre[k]>0 && pBufMask[k]==0)
			{
				SeedFill(pBufThre,cols,rows,j,i,255,pBufMask,x1,x2,y1,y2);

				if(x1==0)
					x1=1;
				if(y1==0)
					y1=1;
				if(x2>=cols)
					x2-=1;
				if(y2>=rows)
					y2-=1;

				Rect r(x1-1, y1-1, x2-x1+2, y2-y1+2); 


				if(r.area()>700&&r.area()<4000)//要考虑到2个芯片的情况
				{
					Mat t_Chip = (maskImg(r).clone());

					if(VerifyChip(t_Chip.clone())>0)//判断是否是芯片数据
					{
						CSegedChip tmpChip(this,chipIdx);
						//是芯片数据，把所有通道内的数据依次入栈
						for(vector<Mat>::const_iterator iter=m_ScannedImgs.begin(); iter!=m_ScannedImgs.end(); ++iter)
						{
							tmpChip.AddChipSegment((*iter)(r).clone());
						}
						tmpChip.AddChipMask(t_Chip);
						m_SegedChips.push_back(tmpChip);

						chipIdx++;
						//待删除
						tmpChip.ChipRecgn();
					}

					/*CString tmpfname;
					tmpfname.Format("%5.5d_oo.tif",k);
					imwrite(tmpfname.GetBuffer(),t_Chip);*/
				}
				

				for(m=y1;m<=y2;m++)
				{
					for(n=x1;n<=x2;n++)
					{
						p=m*cols+n;
						if(pBufMask[p]==255)
						{
							pBufMask[p]=1;
						}
					}
				}

				ret++;

			}
		}
	}
	
	return ret;
}

