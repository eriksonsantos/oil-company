#include <string>
using namespace std;
struct Node
{
    string   data;
    Node* prox;
};

struct linked_list
{
    Node* getBegin();
    void    showData();
    bool    EndInsert(string data);
    bool    BeginInsert(string data);
    bool    PosInsert(string data, int pos);
    bool    EndRemove();
    bool    BeginRemove();
    bool    PosRemove(int pos);
    int     getSize();
    string  getValue(int pos);

    linked_list();
    ~linked_list();

private:
    Node* begin;
    int   size;
};

