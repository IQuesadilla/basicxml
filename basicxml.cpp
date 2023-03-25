#include "basicxml.h"

#include <iostream>

#ifndef __cxa_pure_virtual
extern "C" void __cxa_pure_virtual()
{
    while(1);
}
#endif

basicxml::basicxml()
{
    flags.isInsideBrackets = false;
    flags.lastIteration = false;
}

int basicxml::parse()
{
    char strbuff[300];
    element e;
    char *attstring = nullptr;
    int attslen = 0;
    //attribute a;
    //e.atts = &a;

    flags.doCallback = false;

    size_t strbuffit = 0;
    while (!flags.lastIteration)
    {
        char buffer[buffersize];
        size_t size = loadcallback(buffer,buffersize,userptr);
        if (size < buffersize)
        {
            flags.lastIteration = true;
        }

        //std::cout << "[" << size << "] \"" << std::string(buffer,size) << "\"" << std::endl;

        for (char *c = buffer; c != &buffer[size]; ++c)
        {
            if (flags.isInsideBrackets)
            {
                if (*c == '>')
                {
                    if (flags.isLoadingName)
                    {
                        //e.name = strbuff;
                        //e.namelen = &strbuff[strbuffit] - e.name + 1;
                        //attslen = 0;
                        e.atts = nullptr;
                        //parsecallback(e,userptr);
                    }
                    else if (flags.isLoadingAtts)
                    {
                        //e.atts->name = e.name;
                        //e.atts->namelen = &strbuff[strbuffit] - (e.name + 1);
                        //parse_attributes(nullptr,attstring,attslen,e);
                    }

                    //e.namelen++;
                    //e.atts->namelen++;
                    //strbuff[++strbuffit] = '\0';
                    /*if (attstring != nullptr)
                    {
                        parse_attributes(nullptr,attstring,attslen,e);
                    }
                    else
                    {
                        e.atts = nullptr;
                        parsecallback(e,userptr);
                    }*/
                
                    //parsecallback(e,userptr);

                    if ( !e.closing )
                    {
                        //flags.isLoadingVal = true;
                        //e.value = &strbuff[strbuffit];
                    }

                    flags.isLoadingName = false;
                    flags.isLoadingAtts = false;
                    flags.isInsideBrackets = false;
                    //e.closing = false;
                }

                else if (*c == '/')
                {
                    e.closing = true;
                }

                else if ( whitespace(*c) && flags.isLoadingName )
                {
                    //e.name = c;
                    //e.namelen = &strbuff[strbuffit] - e.name + 1;
                    //strbuff[strbuffit] = '\0';
                    flags.isLoadingName = false;
                    //flags.isLoadingAtts = false;
                    //attslen = 0;
                }

                else
                {
                    if ( !flags.isLoadingName && !flags.isLoadingAtts )
                    {
                        //std::cout << "HERE HERE HERE" << std::endl;
                        strbuff[strbuffit++] = '\0';
                        attstring = &strbuff[strbuffit];
                        flags.isLoadingAtts = true;
                    }

                    strbuff[strbuffit++] = *c;

                    if (flags.isLoadingName)
                        ++e.namelen;
                    else if (flags.isLoadingAtts)
                        ++attslen;
                }
            }
            else
            {
                if (*c == '<')
                {
                    if (flags.doCallback)
                    {
                        if ( e.closing )
                            e.valuelen = 0;
                        
                        strbuff[strbuffit] = '\0';

                        run_parsecallback(e,attstring,attslen);
                    }

                    flags.isInsideBrackets = true;
                    flags.isLoadingVal = false;
                    flags.isLoadingName = true;
                    flags.isLoadingAtts = false;
                    flags.doCallback = true;

                    e.name = strbuff;
                    e.namelen = 0;
                    e.value = nullptr;
                    e.valuelen = 0;
                    e.closing = false;
                    strbuffit = 0;
                    attstring = nullptr;
                    attslen = 0;
                }

                else if ( !whitespace(*c) )
                {
                    if (e.value == nullptr)
                    {
                        if (e.atts == nullptr)
                            strbuff[strbuffit++] = '\0';

                        e.value = &strbuff[strbuffit];
                    }
                    else
                    {
                        if (flags.isLoadingVal == false)
                        {
                            strbuff[strbuffit++] = ' ';
                            ++e.valuelen;
                        }
                    }

                    strbuff[strbuffit++] = *c;
                    ++e.valuelen;

                    flags.isLoadingVal = true;
                }

                else
                {
                    flags.isLoadingVal = false;
                }
                
                /*
                if (flags.isLoadingVal)
                {
                    if ( whitespace(*c) )
                    {
                        flags.isLoadingVal = false;
                        strbuff[strbuffit++] = ' ';
                    }
                    else
                    {
                        strbuff[strbuffit++] = *c;
                    }

                    ++e.valuelen;
                }*/
            }
        }
    }
    
    //*e.value = '\0';
    strbuff[strbuffit] = '\0';
    e.valuelen = 0;
    run_parsecallback(e,attstring,attslen);

    return 0;
}

void basicxml::setUserPtr(void *ptr)
{
    userptr = ptr;
}

void basicxml::parse_attributes(attribute *a, char *attstring, size_t length, element e)
{
    size_t i = 0;
    attribute newa;
    bool lastit = true;

    for (; i < length; ++i)
    {
        if ( !whitespace(attstring[i]) )
        {
            newa.name = &attstring[i];
            lastit = false;
            break;
        }
    } ++i;

    if (lastit)
    {
        parsecallback(e,userptr);
        return;
    }

    for (; i < length; ++i )
    {
        if ( attstring[i] == '=' )
        {
            newa.namelen = &attstring[i] - newa.name;
            attstring[i] = '\0';
            break;
        }
    } ++i;

    for (; i < length; ++i )
    {
        if ( attstring[i] == '\"' )
        {
            newa.value = &attstring[i+1];
            break;
        }
    } ++i;

    for (; i < length; ++i )
    {
        if ( attstring[i] == '\"' )
        {
            newa.valuelen = &attstring[i] - newa.value;
            attstring[i] = '\0';
            break;
        }
    } ++i;
    //char name[equalsloc];
    //char value[currentlen - equalsloc];

    /*
    newa.name = attstring;
    newa.namelen = equalsloc;
    newa.value = &attstring[equalsloc+1];
    newa.valuelen = currentlen - (equalsloc+1);
    newa.next = nullptr;
    */

    newa.next = nullptr;

    //std::cout << "HERHERHERHERHE" << std::string(attstring,length) << std::endl;

    if (a == nullptr)
    {
        e.atts = &newa;
    }
    else
    {
        a->next = &newa;
    }

    parse_attributes(&newa,&attstring[i],length - i, e);
}

void basicxml::run_parsecallback(element e, char *attstring, size_t attslen)
{
    char null = '\0';
    if (e.valuelen == 0)
    {
        e.value = &null;
    }

    if ( attstring == nullptr )
    {
        parsecallback(e,userptr);
    }
    else
    {
        parse_attributes(nullptr,attstring,attslen,e);
    }
}

bool basicxml::whitespace(char c)
{
    if (
            c == ' '  ||
            c == '\r' ||
            c == '\n'
        )
        return true;
    return false;
}