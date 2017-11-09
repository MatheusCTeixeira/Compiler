#include <iostream>
#include <regex>
#include <string>

#ifndef _ARGS_
#define _ARGS_

namespace comp{
    struct info{
        std::string m_filename;
        unsigned int m_list_tokens: 1;   
        unsigned int m_log: 1; 

        void show_all(){
            std::cout << "filename: " << this->m_filename << std::endl;
            std::cout << "-lt:" << this->m_list_tokens << std::endl;       
        }
    };

    info process_params(char ** params, int count){
        info t_info{};

        for(int i = 1; i < count; ++i){
            std::string param{params[i]};
            if (std::regex_match(param, std::regex{"[[:print:]]*(\\.hhh)"})){
                t_info.m_filename = param;
            }
            else if (std::regex_match(param, std::regex{"-lt"})){
                t_info.m_list_tokens = (unsigned int)(1); 
            }
            else if (std::regex_match(param, std::regex{"-all"})){
                t_info.m_list_tokens = (unsigned int)(1); 
                t_info.m_log = (unsigned int)(1); 
            }
            else if (std::regex_match(param, std::regex{"-ls"})){                
                t_info.m_log = (unsigned int)(1); 
            }         
        } 

        return t_info;
    }
}

#endif