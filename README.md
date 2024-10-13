# T1 Software Básico
Esse repositório contém meu trabalho para a disciplina de Software Básico na PUC-RIO. 

"Main"
- Possui os principais módulos da aplicação: main.c, aux.c e aux.h
- Possui o executável da aplicação: exe
- Possui arquivos .dat utilizados como base para conversão: utf8.dat e uft32.dat
- Possui os arquivos .dat convertidos: ret32.dat e ret8.dat
- Possui o módulo reverse, que pega os arquivos UTF-8 e UTF-32 convertidos e os trás à sua forma original
- Possui os arquivos .dat provenientes do reverse: reverse8.dat e reverse32.dat

"Tests"
- Possui módulos que fazem um dump dos arquivos .dat: test8.c, test32.c, auxTestes.c e auxTestes.h
- Possui um executável: exeTeste
- Possui os arquivos .dat a serem comparados: utf8.dat, utf32.dat, ret32.dat, ret8.dat, reverse8.dat e reverse32.dat
