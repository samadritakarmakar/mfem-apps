#ifndef JSONROOTPARSER_HPP
#define JSONROOTPARSER_HPP
#include <jsoncpp/json/json.h> // or jsoncpp/json.h , or json/json.h etc.
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace  mfem
{
class JsonRootParser
{
public:
    JsonRootParser(const char **fileName)
    {
        _fileName = std::string(*fileName);
        ifs= std::ifstream(_fileName);
        reader.parse(ifs, obj);
        if(ifs.fail())
        {
            std::cout<<"File "<<_fileName<<" not found!\n";
        }
    }

    JsonRootParser(std::string fileName)
    {
        _fileName = fileName;
        ifs= std::ifstream(_fileName);
        reader.parse(ifs, obj);
        if(ifs.fail())
        {
            std::cout<<"File "<<_fileName<<" not found!\n";
        }
    }

    std::string AddOption(const char **className, const char* key)
    {
        if(obj[key].isNull())
        {
            std::cout<<"Warning, the given key: "<<key<<" does not exist!\n";
        }
        else if(obj[key].isArray())
        {
            std::cout<<"Wrong if else getting used!\n";
        }
        else
        {
            GetData(obj[key], className, key);
        }
        return (std::string)key;
    }

    template<class GenericClass>
    std::string AddOption(GenericClass *className, const char* key)
    {
        if(obj[key].isNull())
        {
            std::cout<<"Warning, the given key: "<<key<<" does not exist!\n";
        }
        else if(obj[key].isArray())
        {
            std::cout<<"Wrong if else getting used!\n";
        }
        else
        {
            GetData(obj[key], className, key);
        }
        return (std::string)key;
    }

    template<class GenericClass>
    std::string AddOption(std::vector<GenericClass> *className, const char* key)
    {
        if(obj[key].isNull())
        {
            std::cout<<"Warning, the given key: "<<key<<" does not exist!\n";
        }
        else if(obj[key].isArray())
        {
            *className= std::vector<GenericClass>(obj[key].size());
            for (unsigned int i=0; i<className->size(); i++)
            {
                GetData(obj[key][i], &className->operator[](i), key);
            }
        }
        else
        {
            *className=std::vector<GenericClass>(1);
            GetData(obj[key], &className->operator[](0), key);
        }
        return (std::string)key;
    }

    template<class GenericClass>
    std::string AddOption(std::vector<std::vector<GenericClass>> *className, const char* key)
    {
        if(obj[key].isNull())
        {
            std::cout<<"Warning, the given key: "<<key<<" does not exist!\n";
        }
        else if(obj[key].isArray() && obj[key][0].isArray())
        {
            *className= std::vector<std::vector<GenericClass>>(obj[key].size());
            for (unsigned int i=0; i<className->size(); i++)
            {
                className->operator[](i)= std::vector<GenericClass>(obj[key][i].size());
                for (unsigned int j=0; j<className->operator[](i).size(); j++)
                {
                    GetData(obj[key][i][j], &className->operator[](i)[j], key);
                }
            }
        }
        return (std::string)key;
    }

private:
    std::string _fileName;
    std::ifstream ifs;
    Json::Reader reader;
    Json::Value obj;
    std::vector<std::string> charString;

    void GetData(const Json::Value &obj_local, const char **classNameElement, const char* key)
    {

        //std::cout<<"Key :"<<key<<" detected as char string\n";
        std::string buffer(obj_local.asCString());
        charString.push_back(buffer);
        *classNameElement = charString[charString.size()-1].c_str();
        //std::cout<<"CString = "<<*classNameElement<<"\n";
    }

    void GetData(const Json::Value &obj_local, std::string *classNameElement, const char* key)
    {
        std::cout<<"Key :"<<key<<" detected as string\n";
        *classNameElement = obj_local.asString();
    }

    template<class GenericClass>
    void GetData(const Json::Value &obj_local, GenericClass *classNameElement, const char* key)
    {
        if(obj_local.isUInt())
        {
            *classNameElement = obj_local.asUInt();
        }
        else if(obj_local.isBool())
        {
            *classNameElement = obj_local.asBool();
        }
        else if (obj_local.isInt())
        {
            *classNameElement = obj_local.asInt();
        }
        else if (obj_local.isDouble())
        {
            *classNameElement = obj_local.asDouble();
        }
        else if (obj_local.isString())
        {
            std::cout<<"Wrong if else getting used!\n";
        }
        else
        {
            std::cout<<"Warning! No reasonable matching type to the JSON "<<key<<" object found!\n";
        }
    }
};
}
#endif // JSONROOTPARSER_HPP
