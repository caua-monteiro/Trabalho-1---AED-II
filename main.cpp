#include "include/DataStructLib.hpp"
#define nl std::cout<<"\n";

int main(){
    BST<int> Btree;
    Btree.insert_Node(50);
    Btree.insert_Node(25);
    Btree.insert_Node(75);
    Btree.insert_Node(100);

    std::vector<int> emOrdem = Btree.inOrder();
    std::vector<int> preOrdem = Btree.preOrder();
    std::vector<int> posOrdem = Btree.postOrder();


    std::cout << "Pre-Ordem: ";
    for(int i=0; i < Btree.numberOfNodes(); i++){
        std::cout << preOrdem[i] << " ";
    }
    nl
    std::cout << "Em-Ordem: ";
    for(int i=0; i < Btree.numberOfNodes(); i++){
        std::cout << emOrdem[i] << " ";
    }
    nl
    std::cout << "Pos-Ordem: ";
    for(int i=0; i < Btree.numberOfNodes(); i++){
        std::cout << posOrdem[i] << " ";
    }
    nl

    std::cout << "Em ordem reverse: ";
    for(int i=Btree.numberOfNodes()-1; i >= 0; i--){
        std::cout << emOrdem[i] << " ";
    }
    nl
}