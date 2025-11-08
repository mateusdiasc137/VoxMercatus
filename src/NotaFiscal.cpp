#include "NotaFiscal.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <algorithm>

// construtor
NotaFiscal::NotaFiscal(Funcionario* caixa, Cliente& cliente, Carrinho& carrinho) : caixa(caixa), cliente(cliente), carrinho(carrinho){}

//gera a nota fiscal
void NotaFiscal::gerarNotaFiscal(const std::string& data_hora){
    // tira espaço do nome
    std::string nome_sem_espaco = cliente.getNome();
    nome_sem_espaco.erase(remove_if(nome_sem_espaco.begin(), nome_sem_espaco.end(), isspace), nome_sem_espaco.end());
    
    // cria o nome do arquivo com o nome do cliente
    std::string nomeArquivo = "data/nota_fiscal/nota_fiscal-" + nome_sem_espaco + ".txt";

    std::ofstream arquivo(nomeArquivo, std::ios::out); // abre somente para escrita
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para carregar nota fiscal.");
    }

    // Imprime a nota fiscal da compra

    arquivo << "==========================================================================\n";
    arquivo << "                    NOTA FISCAL - SUPERMERCADO CCP\n";
    arquivo << "==========================================================================\n";
    arquivo << "Nome do cliente: " << cliente.getNome() << std::endl;
    arquivo << "CPF: " << cliente.getCpf() << std::endl;
    arquivo << "Data da compra: " << data_hora << std::endl;
    arquivo << "--------------------------------------------------------------------------\n";
    arquivo << "Compra realizada:\n";
    arquivo << "Quantidade  Produto                            Preço Unitário  Preço Total\n";

    // Exibe os produtos agrupados
    double subtotal = 0;  // Subtotal acumulado
    for (const auto& [nome, detalhes] : carrinho.getResumoCarrinho()) {
        int quantidade = detalhes.first;
        double precoUnitario = detalhes.second;
        double precoTotal = quantidade * precoUnitario;

        arquivo << std::setw(10) << std::left << quantidade << "  "
                  << std::setw(35) << std::left << nome
                  << std::setw(16) << std::fixed << std::setprecision(2) << precoUnitario
                  << std::setw(16) << precoTotal << std::endl;

        subtotal += precoTotal;  // Soma ao subtotal
        carrinho.setValorTotal(subtotal); // guarda valor total
    }

    arquivo << "--------------------------------------------------------------------------\n";
    arquivo << "Total pago: R$" << std::fixed << std::setprecision(2) << subtotal << std::endl;
    arquivo << "--------------------------------------------------------------------------\n";

    // fecha o arquivo
    arquivo.close();
}