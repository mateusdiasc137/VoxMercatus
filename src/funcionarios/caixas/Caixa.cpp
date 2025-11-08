#include "Caixa.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

// construtor
Caixa::Caixa(const std::string& nome, const std::string& id, const std::string& cpf,
const std::string& email, const std::string& senha, const double& total_vendido) :
Funcionario(nome, id, cpf, email, senha), total_vendido(total_vendido){}

// Registra uma venda no histórico
void Caixa::registrarVenda(const std::string& cliente, const std::map<std::string, std::pair<int, double>>& resumoCarrinho, 
double valorTotal, const std::string& dataHora) {
    vendas.push_back({cliente, resumoCarrinho, valorTotal, dataHora});
    gerarRelatorio();   // gera relatório de vendas
}

// getters
double Caixa::getTotalVendido() const{  return total_vendido; }

// setters
void Caixa::setTotalVendido(double total_vendido){  this->total_vendido = total_vendido; }

// Método auxiliar para remover espaços do nome
std::string Caixa::removerEspacos(const std::string& str) const {
    std::string resultado = str;
    resultado.erase(remove_if(resultado.begin(), resultado.end(), isspace), resultado.end());
    return resultado;
}

// método para gerar relatório de vendas do caixa
void Caixa::gerarRelatorio() const {

    // Gera o nome do arquivo de relatório com base no nome do caixa
    std::string nome_sem_espaco = removerEspacos(getNome());
    std::string nomeArquivo = "data/relatorios_caixas/relatorio-" + nome_sem_espaco + ".txt";

    // Verifica se o arquivo já existe para evitar repetição de cabeçalho
    bool arquivoExiste = std::ifstream(nomeArquivo).good();

    // Abre o arquivo para escrita (append mode para manter dados existentes)
    std::ofstream arquivo(nomeArquivo, std::ios::app);
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para gerar relatório.");
    }

    // Escreve o cabeçalho apenas se o arquivo for novo
    if (!arquivoExiste) {
        arquivo << "==========================================================================\n";
        arquivo << "                         RELATÓRIO DE VENDAS\n";
        arquivo << "==========================================================================\n";
        arquivo << "Nome: " << getNome() << std::endl;
        arquivo << "CPF: " << getCpf() << std::endl;
        arquivo << "Cargo: Caixa" << std::endl;
        arquivo << "--------------------------------------------------------------------------\n\n";
        
    }

    // Adiciona os dados de vendas
    for (const auto& venda : vendas) {
        arquivo << "==========================================================================\n";
        arquivo << "Cliente: " << venda.cliente << "\n";
        arquivo << "Data: " << venda.dataHora << "\n";
        arquivo << "--------------------------------------------------------------------------\n";
        arquivo << "                          PRODUTOS VENDIDOS:\n";
        arquivo << "Quantidade  Produto                            Preço Unitário  Preço Total\n";

        for (const auto& [produto, detalhes] : venda.resumoCarrinho) {
            int quantidade = detalhes.first;
            double precoUnitario = detalhes.second;
            double precoTotal = quantidade * precoUnitario;

            arquivo << std::setw(10) << std::left << quantidade << "  "
                    << std::setw(35) << std::left << produto
                    << std::setw(16) << std::fixed << std::setprecision(2) << precoUnitario
                    << std::setw(16) << precoTotal << std::endl;
        }

        arquivo << "--------------------------------------------------------------------------\n";
        arquivo << "Total da venda: R$" << std::fixed << std::setprecision(2) << venda.valorTotal << "\n";
        arquivo << "--------------------------------------------------------------------------\n";
    }

    // Fecha o arquivo
    arquivo.close();
}

// método para exibir o relatoório na tela
void Caixa::exibirRelatorio() const {
    // Gera o nome do arquivo de relatório com base no nome do caixa
    std::string nomeSemEspacos = removerEspacos(getNome());
    std::string nomeArquivo = "data/relatorios_caixas/relatorio-" + nomeSemEspacos + ".txt";

    // Verifica se o arquivo existe
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Erro: o arquivo " + nomeArquivo + " não existe ou não pode ser aberto.");
    }
    arquivo.close(); // Fecha o arquivo, pois só queremos verificar sua existência

    // Abre o arquivo no editor de texto padrão
    #if defined(_WIN32) || defined(_WIN64)
        // Comando para Windows
        std::string comando = "start \"\" \"" + nomeArquivo + "\"";
    #elif defined(__linux__)
        // Comando para Linux
        std::string comando = "xdg-open \"" + nomeArquivo + "\"";
    #elif defined(__APPLE__)
        // Comando para macOS
        std::string comando = "open \"" + nomeArquivo + "\"";
    #else
        throw std::runtime_error("Sistema operacional não suportado.");
    #endif

    int resultado = system(comando.c_str());

    // Verifica se houve erro ao abrir o editor de texto
    if (resultado != 0) {
        throw std::runtime_error("Erro ao abrir o editor de texto padrão.");
    }
}

// Método que retorna o cargo
std::string Caixa::getCargo() const{
    return "caixa";
}

