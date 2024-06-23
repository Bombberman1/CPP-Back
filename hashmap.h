#include <iostream>
#define HASH_COUNT 9973


class HashNode {
    private:
        const char *key;
        const char *value;
        HashNode *next;
        const char* arr_copy(const char *str) {
            char len = 0;
            for(; str[len] != 0; len++);
            char *arr = new char[len + 1];
            for(char i = 0; i <= len; i++) {
                arr[i] = str[i];
            }

            return arr;
        }
        bool arr_equal(const char *arr1, const char *arr2) {
            if(arr1 == nullptr) return false;
            short i = 0;
            for(; arr1[i] != 0 && arr2[i] != 0; i++) {
                if(arr1[i] != arr2[i]) return false;
            }
            
            return (arr1[i] == 0 && arr2[i] == 0);
        }
    public:
        HashNode(const char *key) : key(arr_copy(key)), value(nullptr), next(nullptr) {}
        const char*& get_key() {
            return key;
        }
        const char*& get_value() {
            return value;
        }
        HashNode* get_next() {
            return next;
        }
        void set_next(HashNode *next) {
            this -> next = next;
        }
        HashNode& operator=(const char* val) {
            if(!arr_equal(value, val)) {
                delete[] value;
                value = arr_copy(val);
            }

            return *this;
        }
        ~HashNode() {
            delete[] key;
            delete[] value;
        }
};

class HashMap {
    private:
        HashNode **map;
        HashNode *start;
        int get_hash(const char *arr) {
            const short symbols_count = 256;
            int hash = arr[0] % HASH_COUNT;

            for(unsigned char i = 1; arr[i] != '\0'; i++) {
                hash *= symbols_count;
                hash += arr[i];
                hash %= HASH_COUNT;
            }

            return hash;
        }
    public:
        HashMap() {
            map = new HashNode *[HASH_COUNT]();
            start = nullptr;
        }
        HashNode& operator[](const char *key) {
            int hash = get_hash(key);
            if(map[hash]) {
                return *map[hash];
            }

            HashNode *node = new HashNode(key);
            map[hash] = node;
            if(!start) {
                start = node;
            } else {
                HashNode *p = start;
                while(p -> get_next()) {
                    p = p -> get_next();
                }
                p -> set_next(node);
            }

            return *map[hash];
        }
        bool is_key(const char *key) {
            int hash = get_hash(key);
            
            return map[hash] != nullptr;
        }
        friend std::ostream& operator<<(std::ostream& os, const HashMap& map) {
            HashNode *p = map.start;
            while(p) {
                os << p -> get_key() << ": " << p -> get_value() << '\n';
                p = p -> get_next();
            }
            
            return os;
        }
        ~HashMap() {
            HashNode *p = start;
            while(p) {
                HashNode *prev = p;
                p = p -> get_next();
                delete prev;
            }
            delete[] map;
        }
};
