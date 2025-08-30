#pragma once
#include <iostream>
#include <vector>
#include <cstddef>

// ========================
// Classe Node (Nó da Lista)
// ========================

template <typename T>
class Node{
private:
    T info;
    Node *link;

public:
    // Retorna o valor armazenado no nó
    T getInfo() const {
        return info;
    }

    // Define um novo valor para o nó
    void setInfo(T info_) {
        info = info_;
    }

    // Retorna o próximo nó da lista
    Node<T>* getLink() const {
        return link;
    }

    // Define o próximo nó da lista
    void setLink(Node<T>* newLink) {
        link = newLink;
    }

    // Construtor: cria um nó com valor info_ e próximo nó link_ (padrão é nullptr)
    Node(T info_, Node *link_ = nullptr) : info(info_), link(link_) {}
};

// =========================
// Classe LinkedList (Lista)
// =========================

template <typename T>
class LinkedList {
private:
    Node<T> *inicio; // Ponteiro para o primeiro nó da lista

public:
    // Retorna o ponteiro para o início da lista
    Node<T>* getHead() const {
        return inicio;
    }

    // Define o ponteiro de início da lista
    void setHead(Node<T>* head) {
        inicio = head;
    }

    // Construtor: cria uma lista vazia
    LinkedList() : inicio(nullptr) {}

    // Destrutor: limpa todos os nós da lista quando o objeto for destruído
    ~LinkedList() {
        Node<T>* p = inicio;
        while (p != nullptr) {
            Node<T>* temp = p;
            p = p->getLink();
            delete temp; // Libera a memória do nó
        }
    }

    // Insere um novo elemento no início da lista
    void insertStart(T x) {
        Node<T>* n = new Node<T>(x, inicio); // Novo nó aponta para o antigo início
        inicio = n; // Atualiza início para o novo nó
    }

    // Remove e retorna o elemento do início da lista
    T removeStart() {
        if (inicio == nullptr) throw std::runtime_error("Lista vazia");

        T info = inicio->getInfo(); // Salva o valor
        Node<T>* temp = inicio;
        inicio = inicio->getLink(); // Avança o início
        delete temp; // Libera o nó antigo
        return info;
    }

    // Insere um novo elemento no final da lista
    void insertEnd(T x) {
        Node<T>* n = new Node<T>(x, nullptr); // Novo nó com próximo nulo
        if (inicio == nullptr) {
            inicio = n; // Lista estava vazia
        } else {
            Node<T>* p = inicio;
            while (p->getLink() != nullptr) {
                p = p->getLink(); // Vai até o último nó
            }
            p->setLink(n); // Conecta o último nó ao novo nó
        }
    }

    // Imprime todos os elementos da lista
    void imprimeLista() const {
        Node<T>* p = inicio;
        std::cout << "\nItens da lista: ";
        if (p == nullptr) {
            std::cout << "(vazia)";
        }
        while (p != nullptr) {
            std::cout << p->getInfo() << " ";
            p = p->getLink(); // Avança para o próximo
        }
        std::cout << "\n";
    }

    // Retorna true se a lista estiver vazia
    bool isEmpty() const {
        return inicio == nullptr;
    }
};

// =====================
// Classe Queue (Fila)
// =====================

template <typename T>
class Queue {
private:
    LinkedList<T> queue; // Usa uma lista encadeada internamente
public:
    // Adiciona no fim da fila
    void enqueue(T x) {
        queue.insertEnd(x);
    }

    // Remove do início da fila
    T dequeue() {
        return queue.removeStart();
    }

    // Imprime a fila
    void printQueue() const {
        queue.imprimeLista();
    }

    // Verifica se a fila está vazia
    bool isEmpty() const {
        return queue.isEmpty();
    }

    Node<T>* getHead() const{
        return queue.getHead();
    }
};

// =====================
// Classe Stack (Pilha)
// =====================

template <typename T>
class Stack {
private:
    LinkedList<T> stack; // Internamente usa uma lista

public:
    // Insere no topo da pilha (início da lista)
    void push(T x) {
        stack.insertStart(x);
    }

    // Remove do topo da pilha (início da lista)
    T pop() {
        return stack.removeStart();
    }

    // Imprime a pilha
    void printStack() const {
        stack.imprimeLista();
    }

    // Verifica se a pilha está vazia
    bool isEmpty() const {
        return stack.isEmpty();
    }
};

// =================================================
// Classe PrioritizedElement (Elemento com Prioridade)
// =================================================

template <typename T>
class PrioritizedElement {
private:
    T value;                // Valor armazenado
    unsigned int priority;  // Prioridade do elemento (quanto menor, maior a prioridade)
    size_t arrivalOrder;    // Ordem de chegada (para desempatar prioridades iguais)

public:
    // Construtor com valores
    PrioritizedElement(T value_, unsigned int priority_, size_t arrivalOrder_)
        : value(value_), priority(priority_), arrivalOrder(arrivalOrder_) {}

    // Construtor padrão
    PrioritizedElement() : value(), priority(0), arrivalOrder(0) {}

    T getValue() const {
        return value;
    }

    unsigned int getPriority() const {
        return priority;
    }

    size_t getArrivalOrder() const {
        return arrivalOrder;
    }
};

// ========================================
// Classe PriorityQueue (Fila de Prioridade)
// ========================================

template <typename T>
class PriorityQueue {
private:
    LinkedList<PrioritizedElement<T>> list; // Lista ordenada por prioridade
    size_t counter; // Contador de chegada para desempate

    size_t getCounter() const {
        return counter;
    }

public:
    PriorityQueue() : counter(0) {}

    // Insere elemento na posição correta de acordo com prioridade e chegada
    void enqueue(T value, unsigned int priority) {
        PrioritizedElement<T> newElement(value, priority, counter++);

        Node<PrioritizedElement<T>>* anterior = nullptr;
        Node<PrioritizedElement<T>>* atual = list.getHead();

        // Percorre a lista para encontrar a posição correta
        while (atual != nullptr){
            auto infoAtual = atual->getInfo();
            if (infoAtual.getPriority() < newElement.getPriority()){
                anterior = atual;
                atual = atual->getLink();
            }
            else if ((infoAtual.getPriority() == newElement.getPriority()) &&
                     (infoAtual.getArrivalOrder() < newElement.getArrivalOrder())){
                // Mesma prioridade, mas chegou antes
                anterior = atual;
                atual = atual->getLink();
            }
            else {
                // Encontrou posição
                break;
            }
        }

        // Cria novo nó com o elemento
        Node<PrioritizedElement<T>>* newNode = new Node<PrioritizedElement<T>>(newElement);

        if (anterior == nullptr) {
            // Inserção no início
            newNode->setLink(list.getHead());
            list.setHead(newNode);
        } else {
            // Inserção no meio/final
            newNode->setLink(anterior->getLink());
            anterior->setLink(newNode);
        }
    }

    // Remove o elemento com maior prioridade (está no início)
    T dequeue() {
        return list.removeStart().getValue();
    }

    // Verifica se a fila está vazia
    bool isEmpty() const{
        return list.isEmpty();
    }

    // Retorna um std::vector com todos os elementos
    std::vector<PrioritizedElement<T>> getAllElements() const{
        std::vector<PrioritizedElement<T>> elementos;
        Node<PrioritizedElement<T>>* atual = list.getHead();
        while(atual != nullptr){
            elementos.push_back(atual->getInfo());
            atual = atual->getLink();
        }
        return elementos;
    }
};

// Parte menos importante, boa para curiosos.
// Servem para podermos utilizar o operador << para essas classes

// ===================================================
// Sobrecarga do operador << para PrioritizedElement
// ===================================================

template <typename T>
std::ostream& operator<<(std::ostream& os, const PrioritizedElement<T>& elem) {
    os << elem.getValue(); // Mostra apenas o valor do elemento
    return os;
}

// ===================================================
// Sobrecarga do operador << para LinkedList
// ===================================================

template <typename T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list) {
    Node<T>* current = list.getHead();
    os << "Itens da lista: ";
    while (current != nullptr) {
        os << current->getInfo() << " ";
        current = current->getLink();
    }
    return os;
}

// ===============================
// Classe BST (Árvore de Busca)
// ===============================

template <typename T>
class BST {
public:
    // Nó exposto para visualização (sem dependências gráficas)
    struct Node {
        T key;
        Node* left;
        Node* right;
        Node* parent;
        explicit Node(const T& k, Node* p = nullptr)
            : key(k), left(nullptr), right(nullptr), parent(p) {}
    };

    // Entrada de layout "neutro": posição normalizada 0..1
    struct LayoutEntry {
        const Node* node;
        double x;
        double y;
        int depth;
    };

private:
    Node* root_;
    std::size_t sz_;

public:
    BST() : root_(nullptr), sz_(0) {}
    ~BST() { clear(); }

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    Node* root() { return root_; }
    const Node* root() const { return root_; }

    std::size_t size() const { return sz_; }
    bool empty() const { return sz_ == 0; }
    void clear() { clearRec(root_); root_ = nullptr; sz_ = 0; }

    // Operações básicas
    bool contains(const T& k) const { return findNode(k) != nullptr; }

    void insert(const T& k) {
        if (!root_) { root_ = new Node(k); ++sz_; return; }
        Node* cur = root_;
        Node* parent = nullptr;
        while (cur) {
            parent = cur;
            if (k < cur->key) cur = cur->left;
            else if (cur->key < k) cur = cur->right;
            else return;
        }
        Node* n = new Node(k, parent);
        if (k < parent->key) parent->left = n; else parent->right = n;
        ++sz_;
    }

    bool remove(const T& k) {
        Node* n = findNode(k);
        if (!n) return false;
        eraseNode(n);
        --sz_;
        return true;
    }

    std::vector<T> preOrder() const { std::vector<T> out; out.reserve(sz_); preOrderRec(root_, out); return out; }
    std::vector<T> inOrder()  const { std::vector<T> out; out.reserve(sz_); inOrderRec(root_, out);  return out; }
    std::vector<T> postOrder()const { std::vector<T> out; out.reserve(sz_); postOrderRec(root_, out);return out; }

    std::vector<LayoutEntry> layoutNormalized() const {
        std::vector<LayoutEntry> out;
        const int n = static_cast<int>(sz_);
        if (n == 0) return out;

        int idx = 0;
        int maxDepth = 0;
        layoutInorder(root_, 0, idx, n, out, maxDepth);

        // Normaliza y pela profundidade máxima
        const double denom = (maxDepth == 0) ? 1.0 : static_cast<double>(maxDepth);
        for (auto& e : out) {
            e.y = (maxDepth == 0) ? 0.0 : (static_cast<double>(e.depth) / denom);
        }
        return out;
    }

    void insert_Node(const T& k) { insert(k); }
    bool delete_Node(const T& k) { return remove(k); }
    int numberOfNodes() const { return static_cast<int>(size()); }

private:
    // Utilidades internas
    void clearRec(Node* n) {
        if (!n) return;
        clearRec(n->left);
        clearRec(n->right);
        delete n;
    }

    Node* findNode(const T& k) const {
        Node* cur = root_;
        while (cur) {
            if (k < cur->key) cur = cur->left;
            else if (cur->key < k) cur = cur->right;
            else return cur;
        }
        return nullptr;
    }

    static Node* minimum(Node* n) {
        while (n && n->left) n = n->left;
        return n;
    }

    void transplant(Node* u, Node* v) {
        if (!u->parent) root_ = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if (v) v->parent = u->parent;
    }

    void eraseNode(Node* z) {
        if (!z->left) transplant(z, z->right);
        else if (!z->right) transplant(z, z->left);
        else {
            Node* y = minimum(z->right);
            if (y->parent != z) {
                transplant(y, y->right);
                y->right = z->right;
                if (y->right) y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            if (y->left) y->left->parent = y;
        }
        delete z;
    }

    void preOrderRec(Node* n, std::vector<T>& out) const {
        if (!n) return;
        out.push_back(n->key);
        preOrderRec(n->left, out);
        preOrderRec(n->right, out);
    }
    void inOrderRec(Node* n, std::vector<T>& out) const {
        if (!n) return;
        inOrderRec(n->left, out);
        out.push_back(n->key);
        inOrderRec(n->right, out);
    }
    void postOrderRec(Node* n, std::vector<T>& out) const {
        if (!n) return;
        postOrderRec(n->left, out);
        postOrderRec(n->right, out);
        out.push_back(n->key);
    }

    void layoutInorder(Node* n, int depth, int& idx, int nTotal,
                       std::vector<LayoutEntry>& out, int& maxDepth) const {
        if (!n) return;
        if (depth > maxDepth) maxDepth = depth;
        layoutInorder(n->left, depth + 1, idx, nTotal, out, maxDepth);

        const double x = (static_cast<double>(idx) + 1.0) / (static_cast<double>(nTotal) + 1.0);
        out.push_back(LayoutEntry{ n, x, 0.0, depth });
        ++idx;

        layoutInorder(n->right, depth + 1, idx, nTotal, out, maxDepth);
    }
};
