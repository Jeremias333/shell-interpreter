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
int commands_size = 0; //Tamanho da lista de comandos
char *last_line; //Última linha lida
char *str_shell_type = seq_str; //String que indica o tipo de shell

// Function PROTOTYPES
void print_err(char *err);
int exists_file_path(char *path);
int process_address(char *file_name);
int shell_loop();
int process_commands(char *command);
void split_commands(char *line);
void exists_exit_command(char *command);

int main(int argc, char **argv) {
    printf("Dev area operating...\n");
    char *address_path = argv[1];

    //Verifica existencia de parametro passando path do arquivo
    if (exists_file_path(address_path) == 1){
        //Verifica existencia do arquivo
        //Verifica existencia de conteúdo no arquivo
        //Verifica extensão do arquivo
        int proceed = process_address(address_path);
        if (proceed == 0){
            shell_mode = 1;
            printf("Processando arquivo...\n");

            //inicializar o shell por leitura do arquivo
        }else{
            //irá encerrar a aplicação com mensagem de erro
            exit(1);
        }
    }

    //inicializar shell por linha de comando
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
    char line[7];

    if (arq_address == NULL){
        char *ERR = DEFAULT_ERR_MSG"Ao abrir o arquivo. Está vazio ou não existe";
        fclose(arq_address);
        print_err(ERR);
        return 1;
    }

    return 0;
}

int shell_loop(){
    char line[MAX_LINE];
    while(should_run){
        //shell interativo
        if (shell_mode == 0){
            printf("%s", str_shell_type);
            fgets(line, MAX_LINE, stdin);

            if(strcmp(line, "exit\n") == 0){
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
            printf("Deve executar comandos lidos do arquivo\n");
        }
    }
}

int process_commands(char *line){
    split_commands(line);
    for (int i = 0; i < commands_size; i++){
        exists_exit_command(commands[i]);
        system(commands[i]);
    }
}

void split_commands(char *line){
    //pegando o primeiro comando
    char *token = strtok(line, ";");
    int count = 0;
    while(token != NULL){
        commands[count] = token;
        token = strtok(NULL, ";");
        count++;
    }
    commands_size = count;
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