#pragma once

class JImage_mem_ARGB8888 : public JImage
{
private:
    /* data */
public:
    int     PIXEL;
    int     ALINE;
    int     LINE;
    BYTE*   m_pData;
public:
    JImage_mem_ARGB8888(/* args */)
    {

    }
    ~JImage_mem_ARGB8888()
    {

    }
};





