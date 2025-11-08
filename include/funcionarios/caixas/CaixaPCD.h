#ifndef CAIXA_PCD_H
#define CAIXA_PCD_H

#include "Caixa.h"

class CaixaPcd : public Caixa{
public:
    // construtor
    CaixaPcd(const std::string& nome, const std::string& id, const std::string& cpf,
    const std::string& email, const std::string& senha, const double& total_vendido);

    // método para gerar relatório de vendas do caixa
    virtual void gerarRelatorio() const override;

    // Método que retorna o cargo
    virtual std::string getCargo() const override;

    // método para exibir o relatoório na tela
    virtual void exibirRelatorio() const override;

    // função para chamar text-to-speech
    void falarTexto(std::string& caminhoArquivo) const;
};

#endif