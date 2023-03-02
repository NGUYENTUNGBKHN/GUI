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
        // nothing
    }

    int GetSize()
    {
        return (int)BASE::size();
    }

    bool IsContain(const T1& key) const
    {
        return (BASE::find(key) != BASE::end());
    }

    bool has(const T1& key) const 
    {
        return IsContain(key);
    }

    Array<T1> Keys() const
    {
        Array<T1> a;
        for (typename BASE::const_itertor i = BASE::begin(); i!=BASE::end(); i++)
        {
            a.Add(i->first);
        }
        return a;
    }

    T2& Get(const T1& key)
    {
        typename BASE::iterator it = BASE::find(key);
        if (it != BASE::end())
        {
            return it->second;
        }
        return DEFVAL;
    }

    const T2& GetConst(const T1& key) const
    {
        typename BASE::const_iterator it = BASE::find(key);
        if (it != BASE::end())
        {
            return it->second;
        }
        return DEFVAL;
    }

    T2& operator[](const T1& key)
    {
        typename BASE::iterator it = BASE::find(key);
        if (it == BASE::end())
        {
            BASE::insert(std::pair<T1,T2>(key, DEFVAL));
            it = BASE::find(key);
        }
        return it->second;
    }

    void SetDefaultValue(const T2& value)
    {
        DEFVAL = value;
    }
};

#undef BASE
#undef DEFVAL

