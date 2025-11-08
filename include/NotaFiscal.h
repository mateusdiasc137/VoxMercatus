#ifndef NOTA_FISCAL_H
#define NOTA_FISCAL_H

#include "Funcionario.h"
#include "Cliente.h"
#include "Carrinho.h"

class NotaFiscal{
private:
    Funcionario* caixa;
    Cliente& cliente;
    Carrinho& carrinho;

public:
    // construtor
    NotaFiscal(Funcionario* caixa, Cliente& cliente, Carrinho& carrinho);

    //gera a nota fiscal
    void gerarNotaFiscal(const std::string& data_hora);
};

#endif