#include "recognition.h"
 
std::vector<std::string> Recognition::split( const std::string& input, const std::string& delimiter )
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

auto Recognition::prepareInputLines( const std::string& fileName )
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
    bool bret = false;

    // std::string str = input;
    // std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); }); // to change lower case. this method discarded in design.

    auto p = strstr( input.c_str(), pattern.c_str() );
    if( p )
        bret = true;

    return bret;
}

bool Recognition::isFileExist( const std::string& filename ) const
{
    bool bret = false;
    std::ifstream file( filename.c_str());

    if ( file) {
        bret = true;
    }
    return bret;
}

bool Recognition::init( const std::string& folder, const std::string& operationFile, const std::string& questionsFile, const std::string& subjectListFile, const std::string& objectTypeListFile )
{
    if( folder.empty() || operationFile.empty() || questionsFile.empty() || subjectListFile.empty() || objectTypeListFile.empty() )
    {
        return false;
    }

    bool bret = isFileExist( folder + operationFile );
    if( !bret )
    {
        std::cerr << "error : operation file couldn't file at : " << folder + operationFile << "\n";
        return false;
    }
    
    bret = isFileExist( folder + questionsFile );
    if( !bret )
    {
        std::cerr << "error : questions file couldn't file at : " << folder + questionsFile << "\n";
        return false;
    }
    
    bret = isFileExist( folder + subjectListFile );
    if( !bret )
    {
        std::cerr << "error : subjects file couldn't file at : " << folder + subjectListFile << "\n";
        return false;
    }

    bret = isFileExist( folder + objectTypeListFile );
    if( !bret )
    {
        std::cerr << "error : objects file couldn't file at : " << folder + objectTypeListFile << "\n";
        return false;
    }
    

    operationList = prepareInputLines( folder + operationFile );
    for( auto const& item : prepareInputLines( folder + questionsFile ) )
    {
        auto strArr = split( item, "=" );
        if( strArr.empty() || strArr.size() < 2 )
            continue;
        
        auto opIter = std::find( operationList.begin(), operationList.end(), strArr[1] );
        if( opIter == operationList.end() )
            continue;

        questions.insert( { strArr[0], opIter } );
    }

    auto objectLines = prepareInputLines( folder + objectTypeListFile );
    objectTypeList.reserve(objectLines.size());

    for( auto const& line : objectLines )
    {
        auto lineArr = split( line, "=" ); // subject=operationList=subsubject
        if( lineArr.empty() || lineArr.size() < 2 ) continue;
        if( lineArr[1].empty()) continue;

        auto objects = split( lineArr[1], "," );
        objectsLinkMap.insert( {lineArr[0], objects} );
    }

    auto subjectLines = prepareInputLines( folder + subjectListFile );
    subjectList.reserve( subjectLines.size() );
    for( auto const& line : subjectLines )
    {
        auto lineArr = split( line, "=" ); // subject=operationList=subsubject
        if( lineArr.empty() || lineArr.size() < 2 ) continue;
        if( lineArr[1].empty()) continue;

        std::vector<std::string>::iterator operationIter;
        if( lineArr[1].find(",") != std::string::npos )
        {
            auto operationArr = split( lineArr[1], "," );
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
                auto subsubjectArr = split( subsubject, "," );
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
            str_vec_citer_t operationIter = question.second;
            
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

                    str_vec_citer_t subjectIter = rangeIter->second;
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
