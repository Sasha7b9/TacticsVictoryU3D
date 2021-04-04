// Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "FileSystem/ConfigurationFile_.h"
#include "FileSystem/FileSystem_.h"


#define CHECK_ON_VALID_STRING   if (!isValid) { LOGERROR("Configuration file is not valid"); return nullptr; }
#define CHECK_ON_VALID_INT      if (!isValid) { LOGERROR("Configuration file is not valid"); return -1; }


#define ERROR_FUNCTION      LOGERRORF("%s has not realisation", __FUNCTION__);


bool ConfigurationFile::Load(pchar name)
{
    document = new rapidjson::Document();

    FS::File file;
    file.Open((std::string("conf") + FS::delimiter + name).c_str(), __FILE__, __LINE__);

    if (!file.IsOpened())
    {
        LOGERRORF("Can't load configuration file %s", name);

        return false;
    }

    std::string buffer;

    file.Read(buffer, file.Size());

    document->Parse(buffer.c_str());

    if (document->HasParseError())
    {
        LOGERRORF("Can't parse configuration file %s. Error %d", name, document->GetParseError());

        isValid = false;

        return false;
    }
    
    LOGWRITEF("Configuration file %s is parsed", name);

    isValid = true;

    return true;
}


void ConfigurationFile::Unload()
{
    SAFE_DELETE(document);
}


int ConfigurationFile::GetInt(pchar key1, pchar key2, pchar key3, pchar key4)
{
    auto it = FindMember(key1, key2, key3, key4);

    if (&*it && it->value.IsInt())
    {
        return it->value.GetInt();
    }

    return -1;
}


pchar ConfigurationFile::GetString(pchar key1, pchar key2, pchar key3, pchar key4)
{
    auto it = FindMember(key1, key2, key3, key4);

    if (&*it && it->value.IsString())
    {
        return it->value.GetString();
    }

    return "";
}


float ConfigurationFile::GetFloat(pchar key1, pchar key2)
{
    auto it = FindMember(key1, key2);

    if (&*it && it->value.IsFloat())
    {
        return it->value.GetFloat();
    }

    return 0.0f;
}


rapidjson::Value::ConstMemberIterator ConfigurationFile::FindMember(pchar key1, pchar key2, pchar key3, pchar key4)
{

#define IS_VALID(x)            (&*(x))
#define IS_VALID_AND_KEY(x, k) (IS_VALID(x) && k[0])

    auto it = document->FindMember(key1);

    if (IS_VALID(it) && it != document->MemberEnd())
    {
        if(IS_VALID_AND_KEY(it, key2))
        {
            if (it->value.HasMember(key2))
            {
                it = it->value.FindMember(key2);

                if (IS_VALID_AND_KEY(it, key3))
                {
                    if (it->value.HasMember(key3))
                    {
                        it = it->value.FindMember(key3);

                        if (IS_VALID_AND_KEY(it, key4))
                        {
                            if (it->value.HasMember(key4))
                            {
                                it = it->value.FindMember(key4);

                                if (IS_VALID(it) && it != document->MemberEnd())
                                {
                                    return it;
                                }
                            }
                            else
                            {
                                LOGERRORF("Can not find value for \"%s\" \"%s\" \"%s\" \"%s\"", key1, key2, key3, key4);
                            }
                        }
                    }
                    else
                    {
                        LOGERRORF("Can not find value for \"%s\" \"%s\" \"%s\" \"%s\"", key1, key2, key3, key4);
                    }
                }
            }
            else
            {
                LOGERRORF("Can not find value for \"%s\" \"%s\" \"%s\" \"%s\"", key1, key2, key3, key4);
            }
        }
    }

    if (!IS_VALID(it) || it == document->MemberEnd())
    {
        LOGERRORF("Can not find value for \"%s\" \"%s\" \"%s\" \"%s\"", key1, key2, key3, key4);
    }

    return it;
}


bool ConfigurationFile::GetVectorStrings(pchar key, std::vector<std::string> &strings)
{
    strings.clear();

    auto it = FindMember(key);

    if (&*it && it->value.IsArray() && it->value.Size() > 0)
    {
        for (auto elem = it->value.Begin(); elem != it->value.End(); ++elem)
        {
            if (elem->IsString())
            {
                strings.push_back(elem->GetString());
            }
            else
            {
                LOGERRORF("\"%s\" array, but element not string", key);
            }
        }
    }

    return (strings.size() != 0);
}


IntVector2 ConfigurationFile::GetIntVector2(pchar key1, pchar key2, pchar key3, pchar key4)
{
    IntVector2 result(0, 0);

    auto it = FindMember(key1, key2, key3, key4);

    if (&*it)
    {
        auto &value = it->value;

        if (value.IsArray())
        {
            if (value.Size() == 2)
            {
                auto elem = value.Begin();

                if (elem->IsInt())
                {
                    result.x_ = elem->GetInt();
                }
                else
                {
                    LOGERRORF("\"%s\" \"%s\" \"%s\" \"%s\" array, but first element not integer", key1, key2, key3, key4);
                }

                elem++;

                if (elem->IsInt())
                {
                    result.y_ = elem->GetInt();
                }
                else
                {
                    LOGERRORF("\"%s\" \"%s\" \"%s\" \"%s\" array, but second element not integer", key1, key2, key3, key4);
                }
            }
            else
            {
                LOGERRORF("\"%s\" \"%s\" \"%s\" \"%s\" array, but size not 2", key1, key2, key3, key4);
            }
        }
        else
        {
            LOGERRORF("\"%s\" \"%s\" \"%s\" \"%s\" not array", key1, key2, key3, key4);
        }
    }

    return result;
}



void ConfigurationFile::SetInt(pchar /*key*/, int /*value*/)
{
    LOGERRORF("%s has not realisation", __FUNCTION__);
}


void ConfigurationFile::SetInt(pchar /*key1*/, pchar /*key2*/, int /*value*/)
{
    LOGERRORF("%s has not realisation", __FUNCTION__);
}
