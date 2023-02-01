#pragma once

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
};





