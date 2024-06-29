// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class Dict {
private:
    // You can add attributes/helper functions here

    // source http://compoasso.free.fr/primelistweb/page/prime/liste_online_en.php
    // possible sizes of hash table
    vector<long long> mods = {1009 , 10007 , 100003 , 1000003 , 9999889 , 10009079 , 100000007, 1000000070 , 10000000700} ;
    
    vector<vector<pair<string,int>>> HT ;
    long long sizeHT ;
    long long capHT ;

    // function to calculate power
    long long power(long long a , long long b) ;

    // hash val calculator
    long long hashVal(string &word) ;

    // any case to lower case convertor
    void toLowerCase(string &s) ;

    // check if the char is a separator or not
    bool isSeparator(char &ch) ;

    // insert word in hash table
    void insertInHT(string word) ;

    void resizeHT() ;

    


public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    int get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};