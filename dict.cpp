// Do NOT add any other includes
#include "dict.h"

/*                        Theory :
______________________________________________________________

comparision of all common approaches :

Time Analysis :

    L = length of word ~ 7
    n = no. of words <= 10^7

1. AVL tree Node:

    comparision of words => O(L) => @max 7 operations

    -> best case insertion => 1st insertion
       1 operation needed
    -> worst case insertion
       travel complete height
       ~ 23 operations
       @ each ht do comparition
       ~ (say worst case) so 7 operations
       => 23*7 ~ 160 operations
       now while moving up we do roation
       and ht updation that in worst case cost 5 operations
       at each node
       so net needed 160 + 5*23 ~ 250 operations

       now for inserting 10^7 words
       we may consider mean case that each word needs 180 operations
       as there are more words at end
       so 180*10^7 operations ~ 18 sec

2. Trie:

    each insertion needs O(L) complexity
    but remember when we create a new node we create a 26 length array
    that takes 26 more operations. But not always we need to do this.
    Thats the beauty of trie. so say at an avg for a word of length
    7 we create 4 new nodes each time ~ 100 extra operations needed
    for each word

    now for inserting 10^7 words
    100*10^7 operations ~ 10 sec


3. Hash Table :
    (using chaining)
    hash val calculation ~ 7 operations
    
    say initialy we make HT(hash table) of size 1000
    needs 1000 opearations
    and say when HT's 60% space is occupied, HT size = 10*prev_size
    
    -> delete prev HT => 1000 operations
    -> new HT => 10000 operations
    -> new HV(hash val) -> assuming 1.3 words for each original hash val
       ~800 opearations
    -> so 11800 = 118*1e2 for resizing from 1e3->1e4
    
    1e4->1e5 => 118*1e3
    1e5->1e6 => 118*1e4
    1e6->1e7 => 118*1e5

    now for inserting 10^7 words
    it needs 9*10^7(hash val cal and each insertion needs 2 operations) + 1.3*10^7(resizing) ~ 1 sec

--------------------------------------------------------------

Memory Analysis :

1. AVL tree Node:
    it stores word(string) and word count int and
    left and right children address
    => (let word be of 7-8 letters) then one node 
       occupies ~ 50 bytes
    for 10^7 words => ~ 500MB space
    
2. Trie:
    one node stores an array of 26 length and its 1st block's address
    and a count int
    so each node 26 + 8 + 4 = 38 bytes
    one word ~ 7-8 letters ~ 285 bytes (6 times avl)
    for 10^7 words, we will re-use each node
    modeling shows ~ 200MB space

3. Hash Table :
    this will store 10^7 words as string as it is
    so 10^7 words => ~250MB space

Hash table sounds better overall


*/

/*                    Utility functions :
______________________________________________________________
*/


long long Dict::power(long long a , long long b) {
    // 31^12 excede limit of long long
    b %= 11 ;
    long long ans = 1 ;
    while(b > 0) {
        if(b & 1 == 1)  // b is odd
            ans *= a ;
        a *= a ;
        b >>= 1 ;
    }
    return ans ;
}


long long Dict::hashVal(string &word) {

    /*
        hash val of "abc"
        = 1*31^0 + 2*31^1 + 3*31^2
        = 1 + 62 + 2883 = 2946 % 1009 = 928
    */

    long long h = 0 ;
    long long l = word.length() ;

    for(long long i = 0 ; i < l ; i++) {
        if( word[i] <= 'z' && word[i] >= 'a')
            h = (h + ((word[i] - 96)*power(31 , i))) % capHT ;
        else
            h = (h + ((word[i])*power(31 , i))) % capHT ;
    }

    return h ;
}



void Dict::toLowerCase(string &s) {
    /*
        input "SubHojIt iS A BoY."
        output "subhojit is a boy."
    */
    for(char &c : s)    if('A' <= c && c <= 'Z')
                            c = c - 'A' +'a' ;
}


/*
        . , - : ! " ' ( ) ? — [ ] “ ” ‘ ’ ˙ ; @
*/
bool seperator[127] ={false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, false, false, false, false, true, true, true, false, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

void Dict::resizeHT() {
    
    long long m ;
    for(m = 0 ; m < mods.size() ; m++)
        if(mods[m] == capHT) break ;

    long long newCap = (long long)mods[m + 1] ;
    vector<vector<pair<string,int>>> newHT(newCap , vector<pair<string,int>>(0)) ;

    // travel old HT and rehash elements put them in new table
    for(long long i  = 0 ; i < capHT ; i++) {
        for(pair<string,int> p : HT[i]) {
            long long h = 0 ;
            long long l = p.first.length() ;

            for(long long k = 0 ; k < l ; k++) {
                if( p.first[k] <= 'z' && p.first[k] >= 'a')
                    h = (h + ((p.first[k] - 96)*power(31 , k))) % newCap ;
                else
                    h = (h + ((p.first[k])*power(31 , k))) % newCap ;
            }
            
            newHT[h].push_back(p) ;
        }
    }
    capHT = newCap ; HT = newHT ;
}


void Dict::insertInHT(string word) {
    
    // check for resize
    if(((capHT * 3) / 5) < sizeHT && capHT < mods.back()) {
        resizeHT() ;
    }

    // insert the word
    long long h = hashVal(word) ;
    long long l = HT[h].size() ;
    long long i = 0 ;
    for( ; i < l ; i++) {
        if(HT[h][i].first == word) {
            HT[h][i].second++ ;
            break;
        }
    }
    if(i == l)
        {HT[h].push_back({word,1}) ; sizeHT++ ;}

}

/*
                        Main functions:
______________________________________________________________
*/

Dict::Dict() {
    // Implement your function here
    sizeHT = 0 ;
    capHT = mods[0] ;
    HT = vector<vector<pair<string,int>>>(capHT , vector<pair<string,int>>(0)) ;
}

Dict::~Dict(){
    // Implement your function here

    // nothing to delete specially as everything allocated on stack
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here
    toLowerCase(sentence) ;
    int l = sentence.length() ;
    int x1 = 0 , x2 = 0 ;

    while(x2 < l) {
        if(seperator[sentence[x2]]) {
            string word ;
            for(int i = x1 ; i < x2 ; i++) {
                word += sentence[i] ;
            }
            if(x1 < x2) {
                insertInHT(word) ;
            }
            x1 = x2 + 1 ;
        } else if (x2 == l-1) {
            string word ;
            for(int i = x1 ; i <= x2 ; i++) {
                word += sentence[i] ;
            }
            if(x1 <= x2) {
                // toLowerCase(word) ;
                insertInHT(word) ;
            }
        }
        x2++ ;
    }
}

int Dict::get_word_count(string word){
    // Implement your function here
    toLowerCase(word) ;
    long long h = hashVal(word) ;

    for(pair<string,int> &p : HT[h])
        if(p.first == word) return p.second ;

    return 0;
}

void Dict::dump_dictionary(string filename){
    // Implement your function here  
    ofstream out(filename , ios::app) ;

    for(vector<pair<string,int>> &v : HT) {
        for(pair<string,int> &p : v)
            out << p.first << ", " << p.second << endl ;
    }

    out.close();
}

/*
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
                        END OF CODE
                    Yadvendra | Subhojit
                  2022EE31764 | 2022mt61976
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
*/