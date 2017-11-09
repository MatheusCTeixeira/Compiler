#include <fstream>
#include <string>

#ifndef _FILE_READER_
#define _FILE_READER_

namespace comp{
    class file_reader{
    public:
        file_reader(std::string filename);

        void open(std::string filename);
        void close();

        bool is_opened() const;
        bool is_closed() const;
        bool eof();

        std::string next_line();
        
    private:
        std::ifstream m_file;
    };
}

#endif