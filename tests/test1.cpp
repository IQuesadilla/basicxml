#include "../basicxml.h"
#include "iostream"

const char xml[] = " \
<db> \
    <table val1=\"123\" val2=\"value\"> \
        <column rows=\"int;float;string\"> \
            <row> \
                <int>7</int> \
                <float>4.6</float> \
                <string>   hey yo   </string> \
                <db var=\"1\" /> \
            </row> \
        </column> \
    </table> \
</db> \
";

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
        size_t *index = (size_t*)ptr;
        size_t count = buffsize;

        if ( (sizeof(xml) - *index) < buffsize)
        {
            count = sizeof(xml) - *index;
        }

        for (size_t i = 0; i < count; ++i)
            buffer[i] = xml[i + *index];

        *index += count;
        return count;
    }
};

int main()
{
    mybasicxml parser;
    size_t index = 0;
    parser.setUserPtr(&index);
    int ret = parser.parse();
    return ret;
}