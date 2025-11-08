#ifndef MENU_H
#define MENU_H

#include "Funcionario.h"
#include "BancoFuncionario.h"
#include "Estoque.h"


class Menu {
public:
    static void exibirMenuGerente(Funcionario* gerente, BancoFuncionario& banco, Estoque& estoque);

    static void exibirMenuCaixa(Funcionario* caixa, Estoque& estoque);
};

#endif