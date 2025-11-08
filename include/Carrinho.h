#ifndef CARRINHO_H
#define CARRINHO_H

#include "Cliente.h"
#include "Funcionario.h"
#include "Estoque.h"
#include "Produto.h"
#include <vector>
#include <map>

class Carrinho{
private:
    Cliente& cliente;
    Funcionario* caixa;
    Estoque& estoque;
    std::map<std::string, std::pair<int, double>> resumoCarrinho;  // Map para guardar produtos do carrinho
    double valor_total;     // guarda o valor total do carrinho

public:
    // construtor
    Carrinho(Cliente& cliente, Funcionario* caixa, Estoque& estoque);
    
    // getters
    Cliente& getCliente() const;
    Funcionario* getCaixa() const;
    Estoque& getEstoque() const;
    const std::map<std::string, std::pair<int, double>>& getResumoCarrinho() const;
    double getValorTotal() const;

    // setters
    void setValorTotal(double valor_total);

    // gerencia o carrinho durante a compra
    void gerenciarCarrinho(Estoque& estoque);

    // adiciona produtos ao carrinho
    void adicionarProdutoAoCarrinho(const std::string& nomeProduto, int quantidade, std::vector<std::pair<Lote*, int>>& lotes);

    // remove produtos do carrinho
    void removerProdutoDoCarrinho(const std::string& nomeProduto, int quantidade);

    // finaliza a compra limpando os lotes do estoque  
    void finalizarCompra();

    // cancela a compra
    void cancelarCompra();
};

#endif