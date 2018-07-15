#ifndef _SIMPLE_HASH_TABLE_H_
#define _SIMPLE_HASH_TABLE_H_

#include <string>
#include <vector>

//Our hash table is super simple
//Key 128 bit
//Value std::string.

using KeyType = int64_t;
struct DllNodeValue {
        virtual bool isEqual(const DllNodeValue* o) const = 0;
        virtual std::string toString() const = 0;
};

struct StringDllNodeValue final : public DllNodeValue {
        bool isEqual(const DllNodeValue* o) const override {
                if (o == nullptr) return false;
                const StringDllNodeValue* value = (const StringDllNodeValue*) o;
                return *_ptr == *(value->_ptr);
        }

        std::string toString() const override {
                return *_ptr;
        }

        StringDllNodeValue(std::string* ptr) : _ptr(ptr) {
        }

        std::string* _ptr;
};

struct HashTableElement final : public DllNodeValue {
        KeyType getKey() const {
                return _key;
        }
        std::string* getValue() const {
                return _value;
        }

        bool isEqual(const DllNodeValue* o) const override {
                const HashTableElement* ptr = (const HashTableElement*)o;
                if (o == nullptr) return false;
                return (_key == ptr->_key);
        }

        std::string toString() const override;

        HashTableElement(KeyType key, std::string* value) : _key(key), _value(value) {
        }

        private:

        KeyType _key;
        std::string* _value;
};

struct Dll;
struct DllNode {
        DllNode(DllNodeValue* val) : _p(nullptr), _n(nullptr), _v(val) {
        }

        DllNodeValue* getValue() const {
                return _v;
        }

        private:

        DllNode* _p;
        DllNode* _n;
        DllNodeValue* _v;

        friend struct Dll;
};

struct Dll {
        Dll() : _head(nullptr), _tail(nullptr), _size(0) {
        }

        bool empty() const {
                return _size == 0;
        }

        void addNode(DllNodeValue* val);
        DllNode* findNode(DllNodeValue* val);
        bool deleteNode(DllNodeValue* val);

        std::string toString() const;

        DllNode* _head;
        DllNode* _tail;
        size_t _size;
};


struct HashTable {

        HashTable() : _hashRange(10), _size(0) {
                init();
        }

        bool addElement(const HashTableElement& element);
        bool deleteElement(KeyType key);
        bool hasElement(KeyType key) const;
        std::pair<bool, HashTableElement> getValue(KeyType key) const;

        double getLoadFactor() const {
                double res = _size;
                res /= _hashRange;
                return res;
        }

        void init() {
                for (size_t i = 0; i < _hashRange; i++) {
                        _dllEntries.push_back(new Dll());
                }
        }

        ~HashTable() {
                for (auto& it: _dllEntries) {
                        delete it;
                }
        }

        std::string toString() const;
        size_t size() const { return _size;}
        bool empty() const { return _size == 0;}

    private:

        int getIndex(KeyType key) const {
                return key % _hashRange;
        }

        DllNode* findDllNode(KeyType key) const;


        size_t _hashRange;
        size_t _size;
        std::vector<Dll*> _dllEntries;
};

#endif //_SIMPLE_HASH_TABLE_H_
