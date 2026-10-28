# avl_llrb
Repositório para solução de árvores binárias AVL e LLRB

Para execução do programa, há 3 métodos:
1- Utilize o executável já fornecido;
2- Pegar o código dentro de main.c, colar ele dentro de um compilador on-line (ex: GDB online) e executar;
3- Dentro da pasta que main.c estiver, abra o terminal, e caso sua máquina já tenha o compilador de C instalado, use a linha: gcc main.c -o (nome); O executável será criado na mesma pasta. 

De dependência, a única coisa necessária deve ser o compilador C.

Para testar cargas diferentes nos macros, você pode editar o valor bem no início do código chamado "SIZEMACRO" e compilar o programa mais uma vez.

---------------------------
Sobre a autoria do código, por ter sido permitido, grande parte foi feita a partir do ChatGPT. O restante, que se caracteriza pelos macros, seus interiores e a adição deles ao switch foram feitos por quem vos escreve, Davi A. Trindade.

---------
O programa possui 5 principais funcionalidades, que incluem:
-Adicionar elementos a árvore;
-Remover elementos da árvore;
-Buscar elementos na árvore;
-Exibir elementos em ordem crescente;
-Exibição gráfica no terminal dos elementos da árvore.

Essas 5 funcionalidades também são usadas para implementação dos macros, que executam várias operações de adição, busca e remoção em seguida, depois relatando o tempo tomado - excelente para testar o desempenho de cada árvore para tais processos.
---------

Sobre o funcionamento, o usuário primeiramente precisa escolher o tipo de árvore que deseja: AVL ou LLRB. Após isso, diferentes métodos são utilizados para as mesmas operações, dependendo da árvore.
Por exemplo, inserirAVL() é usado para adicionar elementos a árvore AVL, enquanto inserirLLRB() é usado para a árvore LLRB.
Outro exemplo, a AVL usa balancearAVL() para se organizar, enquanto LLRB usa corrigirLLRB().
Esses métodos utilizam de verificações internas para decidir qual o caso e que rotação/ação deve ser tomada.

Alguns outros códigos como emOrdem() e buscar() funcionam para ambos.
