#include <iostream>
#include <string>
#include "Vector.hpp"
#include <random>

template <typename T>
struct Node
{
    std::string key_;
    T value_;
    Node* next_ = nullptr;

    Node(const char* _key) : key_(_key) {}
};

template <typename S>
class HashMap
{
    unsigned int capacity_ = 10;

public:
    Node<S>** hashMap_ = nullptr;
    
    HashMap() {
		hashMap_ = new Node<S>*[capacity_];
		
        for (int i = 0; i < capacity_; ++i) {
            hashMap_[i] = nullptr;
        }
    }

	HashMap(const HashMap<S>& _map) {
		capacity_ = _map.capacity_;
		hashMap_ = new Node<S>*[capacity_];

		for (int i = 0; i < capacity_; ++i) {
            hashMap_[i] = nullptr;
        }
		
		for (int i = 0; i < capacity_; ++i) {
			Node<S>* currentNode = _map.hashMap_[i];
			
			while (currentNode != nullptr) {
				unsigned int hash = HashFunction(currentNode->key_.c_str());
				Link(hashMap_[hash], currentNode->key_.c_str()) = currentNode->value_;

				currentNode = currentNode->next_;
			}
		}
	}

	void operator=(const HashMap<S>& _map) {
		capacity_ = _map.capacity_;
		hashMap_ = new Node<S>*[capacity_];

		for (int i = 0; i < capacity_; ++i) {
            hashMap_[i] = nullptr;
        }
		
		for (int i = 0; i < capacity_; ++i) {
			Node<S>* currentNode = _map.hashMap_[i];
			while (currentNode != nullptr) {
				unsigned int hash = HashFunction(currentNode->key_.c_str());
				Link(hashMap_[hash], currentNode->key_.c_str()) = currentNode->value_;

				currentNode = currentNode->next_;
			}
		}
	}
	
    S& operator[](const char* _key) {
        unsigned int hash = HashFunction(_key);

        if (hash > capacity_)
            Rehash(hash);

        return Link(hashMap_[hash], _key);
    }

    bool Contain(const char* _key) {
        unsigned int hash = HashFunction(_key);
        Node<S>* node = hashMap_[hash];

        while(node != nullptr) {
            if (node->key_ == _key)
                return true;
            else
                node = node->next_;
        }

        return false;
    }

    ~HashMap() {
        for (int i = 0; i < capacity_; ++i) {
            Node<S>* node = hashMap_[i];
            while(node != nullptr) {
                Node<S>* nodeTemp = node;
                node = node->next_;
                delete nodeTemp;
            }
        }
        delete [] hashMap_;
		hashMap_ = nullptr;
    }
private:
    S& Link(Node<S>*& _node, const char* _key) {
        if (_node == nullptr) {
            _node = new Node<S>(_key);
            return _node->value_;
        } else {
            if (_node->key_ == _key) {
                return _node->value_;
            }
            
            return Link(_node->next_, _key);
        }
    }
    
    unsigned int HashFunction(const char* _key) {
        unsigned int sum = 0;
        unsigned int counter   = 0;
        while (_key[counter] != '\0') {
            sum += _key[counter];
            ++counter;
        }

        unsigned int reminder = sum % capacity_;
        return reminder;
    }

    void Rehash(unsigned int _hash) {
        unsigned int reminder = _hash % 10;
        capacity_ = _hash + (10 - reminder);
        
        Node<S>** temp = new Node<S>*[capacity_];

        for (int i = 0; i < capacity_; ++i)
            temp[i] = hashMap_[i];

        delete [] hashMap_;
        hashMap_ = nullptr;
        hashMap_ = temp;
    }
};

union Variant
{
    std::string string;
    float integer;
    bool boolean;
    int null;
    GLVM::Core::Vector<Variant> array;
    HashMap<Variant> map;
    Variant() {}
    ~Variant() {}
};
