#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"

//Error CONSTANTES
#define DEFAULT_ERR_MSG "\nOcorreu um problema: "
#define CLOSE_MSG "PROGRAMA ENCERRADO\n"
#define MAX_LINE 80
#define seq_str "jos seq> "
#define par_str "jos par> "

/*    SHELL TYPE
    - 0: shell seq
    - 1: shell par
*/
int shell_type = 0; 

/*    SHELL MODE
    - 0: shell interative
    - 1: shell batch
*/
int shell_mode = 0;


//Global VARS
char *arguments[MAX_LINE/2 + 1]; //A linha de comandos podem ter no máximo 40 argumentos
int should_run = 1; //Flag de controle para ajudar a execução 
FILE *arq_address; //Endereço do arquivo de entrada total
char *commands[MAX_LINE]; //Lista de comandos
char *lines_commands[MAX_LINE]; //Lista de linhas de comandos
int lines_count = 0;
int commands_size = 0; //Tamanho da lista de comandos
char *last_line; //Última linha lida
char *str_shell_type = seq_str; //String que indica o tipo de shell

// Function PROTOTYPES
void print_err(char *err);
int exists_file_path(char *path);
int process_address(char *file_name);
void shell_loop();
void process_commands(char *command);
void process_file();
int split_commands(char *line, int count);
void exists_exit_command(char *command);
void exists_exit_in_file(char *commands);

int main(int argc, char **argv) {
    printf("Dev area operating...\n");
    char *address_path = argv[1];

    //Verifica existencia de parametro passando path do arquivo
    if (exists_file_path(address_path) == 1){
        // Verifica existencia do arquivo
        // Verifica existencia de conteúdo no arquivo
        // Verifica extensão do arquivo
        int proceed = process_address(address_path);
        if (proceed == 0){
            shell_mode = 1;
            //inicializar shell por arquivo (batch) caso o shell_mode seja 1
            shell_loop();
        }else{
            //irá encerrar a aplicação com mensagem de erro
            exit(1);
        }
    }

    //inicializar shell por linha de comando caso o shell_mode seja 0
    shell_loop();
    return 0;
}

void print_err(char *err) {
    printf("%s\n", err);
    printf(CLOSE_MSG);
    exit(1);
}

int exists_file_path(char *path){
    if (path == NULL){
        return 0;
    }
    return 1;
}

int process_address(char *file_name){
    char *temp[3];
    *temp = &file_name[strlen(file_name)-3];

    if(strcmp(*temp, "txt") != 0){
        char *ERR = DEFAULT_ERR_MSG"O arquivo passado não é um arquivo .txt";
        print_err(ERR);
        return 1;
    }

    arq_address = fopen(file_name, "r");

    if (arq_address == NULL){
        char *ERR = DEFAULT_ERR_MSG"Ao abrir o arquivo. Está vazio ou não existe";
        fclose(arq_address);
        print_err(ERR);
        return 1;
    }

    return 0;
}

void shell_loop(){
    char line[MAX_LINE];
    while(should_run){
        //shell interativo
        if (shell_mode == 0){
            printf("%s", str_shell_type);
            fgets(line, MAX_LINE, stdin);

            if (feof(stdin)){
                //resolve problema de Control+D loop infinito
                should_run = 0;
                printf(CLOSE_MSG);
                exit(0);
            }else if(strcmp(line, "exit\n") == 0){
                should_run = 0;
                printf(CLOSE_MSG);
                exit(0);
            }

            process_commands(line);
            fflush(stdout);
        }

        //shell batch
        if (shell_mode == 1){
            //executar comandos lidos
            process_file();
            // //verificando existencia do comando exit
            // exists_exit_in_file(commands[commands_size-1]);
        }
    }
}

void process_commands(char *line){
    //irá dividir os comandos colocando todos dentro do vetor global commands
    split_commands(line, 0);

    for (int i = 0; i < commands_size; i++){
        exists_exit_command(commands[i]);
        system(commands[i]);
    }
}

void process_file(){
    // printf("Processando arquivo...\n");

    //inicializar o shell por leitura do arquivo
    char line_arq[MAX_LINE];
    // int count = 0;

    //lendo linha a linha do arquivo

    //BUG: Para arquivos a linha abaixo está sobreescrevendo
    //Cada série de comandos.
    // while(fgets(line, MAX_LINE, arq_address) != NULL){
    //     lines_commands[count] = line;
    //     count++;
    // }

    int count = 0;

    while (!feof(arq_address)){
        fgets(line_arq, sizeof(line_arq), arq_address);
        // lines_count = count;
    
        // for(int i = 0; i < lines_count; i++){
        commands_size += split_commands(line_arq, count);
        // }

        printf("Linha processada %d\n", commands_size);
    }

    //irá dividir os comandos colocando todos dentro do vetor global commands
    // split_commands(line);

    // commands_size += lines_count;

    for (int i = 0; i < commands_size; i++){
        // if (commands[i] != NULL){
            printf("valor do command: %s em %d\n", commands[i], i);
        // }
        // exists_exit_command(commands[i]);
        // system(commands[i]);
    }

    exit(0);

}

/*
    BUG: split_commands está sobreescrevendo as posições do array de comandos com
    a linha posterior no arquivo.
*/
int split_commands(char *line, int count){
    //pegando o primeiro comando
    char *token = strtok(line, ";");
    // int count = 0;
    // if(count != 0){
    //     count += 1;
    // }
    while(token != NULL){
        printf("Token da vez: %s para a posição: %d\n", token, count);
        commands[count] = token;
        count++;
        // printf("Valor do command: %s\n", commands[count]);
        token = strtok(NULL, ";");
    }
    // printf("Valor do count: %d", count);
    //itera o valor que foi contado a variavel de controle para tamanho de cada comando

    return count++;
}

void exists_exit_command(char *command){
    if(strcmp(command, "exit\n") == 0){
        should_run = 0;
        printf(CLOSE_MSG);
        exit(0);
    }else if(strcmp(command, " exit\n") == 0){
        should_run = 0;
        printf(CLOSE_MSG);
        exit(0);
    }else if(strcmp(command, "exit") == 0){
        should_run = 0;
        printf(CLOSE_MSG);
        exit(0);
    }else if(strcmp(command, " exit") == 0){
        should_run = 0;
        printf(CLOSE_MSG);
        exit(0);
    }else if(strcmp(command, "exit \n") == 0){
        should_run = 0;
        printf(CLOSE_MSG);
        exit(0);
    }
}

void exists_exit_in_file(char *commands){
    long initialize_exit = strstr(commands, "exit") - commands;
    // printf("valor: %ld\n", initialize_exit);

    if (initialize_exit < 0){
        printf("Não existe exit no arquivo de entrada\n");
        should_run = 0;
        printf(CLOSE_MSG);
        exit(0);
    }
}