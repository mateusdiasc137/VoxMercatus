# Sistema de Gerenciamento de Supermercado

## Descrição do Projeto

Este projeto implementa um **sistema de gerenciamento de supermercado** em C++ que possui funcionalidades voltadas para a gestão de funcionários, estoque, vendas e acessibilidade. O sistema foi projetado para rodar em sistemas **Linux** e conta com suporte a **text-to-speech (TTS)** para auxiliar usuários com deficiência visual, utilizando a ferramenta [**RHVoice**](https://rhvoice.org).

O programa simula as operações diárias de um supermercado, incluindo:

- **Gestão de Estoque:** Adicionar, remover, alterar preços e consultar lotes de produtos.
- **Gestão de Funcionários:** Cadastro, remoção e listagem de funcionários com diferentes cargos (Gerente, Caixa e CaixaPCD).
- **Processamento de Vendas:** Controle de carrinhos de compras, emissão de notas fiscais e geração de relatórios mensais.
- **Acessibilidade:** Utilização de síntese de voz para leitura de textos em áreas críticas do programa, como menus e relatórios, garantindo suporte inclusivo para usuários com deficiência visual.

---

## Funcionalidades Principais

1. **Login e Gerenciamento de Funcionários:**
   - Gerentes podem adicionar, editar e remover funcionários.
   - Controle de permissões baseado no cargo (Gerente ou Caixa).

2. **Gestão de Estoque:**
   - Adição de novos produtos organizados em lotes.
   - Controle de lotes baseado na validade e quantidade.
   - Ajuste dinâmico de preços.

3. **Processamento de Vendas:**
   - Registro de compras no carrinho de clientes.
   - Atualização do estoque ao finalizar a venda.
   - Emissão de relatórios e notas fiscais.

4. **Acessibilidade com TTS:**
   - Uso do **RHVoice** para síntese de voz em menus e relatórios.
   - Auxílio sonoro em partes essenciais do programa.

---

## Requisitos para Instalação

1. **Instalar o Make:**
   - Atualizar os pacotes do sistema:
     ```bash
     sudo apt update
     ```
   - Instalar o Make:
     ```bash
     sudo apt install make
     ```

2. **Instalar o RHVoice:**
   - Instale o RHVoice com:
     ```bash
     sudo snap install rhvoice
     ```
   - Instale a voz em português:
     ```bash
     sudo rhvoice.vm -i Letícia-F123
     ```

3. **Configurar o RHVoice no sistema:**
   - Acesse o diretório do **Speech Dispatcher**:
     ```bash
     cd /usr/lib/speech-dispatcher-modules
     ```
   - Crie o link simbólico para o RHVoice:
     ```bash
     sudo ln -s /snap/rhvoice/current/bin/sd_rhvoice
     ```
   - Reinicie o sistema para aplicar as configurações:
     ```bash
     sudo reboot
     ```
*obs: Algumas distros do linux não possuem o snap instalado por padrão*
- Instale o snap com o comando:
  ```bash
  sudo apt install snapd
     ```
- Caso não consiga instalar o snap dessa forma será nescessário remover um arquivo que impede sua instalação *nosnap.pref*:
  ```bash
  sudo rm /etc/apt/preferences.d/nosnap.pref
     ```
- Ou mover o arquivo para outra pasta:
  ```bash
  sudo mv /etc/apt/preferences.d/nosnap.pref ~/Documents/nosnap.backup
     ```
- Após remover o arquivo da pasta, basta instalar o snap com o primeiro comando.
---

## Como Compilar e Executar

1. Clone o repositório do projeto:
   ```bash
   git clone <URL_DO_REPOSITORIO>
   cd <PASTA_DO_PROJETO>
   ```

2. Compile o projeto com o Make:
   ```bash
   make
   ```

3. Execute o sistema:
   ```bash
   ./bin/supermercado
   ```

---

## Dependências

- **GCC**: Compilador C++.
- **Make**: Ferramenta para compilação.
- **RHVoice**: Síntese de voz (TTS).

---
