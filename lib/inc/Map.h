#ifndef __MAP_H
#define __MAP_H

template <typename T1, typename T2>
class Map : public MapDefPtr<T1, T2>
{
private:
    /* data */
    T2  m_DefaultValue;
public:
    Map(/* args */) : MapDefPtr<T1,T2>(&m_DefaultValue)
    {

    }
    ~Map()
    {

    }


    void Append(Map<T1,T2> mSrc)
    {
        Array<T1> a = mSrc.Keys();
        for (int i = 0; i < a.GetSize(); i++)
        {
            T1 key = a[i];
            T2 val = mSrc[key];
            (*this)[key] = val;
        }
    }
};

#endif



