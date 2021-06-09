#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

/*
	s[256] = variavel armazenada com caracteres de 0 a 256 da tabela ASCII.
	keyUser[256] = chave de usuario.
	key[256] = chave.
	msg[1000] = mensagem do usuario.
	msgCrip[1000] = mensagem criptografada.
	comp[256] = Variavel para comparar com a chave.
	op = opção do usuário.
	aux = Variavel auxiliar e temporaria.
	i = variavel contador padr?o para o for.
	j = variavel contador padr?o para o for.
	tam_chave = tamanho da chave.
	tam_entrada = tamanho da entrada.
	randNum[256] = um array de caracters aleatorios.
	k = contador para criptografar a mensagem.
	l = contador para mostrar a mensagem na tela e salvar em um arquivo.
	escolha = variavel para escolha do usuário no menu.
	confirma = confirmar se a chave é igual ao digitado no Descriptografar().
	arqmsg = variavel para onde vai a mensagem criptografada.
	arqkey = variavel para onde vai a chave criptografada.
	x = variavel de posição X.
	y = variavel de posição Y.
	maxx = maxima posição do eixo X.
	maxy = maxima posição do eixo Y.
	maxxx = criação das extremidades X.
	maxxy = criação das extremidades Y.
	parede = character das extremidades.
*/
unsigned char s[256], keyUser[256],key[256], msg[1000], msgCrip[1000], comp[256], auxSen[100], randNum[256], op, aux;
unsigned int i, j, tam_chave, tam_entrada;
char escolha, parede=219;
const int maxx=120, maxy=30;
const int maxxx=1000, maxyy=29;
int k , l, confirma, x=0, y=0, px=0, py=0, numcar=0;
FILE* arqmsg;
FILE* arqkey;
FILE* arqkeyCrip;
FILE* arqrandNum;
FILE* arqmsgdes;
//--------------------------------------------------------------------------------------------------------------------
void gotoxy(int x,int y){//Muda a posição do cursor do console
	COORD p={x,y};// define as cordenadas de p como uma matriz com vetores x e y
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),p);// plica a variavel cursor no programa
}
//--------------------------------------------------------------------------------------------------------------------
void HideCursor(){// Oculta cursor do console
	CONSOLE_CURSOR_INFO cursor = {1, FALSE};// define o tamanho e visibilidade
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);// aplica a variavel cursor no programa
}
//--------------------------------------------------------------------------------------------------------------------
void clear(){// Limpa a tela
	HideCursor();// Oculta o cursor do console
	system("color 71");// muda a cor do background para branco(1) e letras em azul(7)
	system("cls");// limpa a tela
	for(x=0;x<maxx;x++){// cria as barras no vetor x
	for(y=0;y<maxy;y++)// cria as barras no vetor y
		if(y==0 || x==0 || y==maxyy || x==maxxx){// se y for 0 ou x for 0 ou y for maxyy ou x for maxxx
		gotoxy(x,y);// executa a função gotoxy
		printf("%c",parede);// escreve o caractere 219 na posição onde esta o ponto do gotoxy
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------
void TrocaVal(){// Função para trocar os valores dentro de S
	aux = s[i]; // Deixar temporariamente nessa variavel para torcar logo após
	s[i] = s[j]; // Conteudo de s[j] passa para s[i], no caso dentro da localização dos mesmos no array
	s[j] = aux; // Conteudo que anteriormente era de s[i] passa para s[j]
}
//--------------------------------------------------------------------------------------------------------------------
void KeyGen(){// Gerador da chave automatica e randomica
	arqkey = fopen("arq//key.txt","w"); // Abertura da chave para escrever
	arqrandNum = fopen("arq//randnum.txt","w"); // Abertura dos numeros aleatorios para escrever
	tam_chave = strlen(keyUser); // Tamanho do array, onde há caracter que o usuário digitou
	for (i = 0; i < tam_chave; i++){ // Contador para alterar todos os valores da chave
		randNum[i] = rand()% 100 + 20; // Caracter aleatorio
		key[i] = (keyUser[i] + randNum[i]) % 256; // Novo valor da chave, mod de 256, para nunca deixar passar de 256
		fprintf(arqkey,"%c", key[i]); // Armazenando caracter no arquivo "key.txt"
		fprintf(arqrandNum,"%c",randNum[i]); // Armazenando caracter aleatorio no arquivo "randnum.txt"
		}
	fclose(arqrandNum); // fclose = fechamento do arquivo
	fclose(arqkey);
}
//--------------------------------------------------------------------------------------------------------------------
void KeyReturn(){// Função inversa do Gerador da chave
	arqrandNum = fopen("arq//randnum.txt","r"); // Abertura de "randnum.txt" para ler
	fgets(randNum, 100, arqrandNum); // Colocando todos os carateres na variavel randNum
	fclose(arqrandNum);//Fechamento do arquivo arqrandNum
	for(i = 0; i < tam_chave; i++){
		comp[i] = (auxSen[i] + randNum[i]) % 256; // Transformando o comp no key, caso for certo
	}
}
//--------------------------------------------------------------------------------------------------------------------
void Ksa(){//Key-Scheduling Algorithm
	for (i = 0; i < 256; i++){// Fazendo que o S seja de 0 a 255
		s[i] = i;
	}
	j = 0;
	for (i = 0; i < 256; i++){ // Forma de embaralhar os numeros
		j = (j + s[i] + key[i%tam_chave])%256; // Tendo um j para ter um local de troca
		TrocaVal(s, i, j); // Chamando a função de troca de valores
	}
	i = 0;
	j = 0;
}
//--------------------------------------------------------------------------------------------------------------------
void Prga(){//Pseudo-random generation algorithm
	for(k = 0; k < tam_entrada ; k++){ //Fazendo a mensagem criptografada e embaralhando
		i = (i + 1) % 256;
		j = (j + s[i])%256;
		TrocaVal(s, i, j);
		msgCrip[k] = (s[(s[i] + s[j])%256]) ^ msg[k]; // Modifica o estado e saida do byte, ^ porta lógica XOR
  }
}
//--------------------------------------------------------------------------------------------------------------------
void Criptografar(){//Criptografia
	clear();// Limpar a tela e deixar o plano de fundo e borda
	gotoxy(1,1);// muda o ponto do cursor
	printf("Digite sua mensagem:(aperte enter para finalizar)");// mostra a mensagem criptografada no console
	gotoxy(1,2);// muda o ponto do cursor
	fgets (msg, 1000, stdin);// usuario digitando na array msg
	tam_entrada = strlen(msg);// determina o tamanho da mensagem
	clear();// Limpar a tela e deixar o plano de fundo e borda
	gotoxy(1,1);// muda o ponto do cursor
	printf("Insira alguma chave:");// mostra a mensagem criptografada no console
	gotoxy(1,2);// muda o ponto do cursor
	fgets (keyUser, 255, stdin);// usuario digitando na array keyUser
	KeyGen();// Gerador da chave automatica e randomica
	Ksa();//Key-Scheduling Algorithm
	Prga();//Pseudo-random generation algorithm
	clear();// Limpar a tela e deixar o plano de fundo e borda
	arqmsg = fopen("arq//msg.txt","w");// abre o arquivo arqmsg para escrita(w)
	gotoxy(1,1);// muda o ponto do cursor
	printf("mensagem criptografada:(Enter para continuar)");// mostra a mensagem criptografada no console
	gotoxy(1,2);// muda o ponto do cursor
	for ( l = 0; l < tam_entrada; l++){// escreve a mensagem criptografada e envia para o arquivo arqmsg
		fprintf(arqmsg,"%c",msgCrip[l]);// envia para o arquivo arqmsg
		printf("%c", msgCrip[l]);// mostra a mensagem criptografada no console
	}
	getch();// pega a informação da primeira tecla que o usuario apertar
	fclose(arqmsg);// fecha o arqmsg
	clear();// Limpar a tela e deixar o plano de fundo e borda
	gotoxy(1,1);// muda o ponto do cursor
	printf("Deseja voltar ao menu principal? S/N");// mostra a mensagem criptografada no console
	gotoxy(1,2);// muda o ponto do cursor
	op = getch();// pega a informação da primeira tecla que o usuario apertar e amarzena na variavel op
	if (op != 's'){// se op for diferente de s ele sai do programa
		clear();// Limpar a tela e deixar o plano de fundo e borda
		gotoxy(1,1);// muda o ponto do cursor
		printf("Fechando o programa...");// mostra a mensagem criptografada no console
		gotoxy(1,2);// muda o ponto do cursor
		system("cls");//limpa a tela
		system("color a");// muda o background para preto(0) e a letra para verde claro(a)
		exit(0);// fim do programa
	}
}
//--------------------------------------------------------------------------------------------------------------------
void Descriptografar(){//Descriptografia
	clear(); // Limpar a tela e deixar o plano de fundo e borda
	gotoxy(1,1); // muda o ponto do cursor
	printf("Insira a chave:");// mostra a mensagem no console
	gotoxy(1,2); // muda o ponto do cursor
	fgets(comp, 100, stdin); // Usuario digitando no array comp
	strcpy (auxSen, comp); // Copia de comp para auxSen
	tam_chave = strlen (comp); // Determinar o tamanho  da chave
	KeyReturn(); // Chamando função KeyReturn
	arqkey = fopen("arq//key.txt","r"); // Abertura do arquivo "key.txt" para leitura
	fgets(key, 100, arqkey); // E armazena-lo na variavel key
	fclose(arqkey);
	arqmsg = fopen("arq//msg.txt","r"); // Abertura do arquivo "msg.txt" para leitura
	fgets(msg, 200, arqmsg); // E armazena-lo na variavel msg
	fclose(arqmsg);
	tam_entrada = strlen(msg); // Definindo o tamnho da mensagem
	confirma = strcmp (key,comp);//Comparação se as strings comp e key sao iguais
	if (confirma == 0){//Se for = 0 quer dizer que sao iguais
		system("cls");// Limpa toda a tela
		printf("Descriptografando...\n\n");// mostra a mensagem no console
    	Ksa();
    	Prga();
    	clear();
  		gotoxy(1,1);// muda o ponto do cursor
		printf("mensagem descriptografada:\n\n");// mostra a mensagem no console
		gotoxy(1,2);// muda o ponto do cursor
  		arqmsgdes = fopen("arq//msgdes.txt","w"); // Abertura do arquivo "msgdes.txt", para escrever
			for ( l = 0; l < tam_entrada; l++){
				printf("%c", msgCrip[l]); // Mostrar mensagem para o usuário
				fprintf(arqmsgdes,"%c",msgCrip[l]); // Armazenar mensagem no arquivo "msgdes.txt"
			}
		fclose(arqmsgdes);// fecha o arquivo arqmsgdes
		getch();
	}else{ // Caso o usuário erre a senha
		system("cls");// limpa a tela
		clear();
		gotoxy(1,1);// muda o ponto do cursor
		printf("Senha incorreta, a senha nao e %s", auxSen); // Mostrar a senha que o usuario digitou, que esta errada
		gotoxy(1,2);// muda o ponto do cursor
		printf("Aperte qualquer tecla para continuar.");// mostra a mensagem criptografada no console
		gotoxy(1,3);// muda o ponto do cursor
		getch();
	}
	clear();
	gotoxy(1,1);// muda o ponto do cursor
	printf("Deseja voltar ao menu principal? S/N");// mostra a mensagem no console
	gotoxy(1,2);// muda o ponto do cursor
	op = getch();
	setbuf(stdin, NULL);
	if (op != 's'){ // Caso o usuário digite s ele volta pro menu, qualquer outro o programa fecha
		clear();
		gotoxy(1,1);// muda o ponto do cursor
		printf("Fechando o programa");
		gotoxy(1,2);// muda o ponto do cursor
		system("cls");
		system("color a");
		exit(0); // Fechamento do programa
	}
}
//--------------------------------------------------------------------------------------------------------------------
void menu(){//Menu inicial
	while (1){// ciclo infinito
		clear();// Limpar a tela e deixar o plano de fundo e borda
		gotoxy(1,1);// muda o ponto do cursor
		printf("Escolha um comando para executar:")// mostra a mensagem no console
		gotoxy(1,2);// muda o ponto do cursor
		printf("1-Criptografar");// mostra a mensagem no console
		gotoxy(1,3);// muda o ponto do cursor
		printf("2-Descriptografar");// mostra a mensagem no console
		gotoxy(1,4);// muda o ponto do cursor
		printf("3-Sair");// mostra a mensagem criptografada no console
		escolha = getch();// pega a informação da primeira tecla que o usuario apertar e amarzena na variavel escolha
		setbuf(stdin, NULL);
		switch (escolha) {
			case '1':
				Criptografar();
				break;
			case '2':
				Descriptografar();
				break;
	   	case '3':
	    		clear();
					gotoxy(1,1);// muda o ponto do cursor
					printf("Fechando o programa...");// mostra a mensagem no console
					gotoxy(1,2);// muda o ponto do cursor
					system("cls");// limpa a tela
					system("color a");// muda o background para preto(0) e a letra para verde claro(a)
					exit(0);// fim do programa
					break;// quebra do loop
			default:
				clear();// Limpar a tela e deixar o plano de fundo e borda
				gotoxy(1,1);// muda o ponto do cursor
				printf("Opcao invalida");// mostra a mensagem no console
				gotoxy(1,2);// muda o ponto do cursor
				getch();// pega a informação da primeira tecla que o usuario apertar
			}
    }
}
//--------------------------------------------------------------------------------------------------------------------
int main() {//Principal
	srand(time(NULL));// Definindo para que o Rand gere numeros realmente aleatorios
	system("title APS criptografia"); // Mudar o titulo no momento de executar
	menu();// Entra no menu inicial
}
