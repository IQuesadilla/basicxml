#ifndef BASICXML_H
#define BASICXML_H
#pragma once
typedef unsigned long size_t;
typedef unsigned int uint;

struct attribute
{
    char *name;
    size_t namelen;
    char *value;
    size_t valuelen;

    attribute *next;
};

struct element
{
    char *name;
    size_t namelen;
    char *value;
    size_t valuelen;

    bool closing;

    attribute *atts;
};

class basicxml
{
public:
    basicxml();
    ~basicxml(){};

    int parse();

    // void setLoadCallback(int (*callback)(char*,size_t,void*), void* userptr);
    // void setParseCallback(void (*callback)(element,void*), void* userptr);
    void setUserPtr(void *ptr);

    static const size_t buffersize = 10;

private:
    struct
    {
        bool lastIteration : 1;
        bool isInsideBrackets : 1;
        bool isLoadingName : 1;
        bool isLoadingAtts : 1;
        bool isLoadingVal : 1;
        bool doCallback : 1;
        bool isComment : 1;
        uint findingComment : 2;
    } flags;

    virtual int loadcallback(char *, size_t, void *) = 0;
    virtual void parsecallback(element, void *) = 0;

    void parse_attributes(attribute *a, char *attstring, size_t length, element e);

    void run_parsecallback(element e, char *attstring, size_t attslen);
    bool whitespace(char c);

    // int (*loadcallback)(char*,size_t,void*);
    // void* loadcallbackuserptr;
    // void (*parsecallback)(element,void*);
    // void* parsecallbackuserptr;
    void *userptr;
};

#endif