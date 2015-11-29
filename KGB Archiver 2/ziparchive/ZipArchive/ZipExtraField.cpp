////////////////////////////////////////////////////////////////////////////////
// This source file is part of the ZipArchive library source distribution and
// is Copyrighted 2000 - 2006 by Tadeusz Dracz (tdracz@artpol-software.com)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// For the licensing details refer to the License.txt file.
//
// Web Site: http://www.artpol-software.com
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZipExtraField.h"

bool CZipExtraField::Read(CZipStorage *pStorage, WORD uSize)
{
	if (uSize == 0)
		return true;
	CZipAutoBuffer buffer;
	buffer.Allocate(uSize);
	pStorage->Read(buffer, uSize, true);
	int offset = 0;
	do
	{
		CZipExtraData* pExtra = new CZipExtraData();
		if (!pExtra->Read(buffer + offset, uSize))
		{
			delete pExtra;
			return false;
		}
		int totalSize = pExtra->GetTotalSize();
		if (totalSize > uSize || totalSize < 0)
			return false;
		offset += totalSize;		
		uSize = (WORD)(uSize - totalSize);
		Add(pExtra);
	}
	while (uSize > 0);
	return true;
}


void CZipExtraField::Write(char* buffer)const
{
	int offset = 0;
	for (int i = 0; i < GetCount(); i++)
		offset += GetAt(i)->Write(buffer + offset);
}

int CZipExtraField::GetTotalSize()const
{
	int total = 0;
	for (int i = 0; i < GetCount(); i++)
		total += GetAt(i)->GetTotalSize();
	return total;
}

void CZipExtraField::RemoveInternalHeaders()
{
	for (int i = GetCount() - 1; i >= 0; i--)
	{
		WORD headerID = GetAt(i)->GetHeaderID();
		if (
			headerID == ZIP_EXTRA_ZARCH)
				RemoveAt(i);
	}
}

CZipExtraData* CZipExtraField::Lookup(WORD headerID, int& index) const
{
	for (int i = 0; i < GetCount(); i++)
	{
		CZipExtraData* pExtra = GetAt(i);
		if (pExtra->m_uHeaderID == headerID)
		{
			index = i;
			return pExtra;
		}
	}
	return NULL;
}

