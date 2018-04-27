#include <tchar.h>
#include <stdio.h>      /* printf */
#include <math.h>
#include "Image.h"

template <typename Type>
Type round(Type val)
{
	return (val> 0.0) ? floor(val+ 0.5) : ceil(val- 0.5);
}

typedef struct _BmpInfo
{
	int nWidth;
	int nHeight;
	int nBitCount;
	int nPitch;
	byte *pBmpColorData;
}BmpInfo,*PBmpInfo;


void ApplyMapping(BmpInfo* pBmp,byte** Map)
{
	byte* p = pBmp->pBmpColorData;
	int offset = pBmp->nPitch - pBmp->nWidth*pBmp->nBitCount/8;
	for (int i=0; i< pBmp->nHeight;i++)
	{
		for (int j =0;j< pBmp->nWidth;j++)
		{
			p[0] = (*Map)[p[0]];
			p[1] = (*Map)[p[1]];
			p[2] = (*Map)[p[2]];
			p +=3;
		}
		p+=offset;
	}
	return;
}


/// <summary>
/// ɫ�׵���
/// </summary>
/// <param name="b">λͼ����24λ��ɫ</param>
/// <param name="iB">����ɫ�ף��ڳ�</param>
/// <param name="iW">����ɫ�ף��׳�</param>
/// <param name="oB">���ɫ�ף��ڳ�</param>
/// <param name="oW">���ɫ�ף��׳�</param>
/// <returns>λͼ����24λ��ɫ</returns>
void Gradation(BmpInfo* pBmp,byte iB,byte iW,byte oB,byte oW)
{
	byte* Map = new byte[256]; //����ӳ���
	for (int i = 0; i <= iB; i++) //��С������ɫ�׺�ɫ����ֵ������ӳ��Ϊ���ɫ�׺�ɫ����ֵ
		Map[i] = oB;
	for (int i = iW; i <= 255; i++) //����������ɫ�װ�ɫ����ֵ������ӳ��Ϊ���ɫ�װ�ɫ����ֵ
		Map[i] = oW;
	double detX = iW - iB;//x�仯����������ɫ�׷�Χ
	double detY = oW - oB;//y�仯���������ɫ�׷�Χ
	double k = detY / detX;//ӳ������б��
	double Sum = oB;//ɫ��ӳ���ۼ������൱�������y
	for (int i = iB + 1; i < iW; i++)
	{
		Sum += k;
		Map[i] = (byte)round(Sum);
	}
	ApplyMapping(pBmp,&Map);
	delete [] Map;
	Map = NULL;
}

int main(int agrc,char** agrv)
{
	CImage * pImge = new CImage;
	pImge->Load(_T("bmp.bmp"));
	BmpInfo bmpInfo;
	bmpInfo.nWidth = pImge->GetWidth();
	bmpInfo.nHeight = pImge->GetHeight();
	bmpInfo.nBitCount = 24;
	bmpInfo.pBmpColorData = pImge->GetBits();
	bmpInfo.nPitch = pImge->GetPitch();
	Gradation(&bmpInfo,0,180,0,255);

	pImge->Save(_T("bmp_deal.bmp"));


	return 0;
}