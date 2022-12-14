#ifndef VECTOR_CONTAINER
#define VECTOR_CONTAINER

#include "Constants.hpp"
#include "IContainer.hpp"
#include <cmath>
#include <iostream>

namespace GLVM::Core
{

	template<class T>
	class Vector : public IContainer
	{
		unsigned int iSize_ = 0;
		unsigned int iCapacity_ = 0;
		unsigned int iExpander_ = 10;
		T* aVector_Container_ = new T[iSize_];
	public:
        Vector() {}
        Vector(const Vector<T>& _vector);
        ~Vector();
		void Push(const T _Item);
		T Pop();
		void Insert(const T _Item, const unsigned int _Index);
		void RemoveItem(const T _Item);
        void RemoveObject(const T _Item);
		void RemoveFirstItem();
		T& GetItem(const T _Item);
		T& GetFirstItem();
		T& GetHead();
		T* GetVectorContainer();
		int GetSize();
		int GetCapacity();
		T& operator[](const unsigned int _iIndex);
        void Print();
        Vector& operator=(const Vector<T>& _vector);
        bool operator==(const char* _string);
        void Clear();
	};

    template <class T>
    void Vector<T>::Clear() {
        delete [] aVector_Container_;
		aVector_Container_ = nullptr;
    }

    template <class T>
    bool Vector<T>::operator==(const char* _string) {
        for (int i = 0; i < iSize_; ++i) {
            if (aVector_Container_[i] == _string[i])
                continue;
            else
                return false;
        }

        return true;
    }
    
    template <class T>
    Vector<T>& Vector<T>::operator=(const Vector<T>& _vector)
    {
        if(this == &_vector)
            return *this;

        T* aTemp_Vector_Container_ = new T[_vector.iSize_];
        this->iSize_ = _vector.iSize_;
        
        for(int i = 0; i < _vector.iSize_; ++i)
            aTemp_Vector_Container_[i] = _vector.aVector_Container_[i];
        delete [] this->aVector_Container_;
        this->aVector_Container_ = nullptr;
        this->aVector_Container_ = aTemp_Vector_Container_;

        return *this;
    }

    template <class T>
    Vector<T>::Vector(const Vector<T>& _vector)
    {
        
        
        T* aVector_Container = new T[_vector.iSize_];
        iSize_ = _vector.iSize_;
        
        for(int i = 0; i < _vector.iSize_; ++i)
            aVector_Container[i] = _vector.aVector_Container_[i];

        delete [] this->aVector_Container_;
        this->aVector_Container_ = aVector_Container;

    }
    
	template<class T>
	Vector<T>::~Vector()
	{
		delete [] aVector_Container_;
		aVector_Container_ = nullptr;
	}

    /// Push element on top of the container.
    
	template<class T>
	void Vector<T>::Push(const T _Item)
	{
		if(iSize_ == iCapacity_)
		{
			T* aTemp_Vector_Container = new T[iCapacity_ + iExpander_];
			for(unsigned int i = 0; i < iCapacity_; ++i)
				aTemp_Vector_Container[i] = aVector_Container_[i];

			delete [] aVector_Container_;
			aVector_Container_ = aTemp_Vector_Container;

			iCapacity_ += iExpander_;
			
			aVector_Container_[iSize_] = _Item;
			++iSize_;
			return;
		}

		aVector_Container_[iSize_] = _Item;
		++iSize_;
		return;
	}

	template <class T>
	T Vector<T>::Pop()
	{
		T returnValue;
		
		if (iSize_ > 0) {
			returnValue = aVector_Container_[iSize_ - 1];
			
			T* aTemp_Vector_Container = new T[iCapacity_ + iExpander_];
			for(unsigned int i = 0; i < iSize_ - 1; ++i)
				aTemp_Vector_Container[i] = aVector_Container_[i];

			delete [] aVector_Container_;
			aVector_Container_ = nullptr;
			aVector_Container_ = aTemp_Vector_Container;
			
			iSize_ = iSize_ - 1;
		} else {
			std::cout << "Vector size is 0" << std::endl;
			return {};
		}

		return returnValue;
	}
	
    /// Insert element into chosen cell.
    
	template<typename T>
	void Vector<T>::Insert(T _Item, const unsigned int _Index)
	{
		if(_Index >= iCapacity_)
		{
			unsigned int u_iTemp_Capacity = iCapacity_;
			iCapacity_ = (_Index + iExpander_);
			T* aTemp_Vector_Container_ = new T[iCapacity_];

			for(unsigned int j = 0; j < u_iTemp_Capacity; ++j)
				aTemp_Vector_Container_[j] = aVector_Container_[j];

			delete [] aVector_Container_;
            aVector_Container_ = nullptr;
			aVector_Container_ = aTemp_Vector_Container_;

            // delete [] aTemp_Vector_Container_;
            // aTemp_Vector_Container_ = nullptr;
		}

		aVector_Container_[_Index] = _Item;
		++iSize_;
	}
	
	template<class T>
	void Vector<T>::RemoveItem(const T _Item)
	{
		if(iSize_ < 1)
			return;

		bool bRemove_Flag = false;
		
		int iTemp_Index = 0;
		T aTemp_Vector_Container[iCapacity_];
		if(iCapacity_ > 0)
		{
			for(unsigned int i = 0; i < iCapacity_; ++i)
				aTemp_Vector_Container[i] = aVector_Container_[i];
		}
		
		for(unsigned int j = 0; j < iCapacity_; ++j)
		{
			if(_Item == aVector_Container_[j])
			{
				bRemove_Flag = true;
				continue;
			}

			aVector_Container_[iTemp_Index] = aTemp_Vector_Container[j];
			++iTemp_Index;
		}

		if(bRemove_Flag)
		{
			--iSize_;
//			aVector_Container_[iSize_] = k_iNull;
		}
	}

	template<class T>
	void Vector<T>::RemoveObject(const T _Item)
	{
		if(iSize_ < 1)
			return;

		bool bRemove_Flag = false;
		
		int iTemp_Index = 0;
		T aTemp_Vector_Container[iCapacity_];
		if(iCapacity_ > 0)
		{
			for(unsigned int i = 0; i < iCapacity_; ++i)
				aTemp_Vector_Container[i] = aVector_Container_[i];
		}
		
		for(unsigned int j = 0; j < iCapacity_; ++j)
		{
			if(_Item == aVector_Container_[j])
			{
                delete aVector_Container_[j];
                aVector_Container_[j] = nullptr;
				bRemove_Flag = true;
				continue;
			}

			aVector_Container_[iTemp_Index] = aTemp_Vector_Container[j];
			++iTemp_Index;
		}

		if(bRemove_Flag)
		{
			--iSize_;
            aVector_Container_[iSize_] = nullptr;
		}
	}
    
	template<class T>
	void Vector<T>::RemoveFirstItem()
	{
		if(iSize_ < 1)
			return;
		
		T aTemp_Vector_Container[iCapacity_];
		if(iCapacity_ > 0)
		{
			for(unsigned int i = 0; i < (iCapacity_-1); ++i)
				aTemp_Vector_Container[i] = aVector_Container_[i+1];
		}
		
		for(unsigned int j = 0; j < (iCapacity_-1); ++j)
			aVector_Container_[j] = aTemp_Vector_Container[j];
		--iSize_;
		aVector_Container_[iSize_] = k_iNull;
	}
	
	template<class T>
	T& Vector<T>::GetItem(const T _Item)
	{
		for(int i = 0; i < iCapacity_; ++i)
			if(_Item == aVector_Container_[i])
				return aVector_Container_[i];
	}

	template<class T>
	T& Vector<T>::GetFirstItem()
	{
		return aVector_Container_[k_iNull];
	}

	template<class T>
	T& Vector<T>::GetHead()
	{
		return aVector_Container_[iSize_-1];
	}

	template<class T>
	T* Vector<T>::GetVectorContainer()
	{
		return aVector_Container_;
	}

	template<typename T>
	int Vector<T>::GetSize() { return iSize_; }
	template<typename T>
	int Vector<T>::GetCapacity() { return iCapacity_; }
	template<typename T>
	T& Vector<T>::operator[](const unsigned int _iIndex)
	{
		return aVector_Container_[_iIndex];
	}

    template<class T>
    void Vector<T>::Print()
    {
		std::cout << "Start of container" << std::endl;
        for(int i = 0; i < iSize_; ++i)
            std::cout << aVector_Container_[i] << std::endl;

        std::cout << "End of container" << std::endl;
    }
}
    
#endif 
