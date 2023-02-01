#pragma once

#define BASE    std::map<T1, T2>
#define DEFVAL  (*m_pDefaultValue)

template <typename T1, typename T2>
class MapDefPtr : public BASE
{
protected:
    T2*     m_pDefaultValue;
private:
    /* data */
public:
    MapDefPtr(T2 *pDefaultValue)
    {
        m_pDefaultValue = pDefaultValue;
    }
    virtual ~MapDefPtr()
    {

    }

    int GetSize()
    {
        return (int)BASE::size();
    }

};


