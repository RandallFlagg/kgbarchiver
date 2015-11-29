/*
*
* Project : ZipArc                      
* Copyright (c) 2001 Tadeusz Dracz
*
* You are free to use and distribute as long as this copyright notice remains intact.
* May not be sold for profit.
*
* Based on work by Douglas Peterson. 
* http://www.codeguru.com/cpp_mfc/sortable_cobarray.shtml
*****************************************************/




#include "stdafx.h"
#include "SortableObArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSortableObArray::CSortableObArray()
{
	m_pSortFunction = NULL;
	m_UserData = NULL;
	m_bDestroyOnDelete = true;
}

CSortableObArray::~CSortableObArray()
{
	Clear();
}

int CSortableObArray::InsertSorted(CObject *newElement)
{
	if (!m_pSortFunction)	
	{
		TRACE(_T("Sort function not set\n"));
		return -1;
	}

	ASSERT(newElement);

	int L = 0;
	int P = GetSize();
	int M = 0;

	if (P)
	{
		// sortowanie przez wstawianie po³ówkowe		
		while (P - L > 1)
		{
			M = (P + L) / 2;
			if (m_pSortFunction(newElement, m_pData[M], m_UserData) > 0)
				L = M;
			else
				P = M;
		}
		
		if (m_pSortFunction(newElement, m_pData[P - 1], m_UserData) > 0)
			M = P;
		else 
			M = L;
	}

	InsertAt(M, newElement);
	return M;
}

#define STRIDE_FACTOR 3

void CSortableObArray::Sort()
{
	if (!m_pSortFunction)	
	{
		TRACE(_T("Sort function not set\n"));
		return;
	}

	int iElements = GetSize();
	int iInner,iOuter,iStride = 1;
	CObject *pTmp;
	
	while (iStride <= iElements )
		iStride = iStride * STRIDE_FACTOR  + 1;
	
	while (iStride > (STRIDE_FACTOR  - 1))
	{
		iStride = iStride / STRIDE_FACTOR;
		for (iOuter = iStride; iOuter < iElements; iOuter++)
		{
			iInner = iOuter - iStride;
			while (iInner >= 0)
			{
				if (m_pSortFunction(m_pData[iInner+iStride],m_pData[iInner], m_UserData) < 0)
				{
					pTmp = m_pData[iInner+iStride];
					m_pData[iInner+iStride] = m_pData[iInner];
					m_pData[iInner] = pTmp;
					iInner -= iStride;
				}
				else
					break;
			}
		}
	}

}

void CSortableObArray::RemoveAll()
{
	Clear();
	CObArray::RemoveAll();
}

void CSortableObArray::RemoveAt(int nIndex, int nCount)
{
	if (m_bDestroyOnDelete)
		for (int i = 0; i < nCount; i++)
			delete GetAt(i + nIndex);
	CObArray::RemoveAt(nIndex, nCount);
}

void CSortableObArray::Clear()
{
	if (m_bDestroyOnDelete)
		for (int i = 0; i < GetSize(); i++)
			delete GetAt(i);
}

void CSortableObArray::SetSortFunction(SOA_SORTFUNCTION pFunction, WPARAM lpUserData)
{
	m_pSortFunction = pFunction;
	m_UserData = lpUserData;
}
