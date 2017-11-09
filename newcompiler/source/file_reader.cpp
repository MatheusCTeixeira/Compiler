#include "../header/file_reader.hpp"

namespace comp{
    file_reader::file_reader(std::string filename):m_file{filename, std::ifstream::in}{
        
    }

    void file_reader::open(std::string filename){
       this-> m_file.open(filename, std::ifstream::in);
    }

    void file_reader::close(){
        this->m_file.close();
    }

    bool file_reader::is_opened() const {
        return this->m_file.is_open();
    }

    bool file_reader::is_closed() const{
        return !this->is_opened();
    }

    bool file_reader::eof(){
        return this->m_file.eof();
    }

    std::string file_reader::next_line(){
        char txt[512];
        this->m_file.getline(txt, 512);
        return std::string{txt};        
    }

}