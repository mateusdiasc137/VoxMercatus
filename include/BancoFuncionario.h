#ifndef BANCO_FUNCIONARIO_H
#define BANCO_FUNCIONARIO_H

#include "Gerente.h"
#include "CaixaPCD.h"
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <iostream>

class BancoFuncionario {
private:

    // Armazena funcionários
    static std::map<std::string, std::unique_ptr<Funcionario>> funcionarios;

    // Função para validar e formatar o CPF no formato xxx.xxx.xxx-xx
    std::string filtrarCpf(const std::string& cpf);

    // Função que converte uma string para minúsculas
    std::string toLowerCase(const std::string& palavra);

public:
    // construtor
    BancoFuncionario();

    // Método para gerenciar os funcionarios(gerente tem permissão)
    void gerenciarFuncionarios(Funcionario* gerente, BancoFuncionario& banco);

    // Adiciona um funcionário ao banco de dados
    void adicionarFuncionario(Funcionario* gerente, std::unique_ptr<Funcionario> novoFuncionario);

    // Remove um funcionário do banco de dados
    void removerFuncionario(Funcionario* gerente, const std::string& id);

    // Retorna um ponteiro para o funcionário, se encontrado usando email
    Funcionario* buscarFuncionarioPorEmail(const std::string& email) const;

    // Retorna um ponteiro para o funcionário, se encontrado usando nome
    Funcionario* buscarFuncionarioPorNome(const std::string& nome) const;

    // Retorna um ponteiro para o funcionário, se encontrado usando id
    Funcionario* buscarFuncionarioPorId(const std::string& id_funcionario) const;

    // verifica se mais de um funcionário tem o mesmo nome
    bool temChara(const std::string& nome);

    // realiza o login do funcionario
    Funcionario* realizarLogin(const std::string& email, const std::string& senha) const;

    // salva os funcionarios em um arquivo txt (simula o banco de dados)
    void salvarFuncionariosNoArquivo(const std::string& nomeArquivo) const;

    // carrega os funcionarios a partir de um arquivo txt (simula o banco de dados)
    void carregarFuncionariosDoArquivo(const std::string& nomeArquivo);
};


#endif