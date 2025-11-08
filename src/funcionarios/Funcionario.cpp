#include "Funcionario.h"
#include "Gerente.h"
#include "Caixa.h"
#include "CaixaPCD.h"
#include <iostream>
#include <fstream>

// Construtor
Funcionario::Funcionario(const std::string& nome, const std::string& id, const std::string& cpf,
const std::string& email, const std::string& senha)
: nome(nome), id(id), cpf(cpf), email(email), senha(senha) {}

// destrutor virtual declarado explicitamente
Funcionario::~Funcionario() = default;

// Getters
std::string Funcionario::getEmail() const { return email; }
std::string Funcionario::getId() const{ return id; }
std::string Funcionario::getCpf() const{ return cpf; }
std::string Funcionario::getSenha() const { return senha; }
std::string Funcionario::getNome() const {  return nome; }

// Método para validar senha de login
bool Funcionario::validarSenha(const std::string& senhaEntrada) const{
    return senha == senhaEntrada;
}

// Função genérica para registrar saída no console e em um arquivo (usada para integrar TTS para caixaPCD)
void Funcionario::registrarSaida(const std::string& mensagem, const std::string& nome_arquivo) {
    // Abre o arquivo no modo escrita
    std::ofstream arquivo(nome_arquivo, std::ios::out);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo: " << nome_arquivo << "\n";
        return;
    }

    // Escreve a mensagem no arquivo
    arquivo << mensagem;

    // Fecha o arquivo
    arquivo.close();

    // Também exibe no console
    std::cout << mensagem;
}
