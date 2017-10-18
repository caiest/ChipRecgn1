#include "stdafx.h"
#include "publicFuncs.h"

int SeedFill(unsigned char *indata,int cx, int cy, int startX, int startY, unsigned char fillData, unsigned char *fillImg,int &x1,int &x2,int &y1,int &y2)
{
	int fillCount = 0;

	//左右边界像素位置
	int xl, xr;

	x1=startX;x2=startX;y1=startY;y2=startY;

	//是否已填充至边界
	bool bFilll,bFillr;

	//种子堆栈及指针
	vector<int>  Seeds;

	//当前像素位置
	int iCurrentPixelx,iCurrentPixely;
	int iBufferPixelx,iBufferPixely;

	//初始化种子
	Seeds.push_back(startX);
	Seeds.push_back(startY);

	unsigned char pixel;
	unsigned char pixels;
	unsigned char * pSrc;
	unsigned char * pSrc1;
	pSrc = indata + cx * startY + startX;
	pixels = *pSrc;

	while( !Seeds.empty())
	{
		//取出种子
		int stackNum = Seeds.size();
		iCurrentPixelx = Seeds[stackNum-2];
		iCurrentPixely = Seeds[stackNum-1];
		Seeds.pop_back();
		Seeds.pop_back();
		bFilll = true;
		bFillr = true;
		//填充种子所在的行
		//保存种子像素的位置
		iBufferPixelx = iCurrentPixelx;
		iBufferPixely = iCurrentPixely;
		//先向左填充
		while(bFilll)
		{
			pSrc = indata + cx * iCurrentPixely + iCurrentPixelx;
			pixel = *pSrc;

			////目标图像中含有0和255外的其它灰度值
			//if(pixel != 255 && pixel != 0)
			//	return false;
			//遇到边界
			if(pixel==0)
			{
				bFilll = false;
				xl = iCurrentPixelx+1;
				if(xl<x1)
					x1=xl;
			}
			else
			{
				pSrc1 = fillImg + cx * iCurrentPixely + iCurrentPixelx;
				*pSrc1 = fillData;
				fillCount ++;
				iCurrentPixelx--;
				//防止越界
				if(iCurrentPixelx < 0)
				{	
					bFilll = false;
					iCurrentPixelx = 0;
					xl = 0; 
				}
			}		
		}

		//再向右填充
		//取回种子像素的位置
		iCurrentPixelx = iBufferPixelx+1;
		if(iCurrentPixelx >= cx)
		{	
			bFillr = false;
			iCurrentPixelx = cx-1;
			xr = cx-1; 
		}
		while(bFillr)
		{
			pSrc = indata + cx * iCurrentPixely + iCurrentPixelx;
			pixel = *pSrc;

			////目标图像中含有0和255外的其它灰度值
			//if(pixel != 255 && pixel != 0)
			//	return FALSE;
			//遇到边界
			if(pixel==0)
			{
				bFillr = false;
				xr = iCurrentPixelx-1;
				if(xr>x2)
					x2=xr;
			}
			else
			{
				pSrc1 = fillImg + cx * iCurrentPixely + iCurrentPixelx;
				*pSrc1 = fillData;
				fillCount ++;
				iCurrentPixelx++;
				//防止越界
				if(iCurrentPixelx >= cx)
				{	
					bFillr = false;
					iCurrentPixelx = cx-1;
					xr = cx-1; 
				}
			}		
		}
		//上、下两条扫描线是否全为边界象素或已填充过
		//先看上面的扫描线
		iCurrentPixely--;
		if(iCurrentPixely < 0)
		{
			iCurrentPixely = 0;
		}
		bool bSkip = true;
		int i = 0;
		for (i = xr; i>= xl;i--)
		{
			pSrc = indata + cx * iCurrentPixely + i;
			pixel = *pSrc;
			pSrc1 = fillImg + cx * iCurrentPixely + i;

			//有未填充的像素，将新的种子压入堆栈
			if(pixel&&*pSrc1==0)
			{
				if(bSkip)// 连续的象素不进入堆栈 cm
				{
					Seeds.push_back(i);
					Seeds.push_back(iCurrentPixely);
					bSkip = false;
					if(iCurrentPixely<y1)
						y1=iCurrentPixely;
				}
			}		
			else
				bSkip = true;
		}
		//再看下面的扫描线
		iCurrentPixely += 2;
		if(iCurrentPixely >= cy)
		{
			iCurrentPixely = cy-1;
		}
		bSkip = true;
		for (i = xr; i>= xl;i--)
		{
			pSrc = indata + cx * iCurrentPixely + i;
			pixel = *pSrc;
			pSrc1 = fillImg + cx * iCurrentPixely + i;

			//有未填充的像素，将新的种子压入堆栈
			if(pixel&&*pSrc1==0)
			{
				if(bSkip)// 连续的象素不进入堆栈 cm
				{
					Seeds.push_back(i);
					Seeds.push_back(iCurrentPixely);
					bSkip = false;
					if(iCurrentPixely>y2)
						y2=iCurrentPixely;
				}
			}		
			else
				bSkip = true;
		}
	}

	// 返回象素个数
	return fillCount;
}

