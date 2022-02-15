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
    using str_vec_t = std::vector<std::string>;
    using str_vec_citer_t = std::vector<std::string>::const_iterator;

    str_vec_t operationList;
    str_vec_t subjectList;
    str_vec_t objectTypeList;
    std::map<std::string,str_vec_citer_t> questions; // question, operationIter
    std::multimap<str_vec_citer_t,str_vec_citer_t> subjectLinkMap; // operationIter, subjectIter
    std::multimap<str_vec_citer_t,str_vec_citer_t> objectTypeListLinkMap; // subjectIter, objectTypeListIter
    std::unordered_map<std::string,str_vec_t> objectsLinkMap; // objectTypeListIter, object
 
 private: // functions
    std::vector<std::string> split( const std::string& input, const std::string& delimiter );
    auto prepareInputLines( const std::string& fileName );
    bool find( const std::string& input, const std::string& pattern );
    bool isFileExist( const std::string& filename ) const;
};

#endif // RECOGNITION_H
