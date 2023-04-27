#pragma once

#define BASE std::vector<T>


template<class T>
class Array : public BASE
{
protected:

private:
    /* data */
public:

    Array<T>(/* args */)
    {

    }
    ~Array()
    {

    }

    int GetSize() const
    {
        return (int)BASE::size();
    }

    int Add(const T& t)
    {
        int rc = GetSize();
        BASE::push_back(t);
        return rc;
    }

    void Append(const Array<T>& arr)
    {
        for (int i = 0; i < arr.GetSize(); i++)
        {
            Add(arr[i]);
        }
    }

    void Copy(const Array<T>& arr)
    {
        BASE::clear();
        Append(arr);
    }

    T RemoveAt(int nIndex)
    {
        if (nIndex < GetSize() && (nIndex > 0))
        {
            int i = 0;
            for (typename Array<T>::iterator it = BASE::begin(); it != BASE::end(); it++,i++ )
            {
                if (i == nIndex)
                {
                    T t = *it;
                    BASE::erase(it);
                    return t;
                }
            }
        }

        return T();
    }

    void Push(const T& value)
    {
        Add(value);
    }

    T Pop()
    {
        return RemoveAt(this->GetSize() - 1);
    }

    T Shift()
    {
        return RemoveAt(0);
    }

    void Unshift(const T& t)
    {
        BASE::insert(BASE::begin(), t);
    }

    T* GetData()
    {
        if (GetSize() < 1)
        {
            return NULL;
        }
        return &((*this)[0]);
    }

    const T* GetData() const
    {
        if (GetSize() < 1)
        {
            return NULL;
        } 
        return &((*this)[0]);
    }

    T* SetSize(int nSize)
    {
        BASE::resize(nSize);
        return GetData();
    }

    void RemoveAll()
    {
        BASE::clear();
    }

    Array<T>(const Array<T>& src)
    {
        Copy(src);
    }

    bool IsEmpty() const
    {
        return (GetSize() < 1);
    }

    int Find(const T& data) const
    {
        for (int i = 0;i < GetSize(); i++)
        {
            if ((*this)[i] == data)
            {
                return i;
            }
        }
        return -1;
    }

    bool IsValidIndex(int i) const
	{
		return (i >= 0) && (i < GetSize());
	}
};
#undef BASE




