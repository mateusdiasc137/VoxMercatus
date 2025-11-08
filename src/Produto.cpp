#include "Produto.h"

// Construtor
Produto::Produto (const std::string& nome, const int& id, double preco, const std::string& descricao)
        : nome(nome), id(id), preco(preco), descricao(descricao) {}

// construtor
Produto::Produto (const std::string& nome, double preco) : nome(nome), preco(preco){}

// Métodos getters
std::string Produto::getNome() const{    return nome;}
int Produto::getId() const{    return id;}
double Produto::getPreco() const{    return preco;}
std::string Produto::getDescricao() const{    return descricao;}

// Métodos setters
void Produto::setPreco(double novoPreco){   this->preco = novoPreco;}
