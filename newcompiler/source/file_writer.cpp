#include "../header/file_writer.hpp"

namespace comp{
    file_writer::file_writer(std::string filename):m_file{filename, std::ofstream::out}{
        
    }

    void file_writer::open(std::string filename){
       this-> m_file.open(filename, std::ofstream::out);
    }

    void file_writer::close(){
        this->m_file.close();
    }

    bool file_writer::is_opened() const {
        return this->m_file.is_open();
    }

    bool file_writer::is_closed() const{
        return !this->is_opened();
    }

    bool file_writer::eof(){
        return this->m_file.eof();
    }

    void file_writer::write_line(std::string line){
        line += "\n";
        this->m_file.write(line.c_str(), line.size());
    }

}