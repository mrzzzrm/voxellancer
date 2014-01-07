#pragma once

#include <iostream>
#include <streambuf>
#include <string>


#include "ui/hud.h"

class StreamRedirect : public std::basic_streambuf<char>
{
public:
    StreamRedirect(std::ostream &stream, HUD *hud, bool copy = false) : m_stream(stream)
    {
        m_hud = hud;
        m_old_buf = stream.rdbuf();
        m_old_wrap = new std::ostream(m_old_buf);
        m_copy = copy;
        stream.rdbuf(this);
    }
    ~StreamRedirect()
    {
        // output anything that is left
        if (!m_string.empty())
            m_hud->streamCallback(m_string);
        delete m_old_wrap;
        m_stream.rdbuf(m_old_buf);
    }

protected:
    virtual int_type overflow(int_type v)
    {

        if (v == '\n')
        {
            m_hud->streamCallback(m_string);
            if (m_copy) *m_old_wrap << m_string << std::endl;
            m_string.erase(m_string.begin(), m_string.end());
        } else
            m_string += v;

        return v;
    }

    virtual std::streamsize xsputn(const char *p, std::streamsize n)
    {

        m_string.append(p, p + n);

        size_t pos = 0;
        while (pos != std::string::npos)
        {
            pos = m_string.find('\n');
            if (pos != std::string::npos)
            {
                std::string tmp(m_string.begin(), m_string.begin() + pos);
                m_hud->streamCallback(tmp);
                if (m_copy) *m_old_wrap << tmp << std::endl;
                m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
            }
        }

        return n;
    }

private:
    std::ostream &m_stream;
    std::ostream *m_old_wrap;
    std::basic_ios<char, std::char_traits<char>>::_Mysb *m_old_buf;
    std::string m_string;
    bool m_copy;
    HUD* m_hud;
};