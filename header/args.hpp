#include <iostream>
#include <regex>
#include <string>

#ifndef _ARGS_
#define _ARGS_

namespace comp
{
struct info
{
    std::string m_filename;
    std::string m_outputfilename;
    unsigned int m_list_tokens : 1;
    unsigned int m_syn_log : 1;
    unsigned int m_sem_log : 1;
    unsigned int m_cod_log : 1;

    void show_all()
    {
        std::cout << "filename: " << this->m_filename << std::endl;       
        std::cout << "output filename: " << this->m_outputfilename << std::endl;  
        std::cout << "-lt:" << this->m_list_tokens << std::endl;
        std::cout << "-ls:" << this->m_list_tokens << std::endl;
        std::cout << "-sl:" << this->m_list_tokens << std::endl;
    }
};

info process_params(char **params, int count)
{
    info t_info{};

    for (int i = 1; i < count; ++i)
    {
        std::string param{params[i]};
        std::cout << param << std::endl;
        if (std::regex_match(param, std::regex{"[[:print:]]*(\\.hhh)"}))
        {
            t_info.m_filename = param;
        }
        else if (std::regex_match(param, std::regex{"-all"}))
        {
            t_info.m_list_tokens = (unsigned int)(1);
            t_info.m_syn_log = (unsigned int)(1);
            t_info.m_sem_log = (unsigned int)(1);
        }
        else if (std::regex_match(param, std::regex{"-lt"}))
        {
            t_info.m_list_tokens = (unsigned int)(1);
        }
        else if (std::regex_match(param, std::regex{"-ls"}))
        {
            t_info.m_syn_log = (unsigned int)(1);
        }
        else if (std::regex_match(param, std::regex{"-lse"}))
        {
            t_info.m_sem_log = (unsigned int)(1);
        }
        else if (std::regex_match(param, std::regex{"-lgc"}))
        {
            t_info.m_cod_log = (unsigned int)(1);
        }
        else if (std::regex_match(param, std::regex{"[[:print:]]*(\\.asm)"}))
        {
            t_info.m_outputfilename = param;
        }
    }

    return t_info;
}
}

#endif