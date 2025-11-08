#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

#include<string>
#include<vector>


class Funcionario{
protected:
    std::string nome;
    std::string id;
    std::string cpf;
    std::string email;
    std::string senha;

public:
    // Construtor
    Funcionario(const std::string& nome, const std::string& id, const std::string& cpf,
    const std::string& email, const std::string& senha);

    // destrutor virtual declarado explicitamente
    virtual ~Funcionario();

    // Métodos getters
    std::string getEmail() const;
    std::string getId() const;
    std::string getCpf() const;
    std::string getSenha() const;
    std::string getNome() const;

    // Método virtual puro para retornar o cargo
    virtual std::string getCargo() const = 0;

    // Método para validar senha de login
    bool validarSenha(const std::string& senhaEntrada) const;

    // Função genérica para registrar saída no console e em um arquivo (usada para integrar TTS para caixaPCD)
    void registrarSaida(const std::string& mensagem, const std::string& nome_arquivo);

};


#endif