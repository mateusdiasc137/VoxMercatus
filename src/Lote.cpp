#include "Lote.h"
#include <iomanip>
#include <sstream>
#include <iostream>


// Construtor que recebe codigo, uma data no formato "dd/mm/aaaa" e o tamanho do lote.
Lote::Lote(std::string nome, std::string codigo, const std::string& dataValidadeStr, int tamanho) : nome(nome), codigo(codigo), dataValidadeStr(dataValidadeStr), tamanho(tamanho) {
    std::istringstream ss(dataValidadeStr);
    std::tm tm = {};
    ss >> std::get_time(&tm, "%d/%m/%Y");
    if (ss.fail()) {
        throw std::invalid_argument("Formato de data invalido. Use o formato 'dd/mm/aaaa'.");
    }
    this->dataValidade = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    // inicializa o vetor de produtos com tamanho
    produtos.reserve(static_cast<size_t>(tamanho));
}

// Métodos getters
std::string Lote::getNome() const { return nome; }
std::string Lote::getCodigo() const{ return codigo; }
size_t Lote::getTamanho() const{ return produtos.size(); }
std::string Lote::getValidade() const{ return dataValidadeStr; }

 // Métodos setters
void Lote::setProdutoPreco(double novoPreco){   // configura o preço dos produtos do lote
    for(Produto& produto : produtos){
        produto.setPreco(novoPreco);
    }
}

// retorna o valor unitario de um produto do lote
double Lote::getProdutosPreco() const{
    return produtos[0].getPreco();
}

// retorna um produto do lote pelo índice
const Produto& Lote::pesquisarProduto(int indice) const{
    if(static_cast<size_t>(indice) >= produtos.size()){
        throw std::out_of_range("Indice fora do limite do vetor de produtos.");
    }
    return produtos.at(indice);
}

// Método para adicionar um produto ao lote
void Lote::adicionarProduto(const Produto& produto) {
    produtos.push_back(produto); // adiciona ao lote    
}

// Adiciona uma quantidade de produtos ao lote (sobrecarga)
void Lote::adicionarProduto(const Produto& produto, int quantidade) {
    for (int i = 0; i < quantidade; ++i) {
        produtos.push_back(produto); // Adiciona várias cópias do produto
    }
}

// Método otimizado para adicionar apenas a quantidade de produtos
void Lote::adicionarProduto(int quantidade) {
    if (produtos.empty()) {
        throw std::runtime_error("Lote vazio. Não é possível adicionar quantidade sem referência a Produto.");
    }

    // Utiliza o último produto como referência para replicação
    const Produto& referencia = produtos.back();
    for (int i = 0; i < quantidade; ++i) {
        produtos.push_back(referencia);
    }
}

// Método para remover uma quantidade de produtos do lote
void Lote::removerProdutos(int quantidade){
    if (quantidade < 0) {
        throw std::invalid_argument("A quantidade de produtos a ser removida não pode ser negativa.");
    }
    if(quantidade == 0){
        return;     // retorna sem fazer nada
    }
    if(static_cast<size_t>(quantidade) <= produtos.size()){
        produtos.erase(produtos.begin(), produtos.begin() + quantidade); // deleta a quantidade de produtos a partir do primeiro elemento
        std::cout << "Removendo " << quantidade << " produtos do lote. Produtos disponiveis: " << produtos.size() << std::endl;
    } else {
        throw std::invalid_argument("Nao é possivel remover esta quantidade de produtos do lote.");
    }
}

// cria os produtos que serão adicionados ao lote
void Lote::preencherLote(const std::string& nome, const int& id, double preco, const std::string& descricao){
    Produto novoproduto(nome, id, preco, descricao);
    adicionarProduto(novoproduto);

}

// Método para verificar se o produto está vencido
bool Lote::isExpired() const {
    auto agora = std::chrono::system_clock::now();
    return agora > dataValidade;
}