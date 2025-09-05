#include <iostream>
#include <vector>
#include "include\DataStructLib.hpp"

using namespace std;

int main()
{
    BST<int> aux;
    BST<int> arvore = aux.randomIntTree(10);

    vector<vector<int>> matriz = arvore.aplicaBFS();

    for(auto& linha : matriz){
        for(auto& elemento : linha){
            cout << elemento << " ";
        }
        cout << endl;
    }

    return 0;
}
