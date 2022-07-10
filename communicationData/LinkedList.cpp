#include <iomanip>
#include <iostream>
#include "LinkedList.h"
using namespace std;

linked_list::linked_list()
{
    begin = nullptr;
    size = 0;
};

linked_list::~linked_list()
{
    for (int i = 0; i < size; i += 1) BeginRemove();
};

Node* linked_list::getBegin() { return begin; }

void linked_list::showData()
{
    int   nc = 0;
    cout << "Total de " << size << " elementos:\n\n";
    Node* p = begin;
    for (int i = 0; i < size; i += 1, p = p->prox)
    {
        cout << "i =" << i << " " << setw(4) << p->data << " " << endl;
    }
    cout << "\n";
}

string linked_list::getValue(int pos) {
    Node* p = begin;
    for (int i = 0; i < pos; i += 1, p = p->prox);

    if(not p->data.empty())
        return(p->data);
    
}

string linked_list::getLastValue() {
    Node* p = begin;

    for (int i = 0; i < size; i += 1, p = p->prox);

    return(p->data);

}

bool linked_list::BeginInsert(string data)
{
    Node* newData = new Node;
    newData->data = data;
    newData->prox = begin;
    begin = newData;
    size += 1;
    return true;
}

bool linked_list::EndInsert(string data)
{
    Node* newData = new Node;
    newData->data = data;
    newData->prox = NULL;
    if (size == 0) return BeginInsert(data);
    Node* p = begin;
    while (p->prox != NULL) { p = p->prox; }
    p->prox = newData;
    size += 1;
    return true;
}

bool linked_list::PosInsert(string data, int pos)
{
    if (pos > size + 1) return false;
    if (pos <= 0) return false;
    if (pos == 1) return BeginInsert(data);
    if (pos == size) return EndInsert(data);
    Node* novo = new Node;
    novo->data = data;

    Node* p = begin;
    for (int cont = 1; cont < pos; cont += 1) p = p->prox;
    novo->prox = p->prox;
    p->prox = novo;
    size += 1;
    return true;
}

bool linked_list::EndRemove()
{
    if (size == 0) return false;
    if (size == 1) { return BeginRemove(); }
    Node* ant = nullptr;
    Node* p = begin;
    while (p->prox != NULL)
    {
        ant = p;
        p = p->prox;
    }
    if (ant != nullptr)
        ant->prox = NULL;
    else
        begin = nullptr;
    delete p;
    size -= 1;
    return true;
}

bool linked_list::BeginRemove()
{
    if (size == 0) return true;
    Node* newDataBegin = begin->prox;
    delete begin;
    begin = newDataBegin;
    size -= 1;
    return true;
}

bool linked_list::PosRemove(int pos)
{
    if (pos > size) return false;
    if (pos < 1) return false;
    if (pos == 1) return BeginRemove();
    int   Cont = 1;
    Node* ant = nullptr;
    Node* p = begin;
    Node* prox = nullptr;
    while (Cont != pos)
    {
        ant = p;
        p = p->prox;
        prox = p->prox;
        Cont++;
    };

    ant->prox = prox;
    delete p;
    size -= 1;
    return true;
}

int linked_list::getSize() { return size; }