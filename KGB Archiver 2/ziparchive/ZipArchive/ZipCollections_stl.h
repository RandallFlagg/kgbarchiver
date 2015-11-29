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

#ifndef ZIPARCHIVE_ZIPCOLLECTIONS_DOT_H
	#error Do not include this file directly. Include ZipCollections.h instead
#endif

#if _MSC_VER > 1000
	#pragma warning( push, 3 ) // STL "requirements"
	#pragma warning (disable : 4284) //return type for 'identifier::operator >' is not a UDT or reference to a UDT. Will produce errors if applied using infix notation
	#pragma warning (disable : 4018) //'expression' : signed/unsigned mismatch
#endif

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include "ZipString.h"

#define ZIP_ARRAY_SIZE_TYPE size_t

template<class TYPE>
class CZipArray : private std::vector<TYPE>
{
public:
    typedef typename std::vector<TYPE>::iterator iterator;
	typedef typename std::vector<TYPE> inherited;
protected:
	iterator GetIterFromIndex(size_t uIndex)
	{
		iterator iter = this->begin();
		iter += uIndex;
		//	int t = 0; while (t != uIndex) {iter++;t++;}
		return iter;
	}
public:
	void Sort(bool bAscending)
	{
		if (bAscending)
			std::sort (this->begin (), this->end (), std::less<TYPE> ());
		else
			std::sort (this->begin (), this->end (), std::greater<TYPE> ());
	}
	size_t GetSize() const{return this->size();	}
	size_t GetCount() const{return this->size();	}
	size_t GetUpperBound() const {return this->size() - 1;}
	TYPE& GetAt(size_t uIndex) {return this->at(uIndex);}
	const TYPE& GetAt(size_t uIndex) const {return this->at(uIndex);} 
	size_t Add(const TYPE& x) {push_back(x);return GetUpperBound();}
	void RemoveAll() {this->clear();}
	void RemoveAt(size_t uIndex) { erase(GetIterFromIndex(uIndex));}
	void InsertAt(size_t uIndex, const TYPE& x){insert(GetIterFromIndex(uIndex), x);}
#ifndef _MSC_VER
	TYPE& operator[](size_t uIndex)
	{
		return inherited::operator[](uIndex);
	}
	TYPE operator[](size_t uIndex) const
	{
		return inherited::operator[](uIndex);
	}
#else
	using inherited::operator[];
#endif 
};


typedef CZipArray<CZipString> CZipStringArray;
typedef CZipArray<WORD> CZipWordArray;


template<class TYPE>
class CZipPtrList : private std::list<TYPE>
{

public:
	typedef typename std::list<TYPE>::iterator iterator;
	typedef typename std::list<TYPE>::const_iterator const_iterator;
	size_t GetCount() const {return this->size();}
	void AddTail(const TYPE& x){push_back(x);}
	void AddHead(const TYPE& x){push_front(x);}
	void RemoveHead() {this->pop_front();}
	void RemoveTail() {this->pop_back();}
	void RemoveAll() {this->clear();}
	TYPE& GetHead() {return this->front();}
	TYPE GetHead() const {return this->front();}
	TYPE& GetTail() {return this->back();}
	TYPE GetTail() const {return this->back();}
	iterator GetHeadPosition() { return this->begin();}
	const_iterator GetHeadPosition() const { return this->begin();}
	iterator GetTailPosition() { return this->back();}
	TYPE& GetNext(iterator& pos) { return *pos++;}
	const TYPE GetNext(const_iterator& pos) const{ return *pos++;}
	TYPE& GetPrev(iterator& pos) { return *pos--;}
	TYPE GetPrev(iterator& pos) const{ return *pos--;}
	iterator Find(TYPE& x) { return std::find(this->begin(), this->end(), x);}
	void RemoveAt(iterator& pos) { erase(pos);}
	bool IteratorValid(const_iterator &iter) const
	{
		return iter != this->end();
	}
	bool IteratorValid(iterator &iter)
	{
		return iter != this->end();
	}
	iterator FindIndex(size_t uIndex)
	{
		iterator iter = this->begin();
		size_t t = 0; while (t != uIndex) {iter++;t++;}
		return iter;
	}
	const_iterator FindIndex(size_t uIndex) const
	{
		const_iterator iter = this->begin();
		size_t t = 0; while (t != uIndex) {iter++;t++;}
		return iter;
	}
	TYPE& GetAt(const iterator& pos) { return *pos;}
	TYPE GetAt(const_iterator& pos) const{ return *pos;}

};

// simplified and partial only
template<class KEY, class VALUE>
class CZipMap : private std::map<KEY, VALUE>
{
public:
	typedef typename std::map<KEY, VALUE>::const_iterator const_iterator;
	typedef typename  std::map<KEY,VALUE, std::less<KEY>, std::allocator<std::pair<const KEY, VALUE> > >::value_type v_type;
	void SetAt( KEY key, VALUE newValue)
	{
		insert(v_type(key, newValue));
	}
	BOOL RemoveKey( KEY key )
	{
		return erase(key) != 0;
	}
	BOOL Lookup( KEY key, VALUE& rValue ) const
	{
		

#if (__GNUC__ >= 3) // I'm not sure which version precisely should be here
		const_iterator iter = std::map<KEY, VALUE>::find(key);
		if (iter == std::map<KEY, VALUE>::end())
#else
		const_iterator iter = find(key);
		if (iter == end())
#endif
			return FALSE;
		else
		{
			rValue = iter->second;
			return TRUE;
		}
	}
};

#if _MSC_VER > 1000
	#pragma warning( pop)
#endif


