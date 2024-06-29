// Do NOT add any other includes
#include "search.h"

LLNode::LLNode() {
    next = NULL ;
}

LLNode::LLNode(int bk_cd, int pg, int para, int sen_no, string sen) {
    sentence = sen ;
    book_code = bk_cd ;
    paragraph = para ;
    page = pg ;
    sentence_no = sen_no ;
    next = NULL ;
}

LLNode::~LLNode() {
    if(next)
        delete next ;
}

SearchEngine::SearchEngine() {
    headData = NULL ;
}

SearchEngine::~SearchEngine() {
    if(headData)
        delete headData ;
}

void SearchEngine::toLowerCase(string &s) {
    /*
        input "SubHojIt iS A BoY."
        output "subhojit is a boy."
    */
    for(char &c : s)    if('A' <= c && c <= 'Z')
                            c = c - 'A' +'a' ;
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence) {
    toLowerCase(sentence) ;
    // insert @ head
    LLNode* temp = headData ;
    headData = new LLNode(book_code , page , paragraph , sentence_no , sentence);
    headData->next = temp ;
}


Node* SearchEngine::search(string pattern, int& n_matches) {
    Node* head = NULL ;
    LLNode* curr = headData ;
    while(curr) {
        string sentence = curr->sentence ;

        // 2 denotes the pattern
        // and 1 denotes sentence

        int l1 = sentence.length() ;
        int l2 = pattern.length() ;

        if(l1 < l2) {
            // pattern can not be present
            curr = curr->next ;
            continue ;
        }

        int h1 = 0 , h2 = 0 ;
        for(char &c : pattern)
            h2 += c ;

        int x1 = 0  , x2 = l2-1;

        for(int i = 0 ; i < x2 ; i++)
            h1 += sentence[i] ;

        while(x2 < l1) {
            h1 += sentence[x2] ;
            if(h2 == h1) {
                bool isSame = true ;
                for(int it = x1 ; it <= x2 ; it++) {
                    if(sentence[it] != pattern[it-x1]) {
                        isSame = false ;
                        break ;
                    }
                }
                if(isSame) {
                    n_matches++ ;
                    // insert @ head
                    Node* temp = head ;
                    head = new Node(curr->book_code , curr->page , curr->paragraph , curr->sentence_no , x1) ;
                    head->left = NULL ;
                    if(temp) {
                        head->right = temp ;
                        temp->left = head ;
                    } else {
                        head->right = NULL ;
                    }
                }
            }
            h1 -= sentence[x1] ;
            x1++ ;
            x2++ ;
        }

        curr = curr->next ;
    }
    
    return head;
}