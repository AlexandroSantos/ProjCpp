# ProjCpp
Projeto final de EEL7323-08235 - Controlador de ativação do ar condicionado

Diretórios

- Docs -> Contém os relatórios pedidos
- ArControl_Kicad - > Contém o esquematico feito em Kicad 5
- LinuxProj -> Contém o código fonte do programa para Linux (disto Mint 20.1)
- ProjArCond -> Contém o código fonte do programa para o Hardware (Raspberry Pi Pico)

No cabeçalho, dos arquivos fonte principais, estão relatadas as formas de compilar e as suas dependências.

# Para o Hardware:
__Requisitos:__ Arquivos de instalação e tutorial para instalação do SDK
  			   C/C++, do Raspberry PI pico, estão neste pdf (página 4):
           
  [getting started with pico](https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf)
  
  Acessado em 22/04/2021.				
  
  OBS.:
 
  1) Utilizado a distribuição Linux Mint 20.1 Cinnamon para o desenvimento.
  
  2) No arquivo *ProjCpp/ProjArCond/CMakeLists.txt* a linha contendo *set(PICO_SDK_PATH "/home/alex/pico/pico-sdk")* deverá ser alterada apontando para o diretório de instalação do SDK C/C++.
  		
  3) No arquivo *ProjCpp/ProjArCond/build/Makefile* todas as linhas	contendo */home/alex/ProjCpp/ProjArCond* deverão ser modificadas a fim de apontar o caminho correto do código fonte.
  
  __Compilação:__ Dentro do diretório *ProjCpp/ProjArCond/build* deverá ser
  			   executado o comando:
           
  			   # make

# Para o software linux

 __Requisitos:__ 
   - Biblioteca ncurses;
   - O hardware deve estar conectado em */dev/ttyUSB0* ou */dev/ttyUSB1*
  
 __Compilar:__ g++ -o ProgAr ProgAr.cpp -lncurses
 
  __Executar:__ ./ProgAr
  
  
  # Relatório de testes
  No diretório __ProcTestes__ existe um arquivo pdf contento os testes realizados.
