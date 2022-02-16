#ifndef RECOGNITION_H
#define RECOGNITION_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cstring>

class Recognition
{    
public:
    Recognition() = default;
    ~Recognition() = default;
    Recognition( const Recognition& ) = delete;

    bool init( const std::string& folder, const std::string& operationFile, const std::string& questionsFile, const std::string& subjectListFile, const std::string& objectTypeListFile );
    std::string recognize( const std::string& input );

private: // def and variables
    using string_vector = std::vector<std::string>;
    using string_vector_citer = std::vector<std::string>::const_iterator;

    string_vector operationList;
    string_vector subjectList;
    string_vector objectTypeList;
    std::unordered_map<std::string, string_vector_citer> questions; // question, operationIter
    std::multimap<string_vector_citer, string_vector_citer> subjectLinkMap; // operationIter, subjectIter
    std::multimap<string_vector_citer, string_vector_citer> objectTypeListLinkMap; // subjectIter, objectTypeListIter
    std::unordered_map<std::string, string_vector> objectsLinkMap; // objectTypeListIter, object
 
 private: // functions
    std::vector<std::string> strsplit( const std::string& input, const std::string& delimiter );
    std::vector<std::string> prepareInputFileToLines( const std::string& fileName );
    bool find( const std::string& input, const std::string& pattern );
    bool isFileExist( const std::string& filename ) const;

private: // google test friend classes
    friend class TestFixture_FindCorrection_Test;
    friend class TestFixture_IsFileExist_Test;
    friend class TestFixture_SplitLinesToVector_Test;
};

#endif // RECOGNITION_H
