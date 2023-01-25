/*
 Program implementujacy liste z haszowaniem.
 ----------------------------------------------------------------------
 Autor: Michal Kosciolek
 ----------------------------------------------------------------------
*/

#include <iostream>
#include <utility>
#include <string>
#include "LinkedList.hpp"

/* Funkcja haszujaca */
unsigned int hash(const std::string& str){
    char* c = new char[str.length()+1];
    strcpy(c, str.c_str());
    unsigned int h = 0;
    for (int i=0; c[i]; i++)
        h = (h << 3) ^ c[i];
    delete[] c;
    return h%1000;
}

template<class K, class V>
class Dict {
    using Pair = std::pair<K, V>;
public:
    /* Konstruktor */
    Dict(): size_(0), classes_counter(0) {}

    /* Czysci tablice */
    void clear(){
        for(auto & i : array){
            i.clear();
        }
    }

    /* Wstawia do tablicy pare p */
    bool insert(const Pair& p){
        unsigned int k = hash(p.first);
        if(!array[k].find(p))
            return false;
        if(array[k].find_pair(p.first) != nullptr && p.first == array[k].find_pair(p.first)->first){
            Pair tmp = std::make_pair(p.first, array[k].find_pair(p.first)->second);
            int index = array[k].find(tmp);
            array[k].insert(index, p);
            array[k].erase(index+1);
        }
        if(array[k].empty())
            classes_counter++;
        array[k].push_back(p);
        size_++;
        return true;
    }

    /* Zwraca true, jesli istnieje para o danym kluczu */
    bool find(const K& k){
        return array[hash(k)].find_pair(k) != nullptr;
    }

    V& operator[](const K& k){
       // std::cout << "Jestemw[]" << std::endl;
        return array[hash(k)].find_pair(k)->second;
    }

    /* Usuwa pare o kluczu */
    bool erase(const K& k){
        unsigned int key = hash(k);
        if(array[key].find_pair(key) == nullptr)
            return false;
        Pair tmp = std::make_pair(k, array[key].find_pair(k)->second);
        int index = array[key].find(tmp);
        array[key].erase(index);
        if(array[key].empty())
            classes_counter--;
        return true;
    }

    /* Zwraca rozmiar */
    int size(){
        return size_;
    }

    /* Zwraca true, jesli tablica jest pusta */
    bool empty(){
        return !size();
    }

    /* Wypisuje informacje o tablicy */
    void buckets(){
        int max = 0;
        int min = 1000;
        for(auto & i : array){
            if(i.size() > max)
                max = i.size();
            if(i.size() < min)
                min = i.size();
        }
        std::cout << "#" << " " << size() << " " << classes_counter << " " << min << " " << max << std::endl;
    }

    /* Destruktor */
    ~Dict(){
        clear();
    }
private:
    List<Pair> array[1001];
    int size_;
    int classes_counter;
};