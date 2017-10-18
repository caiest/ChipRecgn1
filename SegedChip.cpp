#include "stdafx.h"
#include "SegedChip.h"
#include "SingleViewImages.h"
#include "WellImages.h"


CSegedChip::CSegedChip(CSingleViewImages * pParent, unsigned int chipIdx)
{
	m_pParent = pParent;
	m_ChipIdx = chipIdx;
	return;
}


unsigned int CSegedChip::AddChipSegment(Mat chip)
{
	m_ChipSegments.push_back(chip.clone());

	/*CString tmpfname;
	tmpfname.Format("xx%d.tif",m_ChipSegments.size());
	imwrite(tmpfname.GetBuffer(),chip);*/

	return m_ChipSegments.size();
}

void CSegedChip::AddChipMask(Mat mask)
{
	m_ChipMask = mask.clone();
	return;
}

CSegedChip::CSegedChip(const  CSegedChip &chip)
{
	m_ChipIdx=chip.m_ChipIdx;
	m_Code=chip.m_Code;
	m_pParent=chip.m_pParent;
	m_ChipMask = chip.m_ChipMask.clone();
	m_ChipIdx=chip.m_ChipIdx;

	if(chip.m_ChipSegments.size()>0)
	{
		for(vector<Mat>::const_iterator iter=chip.m_ChipSegments.begin(); iter!=chip.m_ChipSegments.end(); ++iter)
		{
			m_ChipSegments.push_back((*iter).clone());
		}
	}
	else
	{
		m_ChipSegments.clear();
		m_ChipSegments.shrink_to_fit();
	}

}

CSegedChip & CSegedChip::operator = (const CSegedChip &chip)
{
	m_ChipIdx=chip.m_ChipIdx;
	m_Code=chip.m_Code;
	m_pParent=chip.m_pParent;
	m_ChipMask = chip.m_ChipMask.clone();
	m_ChipIdx=chip.m_ChipIdx;

	if(chip.m_ChipSegments.size()>0)
	{
		for(vector<Mat>::const_iterator iter=chip.m_ChipSegments.begin(); iter!=chip.m_ChipSegments.end(); ++iter)
		{
			m_ChipSegments.push_back((*iter).clone());
		}
	}
	else
	{
		m_ChipSegments.clear();
		m_ChipSegments.shrink_to_fit();
	}

	return *this;
}

int CSegedChip::ChipRecgn()
{
	ChipNormalize();

	//开始识别：
	Mat chipBF = m_ChipSegments.at(0);
	Mat bnyChipBF;
	threshold(chipBF,bnyChipBF,170,255,THRESH_BINARY);

	CString tmpfname;
	tmpfname.Format("%d_%d_%d_%d_bny.tif",m_pParent->m_pParent->m_WellXIdx,
		m_pParent->m_pParent->m_WellYIdx, m_pParent->m_ViewID,m_ChipIdx);
	imwrite(tmpfname.GetBuffer(),m_ChipMask);

	unsigned i,ptSum,markPos,maxPts;
	Rect markRect[4];
	markRect[0]= Rect(0,0,12,12);
	markRect[1]= Rect(27,0,12,12);
	markRect[2]= Rect(0,12,12,12);
	markRect[3]= Rect(27,12,12,12);
	Rect bitRect[8];
	bitRect[0]= Rect(4,4,7,7);
	bitRect[1]= Rect(12,4,7,7);
	bitRect[2]= Rect(20,4,7,7);
	bitRect[3]= Rect(28,4,7,7);
	bitRect[4]= Rect(4,13,7,7);
	bitRect[5]= Rect(12,13,7,7);
	bitRect[6]= Rect(20,13,7,7);
	bitRect[7]= Rect(28,13,7,7);
	unsigned int bits[8]={0};

	maxPts=0;markPos=0;
	for(i=0;i<4;i++)
	{
		ptSum=cv::countNonZero(bnyChipBF(markRect[i]));
		if(ptSum>maxPts)
		{
			maxPts=ptSum;
			markPos=i;
		}
	}
	if(maxPts<20)//没找到
		return -1;

	for(i=0;i<8;i++)
	{
		ptSum=cv::countNonZero(bnyChipBF(bitRect[i]));
		if(ptSum>=4)
		{
			bits[i]=1;
		}
		else
		{
			bits[i]=0;
		}
	}

	switch(markPos)
	{
	case 0:
		m_Code = bits[4]*64+bits[1]*32+bits[5]*16+bits[2]*8+bits[6]*4+bits[3]*2+bits[7];
		break;
	case 1:
		m_Code = bits[7]*64+bits[2]*32+bits[6]*16+bits[1]*8+bits[5]*4+bits[0]*2+bits[1];
		break;
	case 2:
		m_Code = bits[0]*64+bits[5]*32+bits[1]*16+bits[6]*8+bits[2]*4+bits[7]*2+bits[3];
		break;
	case 3:
		m_Code = bits[3]*64+bits[6]*32+bits[2]*16+bits[5]*8+bits[1]*4+bits[4]*2+bits[0];
		break;
	default:
		break;
	}
	if(m_Code!=93)
		m_Code =m_Code;

	return m_Code;
}

float CSegedChip::CalRotAngle()
{
	unsigned int width = m_ChipMask.cols;
	unsigned int height = m_ChipMask.rows;

	unsigned int i,j,k;
	unsigned char * pBuf= m_ChipMask.data;
	std::vector<cv::Point> points;  
	if(width>height)//横着的
	{
		for(i=width/2;i<width-4;i++)
		{
			for(j=0;j<height;j++)
			{
				k = j*width+i;
				if(pBuf[k]!=0)
				{
					points.push_back(cv::Point(i, j));  
					if(j<=1)
					{
						i=width;
					}
					break;
				}
			}
		}

		for(i=width/2;i>4;i--)
		{
			for(j=0;j<height;j++)
			{
				k = j*width+i;
				if(pBuf[k]!=0)
				{
					points.push_back(cv::Point(i, j));  
					if(j<=1)
					{
						i=1;
					}
					break;
				}
			}
		}
	}
	else//竖着的
	{
		for(j=height/2;j<height-4;j++)
		{
			for(i=0;i<width;i++)
			{
				k = j*width+i;
				if(pBuf[k]!=0)
				{
					points.push_back(cv::Point(i, j));  
					if(i<=1)
					{
						j=height;
					}
					break;
				}
			}
		}

		for(j=height/2;j>4;j--)
		{
			for(i=0;i<width;i++)
			{
				k = j*width+i;
				if(pBuf[k]!=0)
				{
					points.push_back(cv::Point(i, j));  
					if(i<=1)
					{
						j=1;
					}
					break;
				}
			}
		}
	}

	cv::Vec4f line_para;   
	cv::fitLine(points, line_para, CV_DIST_HUBER, 0, 1e-2, 1e-2);  

	double ang;
	
	if(fabs(line_para[0])>Er)
	{
		ang = atan(line_para[1] / line_para[0]);
	}
	else
	{
		ang = PI/2.0;
	}

	return ang;
}


void CSegedChip::ChipNormalize()
{
	float ang;
	int width = m_ChipMask.cols;
	int height = m_ChipMask.rows;
	int chipWidth,chipHeight;



	unsigned int i,j,k;

	ang=CalRotAngle();

	if(width<33&&height>28&&fabs(ang)<0.01)
		ang+=PI/2.0;

	cv::Point2f center((width-1.0) / 2.0, (height-1.0) / 2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, ang/PI*180.0, 1);
	cv::Size rotSize = m_ChipMask.size();
	rotSize.width+=4;
	rotSize.height+=4;

	cv::Rect bbox = cv::RotatedRect(center, rotSize, ang/PI*180.0).boundingRect();

	rot.at<double>(0, 2) += (bbox.width-1.0) / 2.0 - center.x;
	rot.at<double>(1, 2) += (bbox.height-1.0) / 2.0 - center.y;


	/*CString tmpfname;
	tmpfname.Format("%d_%d_%d_%d_%5.5f.tif",m_pParent->m_pParent->m_WellXIdx,
		m_pParent->m_pParent->m_WellYIdx, m_pParent->m_ViewID,m_ChipIdx, ang/PI*180.0);
	imwrite(tmpfname.GetBuffer(),m_ChipMask);*/

	cv::Mat tmpRotImg;
	cv::warpAffine(m_ChipMask, tmpRotImg, rot, bbox.size(),INTER_AREA);

	width = tmpRotImg.cols;
	height = tmpRotImg.rows;
	center=Point2f((width-1.0) / 2.0, (height-1.0) / 2.0);

	chipWidth=39;
	chipHeight=24;
	cv::Point2f topLeft(center.x+1.0-chipWidth/2.0, center.y+1.0-chipHeight/2.0);
	if(topLeft.x<0)
		topLeft.x=0;
	if(topLeft.y<0)
		topLeft.y=0;
	Rect chipRect(topLeft.x,topLeft.y, chipWidth,chipHeight);
	m_ChipMask=tmpRotImg(chipRect).clone();
	

	for(vector<Mat>::iterator iter=m_ChipSegments.begin(); iter!=m_ChipSegments.end(); ++iter)
	{
		cv::warpAffine(*iter, tmpRotImg, rot, bbox.size(),INTER_AREA);
		(*iter)=tmpRotImg(chipRect).clone();

		/*(*iter).convertTo(tmpRotImg,CV_32FC1);
		if(iter ==m_ChipSegments.begin() )
		{
			*m_pParent->m_pParent->tmpSum = *m_pParent->m_pParent->tmpSum  +tmpRotImg ;
		}*/
	}

	/*CString tmpfname;
	tmpfname.Format("%d_%d_%d_%d_%5.5f.tif",m_pParent->m_pParent->m_WellXIdx,
		m_pParent->m_pParent->m_WellYIdx, m_pParent->m_ViewID,m_ChipIdx, ang/PI*180.0);
	imwrite(tmpfname.GetBuffer(),m_ChipMask);*/

	/*tmpfname.Format("%d_%d_%d_%d.tif",m_pParent->m_pParent->m_WellXIdx,
		m_pParent->m_pParent->m_WellYIdx, m_pParent->m_ViewID,m_ChipIdx);
	imwrite(tmpfname.GetBuffer(),m_ChipMask);*/
}