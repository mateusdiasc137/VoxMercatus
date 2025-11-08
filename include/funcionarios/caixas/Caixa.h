#ifndef CAIXA_H
#define CAIXA_H

#include "Funcionario.h"
#include <map>

// Estrutura para guardar os detalhes de uma venda
struct DetalheVenda {
    std::string cliente;
    std::map<std::string, std::pair<int, double>> resumoCarrinho; // Produto -> {quantidade, preço unitário}
    double valorTotal;
    std::string dataHora; // Data e hora da venda
};

class Caixa : public Funcionario{
protected:
    std::vector<DetalheVenda> vendas; // Histórico de vendas
    double total_vendido; // guarda o valor total vendido pelo caixa

    // Método auxiliar para remover espaços do nome
    std::string removerEspacos(const std::string& str) const;

public:
    // construtor
    Caixa(const std::string& nome, const std::string& id, const std::string& cpf,
    const std::string& email, const std::string& senha, const double& total_vendido);

    // getters
    double getTotalVendido() const;

    // setters
    void setTotalVendido(double total_vendido);

    // Registra uma venda no histórico
    void registrarVenda(const std::string& cliente, const std::map<std::string, std::pair<int, double>>& resumoCarrinho, 
    double valorTotal, const std::string& dataHora); 

    // método para gerar relatório de vendas do caixa
    virtual void gerarRelatorio() const;

    // Método que retorna o cargo
    virtual std::string getCargo() const override;

    // método para exibir o relatoório na tela
    virtual void exibirRelatorio() const;
};

#endif