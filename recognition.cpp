#include "recognition.h"
 
std::vector<std::string> Recognition::strsplit( const std::string& input, const std::string& delimiter )
{
    std::vector<std::string> vec;
    if( input.empty() || delimiter.empty() )
        return vec;

    std::string str = input;
    size_t pos = 0;
    std::string token;
    while ( ( pos = str.find( delimiter ) ) != std::string::npos )
    {
        token = str.substr( 0, pos );
        vec.push_back( token );
        str.erase( 0, pos + delimiter.size() );
    }
    
    if( str.size() > 0 )
        vec.push_back( str );

    return vec;
}

std::vector<std::string> Recognition::prepareInputFileToLines( const std::string& fileName )
{
    std::vector<std::string> vec;
    std::ifstream file( fileName );
    std::vector<std::string> arr;
    std::string str;

    while( std::getline(file, str) )
    {
        vec.push_back( std::move(str) );
    }

    return vec;
}

bool Recognition::find( const std::string& input, const std::string& pattern )
{
    return strstr( input.c_str(), pattern.c_str() );
}

bool Recognition::init( const std::string& folder, const std::string& operationFile, const std::string& questionsFile, const std::string& subjectListFile, const std::string& objectTypeListFile )
{
    if( folder.empty() || operationFile.empty() || questionsFile.empty() || subjectListFile.empty() || objectTypeListFile.empty() )
    {
        return false;
    }

    if( !std::filesystem::exists( folder + operationFile ) )
    {
        std::cerr << "error : operation file couldn't file at : " << folder + operationFile << "\n";
        return false;
    }
    
    if( !std::filesystem::exists( folder + questionsFile ) )
    {
        std::cerr << "error : questions file couldn't file at : " << folder + questionsFile << "\n";
        return false;
    }
    
    if( !std::filesystem::exists( folder + subjectListFile ) )
    {
        std::cerr << "error : subjects file couldn't file at : " << folder + subjectListFile << "\n";
        return false;
    }

    if( !std::filesystem::exists( folder + objectTypeListFile ) )
    {
        std::cerr << "error : objects file couldn't file at : " << folder + objectTypeListFile << "\n";
        return false;
    }
    
    operationList = prepareInputFileToLines( folder + operationFile );
    for( auto const& item : prepareInputFileToLines( folder + questionsFile ) )
    {
        auto strArr = strsplit( item, "=" );
        if( strArr.empty() || strArr.size() < 2 )
            continue;
        
        auto opIter = std::find( operationList.begin(), operationList.end(), strArr[1] );
        if( opIter == operationList.end() )
            continue;

        questions.insert( { strArr[0], opIter } );
    }

    auto objectLines = prepareInputFileToLines( folder + objectTypeListFile );
    objectTypeList.reserve(objectLines.size());

    for( auto const& line : objectLines )
    {
        auto lineArr = strsplit( line, "=" ); // subject=operationList=subsubject
        if( lineArr.empty() || lineArr.size() < 2 ) continue;
        if( lineArr[1].empty()) continue;

        auto objects = strsplit( lineArr[1], "," );
        objectsLinkMap.insert( {lineArr[0], objects} );
    }

    auto subjectLines = prepareInputFileToLines( folder + subjectListFile );
    subjectList.reserve( subjectLines.size() );
    for( auto const& line : subjectLines )
    {
        auto lineArr = strsplit( line, "=" ); // subject=operationList=subsubject
        if( lineArr.empty() || lineArr.size() < 2 ) continue;
        if( lineArr[1].empty()) continue;

        std::vector<std::string>::iterator operationIter;
        if( lineArr[1].find(",") != std::string::npos )
        {
            auto operationArr = strsplit( lineArr[1], "," );
            if( operationArr.empty() || operationArr.size() < 2 )
                continue;

            for( int index = 0; index < operationArr.size(); ++index )
            {
                if( operationArr[index].empty() )
                    continue;
                operationIter = std::find( operationList.begin(), operationList.end(), operationArr[index] );
                if( operationIter == operationList.end() )
                    continue;
                    
                auto iter = std::find( subjectList.begin(), subjectList.end(), lineArr[0] );
                if( iter == subjectList.end() )
                    subjectList.push_back( lineArr[0] );
                subjectLinkMap.insert( { operationIter, subjectList.end() - 1 } );
            }
        }
        else
        {
            operationIter = std::find( operationList.begin(), operationList.end(), lineArr[1] );
            if( operationIter == operationList.end() )
                continue;

            subjectList.push_back( lineArr[0] );
            subjectLinkMap.insert( { operationIter, subjectList.end() - 1 } );
        }

        if( lineArr.size() > 2 )
        {
            if( lineArr[2].empty()) continue;
            auto subsubject = lineArr[2];
            if( subsubject.empty() || subsubject.size() < 2 ) continue;

            if( subsubject.find(",") != std::string::npos )
            {
                auto subsubjectArr = strsplit( subsubject, "," );
                if( subsubjectArr.empty() || subsubjectArr.size() < 2 ) continue;

                for( int index = 0; index < subsubjectArr.size(); ++ index )
                {
                    auto iter = std::find( objectTypeList.begin(), objectTypeList.end(), subsubjectArr[index] );
                    if( iter == objectTypeList.end() )
                    {
                        objectTypeList.push_back( subsubjectArr[index] );
                    }
                    objectTypeListLinkMap.insert( { subjectList.end() - 1, objectTypeList.end() - 1 } );
                }
            }
            else
            {
                auto iter = std::find( objectTypeList.begin(), objectTypeList.end(), subsubject );
                if( iter == objectTypeList.end() )
                {
                    objectTypeList.push_back( subsubject );
                }
                objectTypeListLinkMap.insert( { subjectList.end() - 1, objectTypeList.end() - 1 } );
            }
        }
    }

    return true;
}

std::string Recognition::recognize( const std::string& input )
{
    std::string output = "";

    for( auto const& question : questions )
    {
        bool bret = find( input, question.first );
        if( bret ) // operation found
        {
            string_vector_citer operationIter = question.second;
            
            auto range = subjectLinkMap.equal_range( operationIter );
            for ( auto rangeIter = range.first; rangeIter != range.second; ++rangeIter )
            {
                bret = find( input, *(rangeIter->second) );
                if( bret ) // operation found
                {
                    std::string tmp = *(rangeIter->first);
                    tmp[0] = std::toupper( tmp[0] );
                    output += tmp + " ";

                    tmp = *(rangeIter->second);
                    tmp[0] = std::toupper( tmp[0] );
                    output += tmp;

                    string_vector_citer subjectIter = rangeIter->second;
                    range = objectTypeListLinkMap.equal_range( subjectIter );
                    for ( auto rangeIter = range.first; rangeIter != range.second; ++rangeIter )
                    {
                        auto objects = objectsLinkMap[*(rangeIter->second)];
                        for( auto const& object : objects )
                        {
                            bret = find( input, object );
                            if( bret ) // object found
                            {
                                tmp = *(rangeIter->second);
                                tmp[0] = std::toupper( tmp[0] );
                                output += " " + tmp;
                            }
                        }
                    }
                    return output;
                }
            }
        }
    }
    
    return output;
}