#include "CaixaPCD.h"
#include <iomanip>
#include <fstream>
#include <iostream>
#include <thread>
#include <atomic>

// construtor
CaixaPcd::CaixaPcd(const std::string& nome, const std::string& id, const std::string& cpf,
const std::string& email, const std::string& senha, const double& total_vendido) : Caixa(nome, id, cpf, email, senha, total_vendido) {}

// método para gerar relatório de vendas do caixa
void CaixaPcd::gerarRelatorio() const{
// Remove espaços do nome para usar no nome do arquivo
    std::string nome_sem_espaco = removerEspacos(getNome());
    std::string nomeArquivo = "data/relatorios_caixas/relatorio-" + nome_sem_espaco + ".txt";

    // Verifica se o arquivo já existe para evitar repetição de cabeçalho
    bool arquivoExiste = std::ifstream(nomeArquivo).good();

    // Abre o arquivo para escrita (append mode para manter dados existentes)
    std::ofstream arquivo(nomeArquivo, std::ios::app);
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para gerar relatorio.");
    }

    // Escreve o cabeçalho apenas se o arquivo for novo
    if (!arquivoExiste) {
        arquivo << "==========================================================================\n";
        arquivo << "                         RELATÓRIO DE VENDAS.\n";
        arquivo << "==========================================================================\n";
        arquivo << "Nome: " << getNome() << ".\n";
        arquivo << "CPF: " << getCpf() << ".\n";
        arquivo << "Cargo: Caixa.\n";
        arquivo << "--------------------------------------------------------------------------\n\n";
        
    }

    // Adiciona os dados de vendas
    for (const auto& venda : vendas) {
         arquivo << "==========================================================================\n";
        arquivo << "Cliente: " << venda.cliente << ".\n";
        arquivo << "Data: " << venda.dataHora << ".\n";
        arquivo << "--------------------------------------------------------------------------\n";
        arquivo << "                          PRODUTOS VENDIDOS:\n";
        arquivo << "Quantidade.  Produto.                            Preço Unitário.  Preço Total.\n";

        for (const auto& [produto, detalhes] : venda.resumoCarrinho) {
            int quantidade = detalhes.first;
            double precoUnitario = detalhes.second;
            double precoTotal = quantidade * precoUnitario;

            arquivo << std::setw(10) << std::left << quantidade << ".  "
                    << std::setw(35) << std::left << produto << "."
                    << std::setw(16) << std::fixed << std::setprecision(2) << precoUnitario << "."
                    << std::setw(16) << precoTotal << ".\n";
        }

        arquivo << "--------------------------------------------------------------------------\n";
        arquivo << "Total da venda: " << std::fixed << std::setprecision(2) << venda.valorTotal << " reais\n";
        arquivo << "--------------------------------------------------------------------------\n";
    }

    // Fecha o arquivo
    arquivo.close();
}

// Método que retorna o cargo
std::string CaixaPcd::getCargo() const{
    return "caixapcd";
}

// método para exibir o relatoório na tela
void CaixaPcd::exibirRelatorio() const{
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
    
    // Chama TTS
    falarTexto(nomeArquivo);
    
}

// função para chamar text-to-speech
void CaixaPcd::falarTexto(std::string& caminhoArquivo) const{
    // Abre o arquivo de texto
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para salvar Estoque.");
    }
    // Lê o conteúdo do arquivo linha por linha
    std::string linha;
    std::string textoCompleto;
    while (std::getline(arquivo, linha)) {
        textoCompleto += linha + " ";
    }
    arquivo.close();
    // Comando para o RHVoice
    std::string comando = "echo \"" + textoCompleto + "\" | rhvoice.test";
    std::system(comando.c_str());
}

