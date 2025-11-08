#include "Gerente.h"
#include "CaixaPCD.h"
#include <iostream>
#include <memory>

// construtor
Gerente::Gerente(const std::string& nome, const std::string& id, const std::string& cpf,
const std::string& email, const std::string& senha) : Funcionario(nome, id, cpf, email, senha) {}

// retorna o cargo
std::string Gerente::getCargo() const {
    return "gerente";
}
