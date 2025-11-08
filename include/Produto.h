#ifndef PRODUTO_H
#define PRODUTO_H

#include <string>



class Produto{
private:
    std::string nome;
    int id;
    
    double preco;
    std::string descricao;
public:
    // Construtor sobrecarregado
    Produto(const std::string& nome, const int& id, double preco, const std::string& descricao);

    // Construtor sobrecarregado
    Produto(const std::string& nome, double preco);

    // Métodos getters
    std::string getNome() const;
    int getId() const;
    double getPreco() const;
    std::string getDescricao() const;

    // Métodos setters
    void setPreco(double novoPreco);

    // Calcula o preço com desconto aplicado
    double getPrecoComDesconto() const;    
};

#endif