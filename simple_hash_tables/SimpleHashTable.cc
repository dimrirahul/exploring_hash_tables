#include "SimpleHashTable.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <random>

using namespace std;

void Dll::addNode(DllNodeValue* ptr) {
        DllNode* node = new DllNode(ptr);
        node->_p = _tail;
        if (_tail == nullptr) {
                _head =  node;
        } else {
                _tail->_n = node;
        }
        _tail = node;
        _size++;
}


DllNode* Dll::findNode(DllNodeValue* value) {
        DllNode* curr = _head;

        auto match = [&curr, &value]() {
                return (curr->_v)->isEqual(value);
        };
        while (curr &&  !match()) {
                curr = curr->_n;
        }
        return curr;
}

bool Dll::deleteNode(DllNodeValue* value) {
        DllNode* node = findNode(value);
        if (node == nullptr) return false;

        DllNode* prev = node->_p;
        DllNode* next = node->_n;

        if (prev != nullptr) {
                prev->_n = node->_n;
        } else {
                //node is head;
                _head = node->_n;
        }

        if (next != nullptr) {
                next->_p = node->_p;
        } else {
                //node is tail.
                _tail = node->_p;
        }

        _size--;

        assert(_size != 0 ||  (_head == nullptr && _tail == nullptr));g

        delete node;
        return true;

}

string Dll::toString() const {
        stringstream ss;
        ss << "[size=" << _size << " elements={";
        DllNode* curr = _head;
        while (curr) {
                ss << curr->_v->toString();
                ss << ",";
                curr = curr->_n;
        }
        ss << "}\n";
        return ss.str();
}

string HashTableElement::toString() const {
        stringstream ss;
        ss << "[Key=" << getKey() << ", value=" << *getValue() << "]";
        return ss.str();
}

bool HashTable::addElement(const HashTableElement& element) {
        if (hasElement(element.getKey())) return false;

        int dllIndex = getIndex(element.getKey());
        _dllEntries[dllIndex]->addNode(new HashTableElement(element));
        _size++;g
        return true;
}

DllNode* HashTable::findDllNode(KeyType key) const {
        int dllIndex = getIndex(key);
        unique_ptr<DllNodeValue> ptr(new HashTableElement(key, nullptr));
        DllNode* dllNode = _dllEntries[dllIndex]->findNode(ptr.get());
        return dllNode;
}

bool HashTable::hasElement(KeyType key) const {
        return findDllNode(key) != nullptr;
}

pair<bool, HashTableElement> HashTable::getValue(KeyType key) const {
        DllNode* dllNode = findDllNode(key);
        pair<bool, HashTableElement> res = make_pair(false, HashTableElement(-1, nullptr));

        if (dllNode == nullptr) {
                return res;
        }
        res.first = true;
        res.second = *((HashTableElement *)(dllNode->getValue()));
        return res;
}

bool HashTable::deleteElement(KeyType key) {
        int dllIndex = getIndex(key);
        DllNode* dllNode = findDllNode(key);
        if (dllNode == nullptr)
                return false;

        bool res = _dllEntries[dllIndex]->deleteNode(dllNode->getValue());
        if (res) {
                _size--;
        }
        return res;
}

string HashTable::toString() const {
        stringstream ss;
        ss << "[loadFactor=" << getLoadFactor() << ", size=" << _size << "[";
        for (auto& it: _dllEntries) {
                if (!it->empty()) {
                        ss << it->toString();
                }
        }
        ss << "]]";
        return ss.str();
}

vector<string> getWords(const string& s) {
        vector<string> res;
        int prevPos = 0;
        int currPos = 0;
        while (true) {
                while (currPos < s.size() && s[currPos] != ' ') {
                        currPos++;
                }
                res.push_back(s.substr(prevPos, currPos-prevPos));
                prevPos = currPos + 1;
                currPos += 1;
                if (prevPos > s.size()) {
                        break;
                }
        }
        return res;
}

vector<StringDllNodeValue*> getStrings() {
        vector<StringDllNodeValue*> res;
        vector<string> words = getWords("to stick to a surface or substance [or] to follow a practice or belief");
        for (auto& word: words) {
                res.push_back(new StringDllNodeValue(new string(word)));
        }
        return res;
}

void checkDllOperations() {
        vector<StringDllNodeValue*> strings = getStrings();
        Dll dll;
        for (auto& it: strings) {
                dll.addNode((DllNodeValue*)it);
                cout << dll.toString();
                cout << "\n";
        }

        while (!dll.empty()) {
                int idx = rand() % strings.size();
                cout << "Trying to delete string=" << *(strings[idx]->_ptr) << "\n";
                if (dll.deleteNode((DllNodeValue*)strings[idx])) {
                        cout << "Deleted=true" << dll.toString();
                } else {
                        cout << "not found in dll" << dll.toString();
                }
        }
        for (int i = 0; i < strings.size(); i++) {
                delete strings[i]->_ptr;
                delete strings[i];
        }
}

vector<HashTableElement> getHashTableElements() {
        vector<HashTableElement> elements;
        string sentence("All I know is that I stood spellbound in his high-ceilinged studio room, with its north-facing windows in front of the heavy mahogany bureau at which Michael said he no longer worked because the room was so cold, even in midsummer; and that, while we talked of the difficulty of heating old houses, a strange feeling came upon me, as if it were not he who had abandoned that place of work but I, as if the spectacles cases, letters and writing materials that had evidently lain untouched for months in the soft north light had once been my spectacle cases, my letters and my writing materials.");
        vector<string> words = getWords(sentence);
        for (int i = 0; i < words.size(); i++) {
                elements.push_back(HashTableElement(words[i].size(), new string(words[i])));
        }
        return elements;

}
void checkHashTableOperations() {
        //Test insertion
        auto hashTableElements = getHashTableElements();
        HashTable hashTable;
        for (auto& it: hashTableElements) {
                cout << "Adding element:" << it.toString() << "\n";
                hashTable.addElement(it);
                cout << hashTable.toString();
                cout << "\n";
        }

}
int main() {
        //checkDllOperations();
        checkHashTableOperations();
        return 0;
}
