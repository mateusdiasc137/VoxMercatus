#ifndef VENDA_H
#define VENDA_H

#include "Funcionario.h"
#include "Estoque.h"
#include "Cliente.h"
#include "Carrinho.h"

class Venda{
private:
    // Função para validar e formatar o CPF no formato xxx.xxx.xxx-xx
    std::string filtrarCpf(const std::string& cpf);
public:
    // método para iniciar a venda 
    void iniciarVenda(Funcionario* caixa, Estoque& estoque);

    // finaliza a venda e gera a nota fiscal para o cliente
    void finalizarVenda(Cliente& cliente, Funcionario* caixa, Estoque& estoque, Carrinho& carrinho);

};

#endif