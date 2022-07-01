## Projeto de sistema operacionais

#### criado por
<p>Israel Schmitt Joaquim</p>
<p>Lissandro Roberto Wilford Arabia Rezende</p>

#### Proposta
<p>Criar um sistema de lavação de carro utilizando threads, semafaros e mutex </p>

#### Executando o programa
<p>Para executar basta compilar o arquivo lavacar.c usando um compilador de C como o gcc, para sistemas operacionais linux é necessario passar algumas flags para o gcc para uso de threads e para nomear o arquivo de saida</p>

<p>Para compilar usando o gcc em sistemas linux basta dar o comando a seguir na mesma pasta do arquivo lavacar.c, sera gerado um arquivo para execução com nome lavacar</P>

```$ gcc -pthread -o lavacar lavacar.c ```

<p>Para executar</p>

```$ ./lavar```

<p>Para executar tudo com um unico comando:</p>

```$ gcc -pthread -o lavacar lavacar.c && ./lavacar```