#include <fstream>
#include <string>

#ifndef _FILE_WRITER_
#define _FILE_WRITER_

namespace comp{
    class file_writer{
    public:
        file_writer(std::string filename);

        void open(std::string filename);
        void close();

        bool is_opened() const;
        bool is_closed() const;
        bool eof();

        void write_line(std::string line);
        
    private:
        std::ofstream m_file;
    };
}

#endif