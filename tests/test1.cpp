#include "../basicxml.h"
#include <iostream>
#include <fstream>

typedef struct
{
    std::ifstream ifs;
    size_t index;
} myptr;

class mybasicxml : public basicxml
{
    void parsecallback(element e, void* userptr)
    {
        //std::cout << "\"" << std::string(string,len) << "\"" << std::endl;
        
        std::cout << "Callback:" << std::endl;
        std::cout << "Closing: " << ((e.closing) ? "true" : "false") << std::endl;
        std::cout << "Namelen: " << e.namelen << std::endl;
        std::cout << "Name: \"" << e.name << "\"" << std::endl;
        for (auto ptr = e.atts; ptr != nullptr; ptr = ptr->next)
        {
            std::cout << "  AttNameLen: " << ptr->namelen << std::endl;
            std::cout << "  AttName: \"" << ptr->name << "\"" << std::endl;
            std::cout << "  AttValueLen: " << ptr->valuelen << std::endl;
            std::cout << "  AttValue: \"" << ptr->value << "\"" << std::endl;
        }

        if (e.valuelen > 0)
        {
            std::cout << "ValueLen: " << e.valuelen << std::endl;
            std::cout << "Value: \"" << e.value << "\"" << std::endl;
        }

        //for (size_t i = 0; i < e.namelen; ++i)
        //    std::cout << "[" << e.name[i] << "]";
        std::cout << std::endl;
    }

    int loadcallback(char* buffer, size_t buffsize, void* ptr)
    {
        auto myp = (myptr*)ptr;

        return myp->ifs.readsome(buffer, buffsize);
        //return ;
    }
};

int main()
{
    mybasicxml parser;

    myptr myp;
    myp.index = 0;
    myp.ifs.open("tests/test1.xml");

    parser.setUserPtr(&myp);
    int ret = parser.parse();
    return ret;
}