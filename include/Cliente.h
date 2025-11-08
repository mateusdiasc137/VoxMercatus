#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>

class Cliente{
private:
    const std::string nome;
    const std::string cpf;
public:
    // construtor
    Cliente(const std::string nome, const std::string cpf);

    //getters
    const std::string getNome() const;
    const std::string getCpf() const;
};
#endif