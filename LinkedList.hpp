/*
 Program implementujacy liste za pomoca wskaznikow.
 ----------------------------------------------------------------------
 Autor: Michal Kosciolek
 ----------------------------------------------------------------------
*/

#include <iostream>

template <typename T>
struct Node
{
    Node *prev;
    Node *next;
    T data;

    Node(T x)
    {
        prev = nullptr;
        next = nullptr;
        data = x;
    }

    Node()
    {
        prev = nullptr;
        next = nullptr;
    }
};

template <typename T>
class List
{
public:
    /* Konstruktor */
    List()
    {
        size_ = 0;
        guard.next = &guard;
        guard.prev = &guard;
    }

    /* Wstawia element na początek listy */
    void push_front(T x)
    {
        Node<T> *n = new Node<T>(x);
        n->prev = &guard;
        n->next = guard.next;
        guard.next->prev = n;
        guard.next = n;
        size_++;
    }

    /* Usuwa i zwraca element z poczatku listy */
    T pop_front()
    {
        if(!size())
            std::out_of_range("out_of_range");
        T data = guard.next->data;
        Node<T> *tmp;
        tmp = guard.next;
        guard.next = tmp->next;
        tmp->next->prev = &guard;
        size_--;
        delete tmp;
        return data;
    }

    /* Wstawia element na koniec listy */
    void push_back(T x)
    {
        Node<T> *n = new Node<T>(x);
        n->next = &guard;
        n->prev = guard.prev;
        guard.prev->next = n;
        guard.prev = n;
        size_++;
    }

    /* Usuwa i zwraca element z konca listy */
    T pop_back()
    {
        if(!size())
            std::out_of_range("out_of_range");
        T data = guard.prev->data;
        Node<T> *tmp;
        tmp = guard.prev;
        guard.prev = tmp->prev;
        tmp->prev->next = &guard;
        size_--;
        delete tmp;
        return data;
    }

    /* Zwraca rozmiar listy */
    int size()
    {
        return size_;
    }

    /* Sprawdza czy lista jest pusta */
    bool empty()
    {
        return !size();
    }

    /* Czysci liste */
    void clear()
    {
        Node<T> *walk = guard.prev;
        Node<T> *tmp;
        for(int i=0; i<size(); i++)
        {
            tmp = walk;
            walk = walk->prev;
            delete tmp;
        }
        size_ = 0;
    }

    /* Zwraca pozycje pierwszego wystapienia elementu x */
    int find(T x)
    {
        Node<T> *walk = guard.next;
        for(int i=0; i<size(); i++)
        {
            if(walk->data == x)
                return i;
            walk = walk->next;
        }
        return -1;
    }

    /* Zwraca wskaznik do pary o danym kluczu */
    template<class K>
    T* find_pair(const K& k)
    {
        Node<T> *walk = guard.next;
        for(int i=0; i<size(); i++)
        {
            if(walk->data.first == k)
                return &walk->data;
            walk = walk->next;
        }
        return nullptr;
    }

    /* Usuwa z listy element x */
    T erase(int i)
    {
        if(i == 0) return pop_back();
        if(i == size()-1) return pop_front();
        Node<T> *walk = guard.next;
        for(int j=0; j<i; j++)
        {
            walk = walk->next;
        }
        T data = walk->data;
        walk->prev->next = walk->next;
        walk->next->prev = walk->prev;
        size_--;
        delete walk;
        return data;
    }

    /* Wstawia do listy element x przed pozycje i */
    void insert(int i, T x)
    {
        if(i >= size())
        {
            std::out_of_range("Wrong index");
        }
        else if(size() == 1) push_back(x);
        else
        {
            Node<T> *walk = guard.next;
            for(int j=0; j<i; j++)
            {
                walk = walk->next;
            }
            Node<T> *n = new Node<T>(x);
            n->prev = walk->prev;
            walk->prev->next = n;
            walk->prev = n;
            n->next = walk;
            size_++;
        }
    }

    /* Usuwa wystapienia elementu x i zwraca ich ilosc */
    int remove(T x)
    {
        int count = 0;
        while(find(x) != -1)
        {
          erase(x);
          count++;
        }
        return count;
    }

    /* Destruktor */
    ~List()
    {
        clear();
    }

private:
    Node<T> guard;
    int size_{0};
};
