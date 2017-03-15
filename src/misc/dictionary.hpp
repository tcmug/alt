#ifndef DICTIONARY_H
#define DICTIONARY_H

namespace alt {

template <class TYPE>
class dictionary {

    private:

        class entry {
            public:

                entry() {
                    ID = 0;
                    numChildren = 0;
                    pObj = NULL;
                    pChild = NULL;
                    pNext = NULL;
                    }

                ~entry() {
                    if (pChild)
                        delete pChild;
                    if (pNext)
                        delete pNext;
                    if (pObj)
                        delete pObj;
                    }

                // creates a new entry after pP, NULL for 1st
                entry *newEntry(entry *pP) {
                    entry *pNew = new entry();
                    ++numChildren;
                    if (pChild) {
                        if (pP) {
                            // after pP
                            pNew->pNext = pP->pNext;
                            pP->pNext = pNew;
                            } else {
                                // add first
                                pNew->pNext = pChild;
                                pChild = pNew;
                                }
                        } else
                            pChild = pNew;
                    return pNew;
                    }


                TYPE exists(const char *pStr) {
                    if (pChild) {
                        entry *pI = pChild;

                        while (pI && pI->ID < *pStr)
                            pI = pI->pNext;

                        if (pI && (pI->ID == *pStr)) {
                            TYPE pObu = pI->exists(pStr+1);
                            if (pObu)
                                return pObu;
                            }
                        }

                    // if at null char and pObj points to somewhere it exists
                    if (!*pStr && pObj)
                        return pObj;

                    return NULL;
                    }



                TYPE scan_string(const char *pStart, const char *pEnd, char *pUser) {
                    if (pChild) {
                        entry *pI = pChild;

                        while (pI && pI->ID < *pEnd)
                            pI = pI->pNext;

                        if (pI && (pI->ID == *pEnd)) {
                            TYPE i = pI->scan_string(pStart, pEnd+1, pUser);
                            if (i)
                                return i;
                            }
                        }

                    // if pObj points to somewhere it exists and
                    // must be tested
                    //if (pObj && pObj->scan_string_test(pStart, pEnd, pUser))
                    //    return pObj;

                    return 0;
                    }

                char ID;
                int numChildren;

                entry *pChild;
                entry *pNext;
                TYPE pObj; // null if leaf
            };

        entry root;

    public:

        bool add(const char *, TYPE);
//      void remove(const char *);
//      void funcFetch(const char *pStr, char *pUsr); // calls TYPE->fetchFunc(pStart, pEnd, pUsr) when match is found

        TYPE exists(const char *pStr);
        TYPE scan_string(const char *pStr, char *pUser);

    };


////////////////////////////////////////////////////////////
// add a word to the dictionary
////////////////////////////////////////////////////////////
template <class TYPE>
bool dictionary <TYPE>::add(const char *pWord, TYPE pObject) {
    entry *pParent = &root;
    entry *pI = NULL;
    entry *pP = NULL;
    const char *pChar = pWord;

    while (*pChar) {

        pI = pParent->pChild;
        pP = NULL;

        // find a suitable slot for the entry
        if (pI) {

            while (pI && (pI->ID < *pChar)) {
                pP = pI;
                pI = pI->pNext;
                }

            if (pI && (pI->ID == *pChar))
                pParent = pI;
                else
                    pParent = pParent->newEntry(pP); // last, or less than pI

            } else
                pParent = pParent->newEntry(pP); // 1st

        pParent->ID = *pChar;
        ++pChar;
        }

    if (pParent->pObj)
        return false;

    pParent->pObj = pObject;
    return true;
    }


/*
////////////////////////////////////////////////////////////
// remove a word from the dictionary
////////////////////////////////////////////////////////////
template <class TYPE>
void dictionary <TYPE>::remove(const char *pStr) {
    }
*/


////////////////////////////////////////////////////////////
// tests if a word exists in the dictionary
////////////////////////////////////////////////////////////
template <class TYPE>
TYPE dictionary <TYPE>::exists(const char *pStr) {
    return root.exists(pStr);
    }


////////////////////////////////////////////////////////////
// tests if a word exists in the dictionary
////////////////////////////////////////////////////////////
template <class TYPE>
TYPE dictionary <TYPE>::scan_string(const char *pStr, char *pUser) {
    return root.scan_string(pStr, pStr, pUser);
    }

}

#endif
