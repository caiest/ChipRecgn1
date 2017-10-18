#include "stdafx.h"
#include "publicFuncs.h"

int SeedFill(unsigned char *indata,int cx, int cy, int startX, int startY, unsigned char fillData, unsigned char *fillImg,int &x1,int &x2,int &y1,int &y2)
{
	int fillCount = 0;

	//���ұ߽�����λ��
	int xl, xr;

	x1=startX;x2=startX;y1=startY;y2=startY;

	//�Ƿ���������߽�
	bool bFilll,bFillr;

	//���Ӷ�ջ��ָ��
	vector<int>  Seeds;

	//��ǰ����λ��
	int iCurrentPixelx,iCurrentPixely;
	int iBufferPixelx,iBufferPixely;

	//��ʼ������
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
		//ȡ������
		int stackNum = Seeds.size();
		iCurrentPixelx = Seeds[stackNum-2];
		iCurrentPixely = Seeds[stackNum-1];
		Seeds.pop_back();
		Seeds.pop_back();
		bFilll = true;
		bFillr = true;
		//����������ڵ���
		//�����������ص�λ��
		iBufferPixelx = iCurrentPixelx;
		iBufferPixely = iCurrentPixely;
		//���������
		while(bFilll)
		{
			pSrc = indata + cx * iCurrentPixely + iCurrentPixelx;
			pixel = *pSrc;

			////Ŀ��ͼ���к���0��255��������Ҷ�ֵ
			//if(pixel != 255 && pixel != 0)
			//	return false;
			//�����߽�
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
				//��ֹԽ��
				if(iCurrentPixelx < 0)
				{	
					bFilll = false;
					iCurrentPixelx = 0;
					xl = 0; 
				}
			}		
		}

		//���������
		//ȡ���������ص�λ��
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

			////Ŀ��ͼ���к���0��255��������Ҷ�ֵ
			//if(pixel != 255 && pixel != 0)
			//	return FALSE;
			//�����߽�
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
				//��ֹԽ��
				if(iCurrentPixelx >= cx)
				{	
					bFillr = false;
					iCurrentPixelx = cx-1;
					xr = cx-1; 
				}
			}		
		}
		//�ϡ�������ɨ�����Ƿ�ȫΪ�߽����ػ�������
		//�ȿ������ɨ����
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

			//��δ�������أ����µ�����ѹ���ջ
			if(pixel&&*pSrc1==0)
			{
				if(bSkip)// ���������ز������ջ cm
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
		//�ٿ������ɨ����
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

			//��δ�������أ����µ�����ѹ���ջ
			if(pixel&&*pSrc1==0)
			{
				if(bSkip)// ���������ز������ջ cm
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

	// �������ظ���
	return fillCount;
}

