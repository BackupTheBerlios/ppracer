
#ifndef __PPOGL_FTVECTOR_H_
#define __PPOGL_FTVECTOR_H_

#include "FTGL.h"

namespace ppogl{

/**
 * Provides a non-STL alternative to the STL vector
 */
template <typename FT_VECTOR_ITEM_TYPE>
class FTVector
{
public:
	typedef FT_VECTOR_ITEM_TYPE value_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;
	typedef size_t size_type;
        
	FTVector()
	 : Capacity(0),
	   Size(0),
	  Items(0)
	{
	}
        
	~FTVector()
	{
		clear();
	}
        
	FTVector& operator =(const FTVector& v)
	{
		reserve(v.capacity());
            
		iterator ptr = begin();
		const_iterator vbegin = v.begin();
		const_iterator vend = v.end();
            
		while( vbegin != vend){
			*ptr++ = *vbegin++;
		}
            
		Size = v.size();
		return *this;
	}
        
	size_type size() const
	{
		return Size;
	}
              
	iterator begin()
	{
		return Items;
	}
        
	const_iterator begin() const
	{
		return Items;
	}
        
	iterator end()
	{
		return begin() + size(); 
	}
        
	const_iterator end() const
	{
		return begin() + size(); 
	}
        
	bool empty() const 
	{ 
		return size() == 0; 
	}

	reference operator [](size_type pos) 
	{ 
		return( *(begin() + pos)); 
	}

	const_reference operator []( size_type pos) const 
	{ 
		return( *(begin() + pos)); 
	}
        
	void clear()
	{
		if(Capacity){
			delete [] Items;
			Capacity = Size = 0;
			Items = 0;
		}
	}

	void reserve(size_type n)
	{
		if(Capacity< n){
			expand(n);
		}
	}

	void push_back(const value_type& x)
	{
		if(size() == Capacity){
			expand();
		}
           
		(*this)[size()] = x;
            ++Size;
	}

	void resize(size_type n, value_type x)
	{
		if(n == size()){
			return;
		}
            
		reserve(n);
		iterator l_begin, l_end;
            
		if(n >= Size){
			l_begin = this->end();
			l_end = this->begin() + n;
		}else{
			l_begin = this->begin() + n;
			l_end = this->end();
		}
        
		while(l_begin != l_end){
			*l_begin++ = x;
		}
        
		Size = n;
	}
        
private:
	void expand(size_type capacity_hint = 0)
	{
		size_type new_capacity =( Capacity == 0) ? 256 : Capacity*2;
		if(capacity_hint){
 			while(new_capacity < capacity_hint){
				new_capacity *= 2;
			}
		}
            
		value_type *new_items = new value_type[new_capacity];
            
		iterator l_begin = this->begin();
		iterator l_end = this->end();
		value_type *ptr = new_items;
            
		while(l_begin != l_end){
			*ptr++ = *l_begin++;
		}
            
		if(Capacity){
			delete [] Items;
		}
        
		Items = new_items;
		Capacity = new_capacity;
	}

	size_type Capacity;
	size_type Size;
	value_type* Items;
};

} // namespace ppogl

#endif // __PPOGL_FTVECTOR_H_
