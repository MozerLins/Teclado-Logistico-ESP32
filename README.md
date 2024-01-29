# Teclado Terminal de Dados Multi Comunicação Para Módulo de Rastreamento, 100% Touch Screen ( Micro Controlador - Esp32 )
Este repositório contém o código-fonte e a documentação para um teclado terminal de dados projetado para um sistema de rastreamento de veículos com interface touch screen.

## Visão Geral
Este projeto visa fornecer uma interface intuitiva e eficiente para os usuários interagirem com o sistema de rastreamento de veículos. O teclado terminal de dados permite que os usuários insiram informações, como telefones, senhas e contra senhas e outras entradas necessárias para o funcionamento do sistema.

## Resumo
O Teclado Logístico é um dispositivo de entrada e saída de dados com comunicação multidirecional, atuando como uma interface homem-máquina. Sua principal diferenciação em relação aos teclados existentes no mercado é a comunicação Bluetooth, o que permite sua integração com diversos dispositivos de rastreamento automotivo. Isso proporciona aos usuários a liberdade de desenvolver sua própria interface gráfica e criar suas próprias regras de negócio, com foco na segurança da carga e do motorista, utilizando um protocolo próprio. Este projeto foi desenvolvido utilizando o microcontrolador ESP32.

## Recursos Principais
### Micro Controlador Utilizado
O módulo ESP32 foi escolhido devido às suas características superiores, incluindo comunicação WiFi e Bluetooth nativas, maior capacidade de memória flash e dois núcleos de processamento que facilitam a comunicação entre o módulo e o teclado, sem interrupção no programa principal. O ESP32 pode ser encontrado como um módulo ou em placas de desenvolvimento, sendo utilizada a placa de desenvolvimento Wemos neste projeto para simplificar as conexões, facilitar a integração com a tela touch resistiva e tornar o dispositivo mais compacto, aproximando-se do design atual.

![image](https://github.com/MozerLins/TTL-001/assets/94911429/fb0a4bdb-5550-4ba1-a3c0-6bcaeff6140b)

### Ambiente de Desenvolvimento
O ambiente utilizado para o desenvolvimento do projeto foi o Arduino IDE (Integrated Development Environment). Esta escolha foi feita devido à sua familiaridade e suporte para programação em C e C++. Embora o Arduino IDE seja nativamente voltado para placas compatíveis com o Arduino, foi possível fazer o upload do programa para o ESP32 com o auxílio dos núcleos da Espressif.

A utilização do Arduino IDE permitiu um fluxo de desenvolvimento suave e facilitou a programação do Teclado Logístico, aproveitando as vantagens do ESP32 para comunicação WiFi e Bluetooth, bem como outras características específicas do microcontrolador.

### Módulo de Interface
Para a demonstração dos menus e outras informações, foi utilizado o módulo LCD TFT de 3,5 polegadas com toque resistivo. Este módulo foi escolhido devido à sua melhor compatibilidade e fácil instalação com a placa de desenvolvimento Wemos, uma vez que os sockets da placa são semelhantes aos de um Arduino Uno.
A utilização deste módulo de interface proporcionou uma experiência de usuário mais intuitiva, permitindo a exibição de menus e outras informações de forma clara e acessível. Sua integração com a placa de desenvolvimento Wemos simplificou o processo de montagem e tornou o sistema mais compacto e fácil de usar.

![image](https://github.com/MozerLins/TTL-001/assets/94911429/ff063dc5-7700-47e8-b22e-b1a67ec9f217)
![image](https://github.com/MozerLins/TTL-001/assets/94911429/96ed0336-3dcd-47f4-be98-c3781dfbd216)

## Integração com Linhas de Rastreadores VIRLOC-6
A integração deste teclado terminal de dados foi direcionada especificamente para as linhas de rastreadores do tipo VIRLOC-6. Inicialmente, a ideia era integrar via Bluetooth, porém, devido às especificidades do projeto e às características técnicas dos dispositivos, a integração foi focada nesse tipo específico de rastreador.

### Integração com Outras Tecnologias
Encorajamos os usuários a explorar e integrar este teclado terminal de dados com outras tecnologias e dispositivos além dos rastreadores VIRLOC-6. Embora o projeto tenha sido inicialmente direcionado para essa integração específica, ele foi projetado de forma modular e flexível para facilitar a integração com uma variedade de sistemas.
Se você deseja integrar este teclado com sua própria tecnologia ou adaptá-lo para funcionar com outros dispositivos, sinta-se à vontade para fazê-lo! Estamos abertos a colaborações e novas ideias para expandir a funcionalidade deste projeto.

## Clone este repositório em sua máquina local.
Certifique-se de ter as dependências necessárias instaladas.
Execute o arquivo de instalação install.sh.
Siga as instruções no terminal para concluir a instalação.

## Contribuição de Usuários
Se você deseja contribuir com este projeto, você pode criar sua própria branch e enviar suas alterações através de um Pull Request. 
