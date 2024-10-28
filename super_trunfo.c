#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<locale.h>


#define FORCA 1
#define SAGACIDADE 2
#define HABILIDADE 3

//"É preciso buscar comer cimento." - Albert Einstein

struct carta
{
    char nome[50], classe;
    int forca, sagacidade, habilidade, trunfo;
    struct carta *prox;
};

typedef struct carta Carta;

void enviaCartaParaVencedor(Carta **jogadorVencedor, Carta **cartaVencedora, Carta **cartaDerrotada);
Carta *pegarNo(Carta *cabeca, int indice);
void trocarNos(Carta *a, Carta *b);
void fisher(Carta *cabeca, int tamanho);
Carta *criarNo(Carta novaCarta);
void inserir(Carta **cabeca, Carta novaCarta);
void dividirCartas(Carta *baralho, Carta **jogador1, Carta **jogador2);
void chamarBaralho(Carta **baralho);
void mostrarStatusJogo(Carta *jogador1, int cartasJogador1, Carta *jogador2, int cartasJogador2, int rodada);
void moverCartaParaFundo(Carta **jogador);
int comparaCarta(Carta **jogador1, Carta **jogador2, int *jogadorAtual);
int verificaQualAtributoDaCartaEhMaior(Carta *carta);
int comparaAtributoDasDuasCartas(Carta *carta1, Carta *carta2, int atributo);
void enviarCartaParaFundo(Carta **jogador);
int contarCartas(Carta *jogador);
Carta *retirarCartaDoBaralhoDoDerrotado(Carta **jogador);
void inserirCartaDoDerrotadoNoBaralhoDoVencedor(Carta **jogador, Carta *novaCarta);
void iniciarRodada(Carta **jogador1, Carta **jogador2);
void mostrarStatusJogo(Carta *jogador1, int cartasJogador1, Carta *jogador2, int cartasJogador2, int rodada);
void exibirCartasRodada(Carta *cartaJogador1, Carta *cartaJogador2, int atributoEscolhido);
void declararVencedor(int cartasJogador1, int cartasJogador2);
int escolheOutroAtributo(int atributoAtual);
int escolheAtributo();



int main()
{
    //criando os ponteiros para o baralho com 16 cartas que serão divididas entre dois jogadores, jogador1 e jogador2.
    Carta *baralho = NULL;
    Carta *jogador1 = NULL, *jogador2 = NULL;
    
    setlocale(LC_ALL, "portuguese");
    srand((unsigned)time(NULL));

    // acho que chamarBaralho não ficou o melhor dos nomes, todo caso, essa função insere os dados em cada carta, antes eu inseria
    // na main, eram 32 linhas poluindo a main à toa, tava feio até para mim.

    chamarBaralho(&baralho);   // Carrega todas as cartas para o baralho
    fisher(baralho, 16);       // Ronald Fisher e Frank Yates são muito brabos. Obrigado pela sua contribuição Ronald Fisher e Frank Yates! 

    // baralho embaralhada, agora precisamos dividi-lás entre os dois jogadores. Boraaaa
    dividirCartas(baralho, &jogador1, &jogador2);

    // Variáveis para controlar o status do jogo
    int rodada = 1;
    int jogadorAtual = 1;  // Definimos qual jogador começa escolhendo o atributo

    // 3. Loop principal para rodadas
    while (contarCartas(jogador1) > 0 && contarCartas(jogador2) > 0)
    {
        // Que altura estamos do jogo?  
        mostrarStatusJogo(jogador1, contarCartas(jogador1), jogador2, contarCartas(jogador2), rodada);

        // pega a carta dos dois jogadores e compara
        comparaCarta(&jogador1, &jogador2, &jogadorAtual);

        // E acaba mais uma rodada
        rodada++;
    }

    // 4. Exibir o resultado final
    if (contarCartas(jogador1) > 0)
    {
        printf("Jogador 1 venceu o jogo!\n");
    }
    else
    {
        printf("Jogador 2 venceu o jogo!\n");
    }

    // 5. Liberação da memória (opcional, mas recomendada)
    while (jogador1 != NULL)
    {
        Carta *temp = jogador1;
        jogador1 = jogador1->prox;
        free(temp);
    }
    while (jogador2 != NULL)
    {
        Carta *temp = jogador2;
        jogador2 = jogador2->prox;
        free(temp);
    }

    free(baralho);

    return 0;
}


// falta fazer:
// uma função que verifique qual dos atributos da carta é maior;
// uma função para sortear quem vai iniciar o jogo;
//

// Função para obter o nó na posição 'indice'
Carta *pegarNo(Carta *cabeca, int indice) // está recebendo as cartas, dai a gente cria um contador, em seguida começa a percorrer essa lista
{
    Carta *atual = cabeca;
    int count = 0;
    while (atual != NULL && count < indice) // enquanto não chegar no final,  e a variavel contadora for menor que o indice recebido,
    {                                       // vai passando de um nó para o outro e incrementando o contador. Birlllll!
        atual = atual->prox;
        count++;
    }
    return atual; // saindo do while, retornamos o nó para a função que chamou.Obrigado pelo seu trabalho função!
    // Seus esforços não serão em vão!
}
// as vozes estão invadindo até mesmo meus códigos! Canalhas! Saiam daqui!


void trocarNos(Carta *a, Carta *b)
{ // troca de casais, versão variáveis.
    Carta aux; 
    aux = *a;
    *a = *b;
    *b = aux;
}

//"É que nem o teorema de pitagoras, ele descobriu o teorema e você só tem que aplicar! A hipotenusa é tanto, o cateto é tanto, quanto é
// o outro cateto? Pega a hipotenusa, eleva o quadrado, cateto ao quadrado, somar, diminuir e chegar no resultado...e VOCE ERRA!
// Vai ser burro na cadeia velho! Caralh#, porque o cara na Grécia, a cinco séculos antes de cristo, DESCOBRIU! E você, 5 mil anos depois,
// não consegue aplicar! Tu tem que comer alfafa cara!"

// algoritmo de Fisher-Yates,
void fisher(Carta *cabeca, int tamanho)
{

    int i = 0, j = 0;
    srand((unsigned)time(NULL));

    for (i = tamanho - 1; i > 0; i--)
    {
        j = rand() % (i + 1);
        Carta *no1 = pegarNo(cabeca, i);
        Carta *no2 = pegarNo(cabeca, j);
        trocarNos(no1, no2);
    }
}

// essa foi a melhor parte do trabalho, queria que o código todo fosse assim, queimei alguns quilos de massa cinzenta
// um equivalente do meu cerebro deve ser um motor de Uno 1.0;

Carta *criarNo(Carta novaCarta)
{
    Carta *novo = (Carta *)malloc(sizeof(Carta));
    *novo = novaCarta;
    novo->prox = NULL;
    return novo;
}

// ta recebendo uma carta e a localização da nossa "coleção de cartas", em seguida, adicionando a carta no topo/final da lista
// Carta** cabeca tá apontando para o inicio da nossa lista encadeada que queremos modificar;
void inserir(Carta **cabeca, Carta novaCarta){

    Carta *novo = (Carta *)malloc(sizeof(Carta)); // primeiro, alocamos memória para colocar nossa carta
    *novo = novaCarta;
    novo->prox = NULL;

    if (*cabeca == NULL)
    {                   // verifica se a lista está vazia, quando está vazia, o cabeça tem que apontar para NULL, então se estiver vazia
        *cabeca = novo; // fazemos o novo nó se tornar o primeiro. Boa campeão!
    }
    else
    { // se ele não estiver vazio, ai a brincadeira começa.

        // precisamos percorrer a lista até encontrar o fim, o ultimo nó
        Carta *atual = *cabeca;
        while (atual->prox != NULL)
        {
            atual = atual->prox; // vai passando de próximo em próximo
        }
        atual->prox = novo; // finalmente, quando ele acaba a peregrinação, a gente faz o atual, isto é,
                            // o ultimo da lista apontar para o novo nó, nossa cartinha está oficialmente em ultimo ou no topo.
                            // o fundo do poço é só uma questão de perspectiva?
    }
}

// Função para dividir as cartas entre dois jogadores
void dividirCartas(Carta *baralho, Carta **jogador1, Carta **jogador2)
{
    Carta *atual = baralho;
    int count = 0;

    while (atual != NULL)
    {
        if (count % 2 == 0)
        {                              // quando o numero for par, vai pro player 1, quando for impar, vai para o player 2, na moral, genial de mais.
            inserir(jogador1, *atual); // uma carta pra um
        }
        else
        {
            inserir(jogador2, *atual); // uma carta pro outro
        }
        atual = atual->prox;
        count++;
    }
}

void chamarBaralho(Carta **baralho){ // essa função existe pra evitar que a main fique muito poluida com a inserção de cartas;
  // fazer o trabalho direito dá trabalho -_-

    int numeroCartas = 0, i = 0;

    // inserimos um vetor com os dados das cartas, eu passei uns 4 dias programando um vetor que eu achava ser uma lista
    // agora é uma história engraçada, mas foi uma tristeza.
    Carta carta[] = {
        // nome - classe - força, sagacidade, habilidade, é o trunfo? SIM - 1, NÂO - 0

        {"Soberano", 'A', 10, 10, 10, 1},
        {"Vin", 'A', 4, 7, 9, 0},
        {"Kelsier", 'A', 5, 8, 3, 0},
        {"Ham", 'B', 3, 7, 5, 0},
        {"Rashek", 'B', 2, 6, 8, 0},
        {"Marsh", 'C', 7, 1, 8, 0},
        {"Elend Venture", 'C', 5, 5, 5, 0},
        {"Sazed", 'A', 5, 8, 8, 0},
        {"Gemmel", 'C', 5, 9, 6, 0},
        {"Spook", 'B', 5, 10, 7, 0},
        {"Alendi", 'B', 6, 7, 5, 0},
        {"Dockson", 'B', 7, 9, 6, 0},
        {"Clubs", 'C', 9, 5, 8, 0},
        {"Mare", 'B', 5, 6, 5, 0},
        {"Claudinei", 'A', 5,6, 6, 0},
        {"Fontoura", 'B', 5, 5, 8, 0}};

    numeroCartas = 16;

    for (i = 0; i < numeroCartas; i++)
    {
        inserir(baralho, carta[i]);
    }
}

// Função para mover a carta do topo para o final da lista de um jogador
void moverCartaParaFundo(Carta **jogador){
    if (*jogador == NULL || (*jogador)->prox == NULL)
        return;

    Carta *topo = *jogador;
    *jogador = topo->prox; // Novo topo da lista

    // Encontra o final da lista
    Carta *atual = *jogador;
    while (atual->prox != NULL)
    {
        atual = atual->prox;
    }
    atual->prox = topo; // Insere a carta anterior no final
    topo->prox = NULL;  // Marca o novo final
}

int comparaCarta(Carta **jogador1, Carta **jogador2, int *jogadorAtual){
    if (*jogador1 == NULL || *jogador2 == NULL)
    {
        printf("Pelo menos uma das listas está vazia\n");
        exit(1);
    }

    int atributo = verificaQualAtributoDaCartaEhMaior((*jogadorAtual == 1) ? *jogador1 : *jogador2);

    while (1)
    {
        int resultado = 0;

        switch (atributo)
        {
        case FORCA: // Comparação pela força
            resultado = (*jogador1)->forca - (*jogador2)->forca;
            break;

        case SAGACIDADE: // Comparação pela sagacidade
            resultado = (*jogador1)->sagacidade - (*jogador2)->sagacidade;
            break;

        case HABILIDADE: // Comparação pela habilidade
            resultado = (*jogador1)->habilidade - (*jogador2)->habilidade;
            break;
        }

        if (resultado > 0)
        {
            // Jogador 1 vence
            moverCartaParaFundo(jogador1);
            moverCartaParaFundo(jogador1);
            *jogadorAtual = 1; // Jogador 1 escolhe na próxima rodada
            return 1; // Indica vitória do jogador 1
        }
        else if (resultado < 0)
        {
            // Jogador 2 vence
            moverCartaParaFundo(jogador2);
            moverCartaParaFundo(jogador2);
            *jogadorAtual = 2; // Jogador 2 escolhe na próxima rodada
            return -1; // Indica vitória do jogador 2
        }
        else
        {
            // Empate: alterna para o próximo atributo
            atributo = (atributo % 3) + 1; // Cicla entre 1, 2 e 3
        }
    }
}


int verificaQualAtributoDaCartaEhMaior(Carta *carta){

    if (carta == NULL)
    {
        printf("Deu erro:\n ---------ENTRADA INVALIDA---------\n");
        exit(1); // saida de erro. Então é quase inutil, porque não haverão erros. (...haverão muitos erros)
    }

    if (carta->forca >= carta->sagacidade && carta->forca >= carta->habilidade) return FORCA;
    if (carta->sagacidade >= carta->forca && carta->sagacidade >= carta->habilidade) return SAGACIDADE;
    return HABILIDADE;
}



int comparaAtributoDasDuasCartas(Carta *carta1, Carta *carta2, int atributo){
    switch (atributo)
    {
    case 1:
        return carta1->forca - carta2->forca;
    case 2:
        return carta1->sagacidade - carta2->sagacidade;
    case 3:
        return carta1->habilidade - carta2->habilidade;
    default:
        return 0;
    }
}


// jogador venceu a rodada? Então chama essa função e pega as duas cartas e joga no fundo da lista.
void enviarCartaParaFundo(Carta **jogador){
    if (*jogador == NULL || (*jogador)->prox == NULL)
        return;

    Carta *topo = *jogador;
    *jogador = topo->prox; // Avança a cabeça da lista

    Carta *atual = *jogador;
    while (atual->prox != NULL)
    { // Percorre até o final da lista
        atual = atual->prox;
    }
    atual->prox = topo; // Coloca a carta do topo no final
    topo->prox = NULL;  // Define o antigo topo como último
}

// Função para contar as cartas de um jogador
int contarCartas(Carta *jogador){
    int contador = 0;
    while (jogador != NULL)
    {
        contador++;
        jogador = jogador->prox;
    }
    return contador;
}

// Função para retirar a carta do topo de uma lista e retornar o ponteiro para ela
Carta *retirarCartaDoBaralhoDoDerrotado(Carta **jogador){
    if (*jogador == NULL)
        return NULL;

    Carta *topo = *jogador;
    *jogador = topo->prox;
    topo->prox = NULL;
    return topo;
}

// Função para inserir uma carta ao final da lista
void inserirCartaDoDerrotadoNoBaralhoDoVencedor(Carta **jogador, Carta *novaCarta){
    if (*jogador == NULL)
    {
        *jogador = novaCarta;
    }
    else
    {
        Carta *atual = *jogador;
        while (atual->prox != NULL)
        {
            atual = atual->prox;
        }
        atual->prox = novaCarta;
    }
}

// a partir daqui é o espaço reservado para colocar as funções que manipulam as rodadas e "interagem" com o usuário//

//// Função para iniciar uma rodada de jogo
//void iniciarRodada(Carta *carta1, Carta *carta2) {
//    if (carta1 == NULL || carta2 == NULL) 
//    {
//        printf("Erro: Cartas inválidas.\n");
//        return;
//    }
//
//    int atributoEscolhido = escolheAtributo(); // Inicia com um atributo escolhido
//    int resultado;
//
//    while (1) // Loop infinito, mas saímos se houver um vencedor
//    {
//        resultado = comparaCarta(&carta1, &carta2, &atributoEscolhido);
//
//
////        if (resultado > 0) // jogador1 ganha
////        {
////             printf("Jogador 1 venceu a rodada!\n");
////
////            // Armazena a carta derrotada (topo do baralho do jogador 2)
////            Carta *cartaDerrotada = *jogador2;
////            *jogador2 = (*jogador2)->prox;  // Remove a carta derrotada do baralho do jogador 2
////            cartaDerrotada->prox = NULL;    // Limpa o ponteiro para evitar referências indesejadas
////
////            // Insere a carta derrotada no final do baralho do jogador 1
////            inserirCartaDoDerrotadoNoBaralhoDoVencedor(jogador1, cartaDerrotada);
////            break;
////        }
////        else if (resultado < 0) // jogador2 ganha
////        {
////            printf("Jogador 2 venceu a rodada!\n");
////            enviaCartaParaVencedor(&carta2, &carta1); // Envia carta1 para o baralho do jogador2
////            break;
////        }
////        else // Empate: escolha um novo atributo e continue o loop
////        {
////            printf("Empate! Escolhendo outro atributo para nova comparação.\n");
////            atributoEscolhido = escolheOutroAtributo(atributoEscolhido); // Escolhe novo atributo
////        }
//    if (resultado > 0) // jogador1 ganha
//        {
//            printf("Jogador 1 venceu a rodada!\n");
//
//            // Armazena a carta derrotada (topo do baralho do jogador 2)
//            Carta *cartaDerrotada = *carta2;
//            *carta2 = (*carta2)->prox;  // Remove a carta derrotada do baralho do jogador 2
//            cartaDerrotada->prox = NULL;    // Limpa o ponteiro para evitar referências indesejadas
//
//            // Insere a carta derrotada no final do baralho do jogador 1
//            inserirCartaDoDerrotadoNoBaralhoDoVencedor(carta1, cartaDerrotada);
//            break;
//        }
//        else if (resultado < 0) // jogador2 ganha
//        {
//            printf("Jogador 2 venceu a rodada!\n");
//
//            // Armazena a carta derrotada (topo do baralho do jogador 1)
//            Carta *cartaDerrotada = *carta1;
//            *carta1 = (*carta1)->prox;  // Remove a carta derrotada do baralho do jogador 1
//            cartaDerrotada->prox = NULL;    // Limpa o ponteiro para evitar referências indesejadas
//
//            // Insere a carta derrotada no final do baralho do jogador 2
//            inserirCartaDoDerrotadoNoBaralhoDoVencedor(carta2, cartaDerrotada);
//            break;
//        }
//        else // Empate: escolha um novo atributo e continue o loop
//        {
//            printf("Empate! Escolhendo outro atributo para nova comparação.\n");
//            atributoEscolhido = escolheOutroAtributo(atributoEscolhido); // Escolhe novo atributo
//        }
//	
//	}
//}

// Função para iniciar uma rodada de jogo
void iniciarRodada(Carta **jogador1, Carta **jogador2) {
    if (*jogador1 == NULL || *jogador2 == NULL) 
    {
        printf("Erro: Cartas inválidas.\n");
        return;
    }

    int atributoEscolhido = verificaQualAtributoDaCartaEhMaior(*jogador1); // Inicia com um atributo escolhido
    int resultado;

    while (1) // Loop infinito, mas saímos se houver um vencedor
    {
        resultado = comparaCarta(jogador1, jogador2, &atributoEscolhido);

        if (resultado > 0) // jogador1 ganha
        {
            printf("Jogador 1 venceu a rodada!\n");

            // Armazena a carta derrotada (topo do baralho do jogador 2)
            Carta *cartaDerrotada = *jogador2;
            *jogador2 = (*jogador2)->prox;  // Remove a carta derrotada do baralho do jogador 2
            cartaDerrotada->prox = NULL;    // Limpa o ponteiro para evitar referências indesejadas

            // Insere a carta derrotada no final do baralho do jogador 1
            inserirCartaDoDerrotadoNoBaralhoDoVencedor(jogador1, cartaDerrotada);
            break;
        }
        else if (resultado < 0) // jogador2 ganha
        {
            printf("Jogador 2 venceu a rodada!\n");

            // Armazena a carta derrotada (topo do baralho do jogador 1)
            Carta *cartaDerrotada = *jogador1;
            *jogador1 = (*jogador1)->prox;  // Remove a carta derrotada do baralho do jogador 1
            cartaDerrotada->prox = NULL;    // Limpa o ponteiro para evitar referências indesejadas

            // Insere a carta derrotada no final do baralho do jogador 2
            inserirCartaDoDerrotadoNoBaralhoDoVencedor(jogador2, cartaDerrotada);
            break;
        }
        else // Empate: escolha um novo atributo e continue o loop
        {
            printf("Empate! Escolhendo outro atributo para nova comparação.\n");
            atributoEscolhido = escolheOutroAtributo(atributoEscolhido); // Escolhe novo atributo
        }
    }
}





void mostrarStatusJogo(Carta *jogador1, int cartasJogador1, Carta *jogador2, int cartasJogador2, int rodada){
    printf("---- Rodada %d ----\\n", rodada);
    printf("Jogador 1: %d cartas\\n", cartasJogador1);
    printf("Jogador 2: %d cartas\\n\\n", cartasJogador2);
}

void exibirCartasRodada(Carta *cartaJogador1, Carta *cartaJogador2, int atributoEscolhido){
    printf("Jogador 1 escolheu o atributo %d para comparação.\\n", atributoEscolhido);

    printf("Carta do Jogador 1: %s (Força: %d, Sagacidade: %d, Habilidade: %d)\\n",
           cartaJogador1->nome, cartaJogador1->forca, cartaJogador1->sagacidade, cartaJogador1->habilidade);

    printf("Carta do Jogador 2: %s (Força: %d, Sagacidade: %d, Habilidade: %d)\\n\\n",
           cartaJogador2->nome, cartaJogador2->forca, cartaJogador2->sagacidade, cartaJogador2->habilidade);
           
   	printf("Atributo em comparação: ");
    switch (atributoEscolhido){
        case 
			FORCA: printf("Força\n"); 
		break;
		
        case SAGACIDADE: 
			printf("Sagacidade\n"); 
			break;
			printf("Habilidade\n"); 
			break;
    }    
           
}

void declararVencedor(int cartasJogador1, int cartasJogador2){
    if (cartasJogador1 >= 16)
    {
        printf("\\n*** O JOGADOR 1 É O VENCEDOR COM %d CARTAS! ***\\n", cartasJogador1);
    }
    else if (cartasJogador2 >= 16)
    {
        printf("\\n*** O JOGADOR 2 É O VENCEDOR COM %d CARTAS! ***\\n", cartasJogador2);
    }
}

int escolheOutroAtributo(int atributoAtual) {
    return (atributoAtual % 3) + 1;
}

//void enviaCartaParaVencedor(Carta **jogadorVencedor, Carta **cartaVencedora, Carta **cartaDerrotada) {
//    // Passo 1: Guardar a carta derrotada em uma variável temporária e removê-la da lista do jogador derrotado
//    Carta *cartaMovida = *cartaDerrotada;
//    *cartaDerrotada = (*cartaDerrotada)->prox;
//
//    // Passo 2: Fazer a carta movida apontar para NULL, já que ela irá para o final da lista do vencedor
//    cartaMovida->prox = NULL;
//
//    // Passo 3: Encontrar o último nó da lista do jogador vencedor para adicionar a carta movida
//    Carta *atual = *jogadorVencedor;
//    while (atual->prox != NULL) {
//        atual = atual->prox;
//    }
//
//    // Passo 4: Adicionar a carta derrotada ao final da lista do vencedor
//    atual->prox = cartaMovida;
//}


//------------------------------------------------------------------------------------------------

// função é responsável por verificar qual atributo da carta é maior, retorna 1 - se força for maior,
// 2 - se sagacidade for maior, 3 - se habilidade for maior
// se todos forem iguais... terei de ver o que vou fazer

// a ideia é usar um switch case depois para comparar as cartas

// define como constante pra ficar mais claro no código, antes eu tinha colocado 1,2,3 meio solto no código, ficava meio estranho.

