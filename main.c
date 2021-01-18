#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <unistd.h>
#define max_word 10
#define max_char 100
# define del " "
#include<signal.h>
void remove_endof_line(char line[])
{
    int i=0;
    while(line[i]!='\n')
        i++;
    line[i]='\0'; //get the last char and change it to '\0'
}

void  read_line(char line[])
{
int x=1;
    char *ret= fgets(line,max_char,stdin); //gets the input line
    remove_endof_line(line);
    if(strcmp(line,"exit") ==0 ) //compare it to exit if they are equal the program terminates
        exit(0);

}
int process_line(char* args[],char line[])
{
    int i=0;
    args[i]=strtok(line,del); //put the first token del=" "
    if(args[i]=='\0') // check if it is equal to '\0' so we return 0
    {
        return 0;
    }
    while(args[i]!='\0')
    {
        args[++i]=strtok(NULL,del); // tokenize the rest of the line till '\0'
    }
    return i;
}
int read_parse_line(char*args[],char line[])
{
    read_line(line);
   int x= process_line(args,line);
    return x;
}

int is_background(char *args[]) // check if the process works in foreground or background
{int n=sizeof(args)/sizeof(char*); //get the last string index
if (args[n]!=NULL)
{    if(strcmp(args[n],"&")==0) //chech if it is ampersand
        {
        args[n]='\0';//change the ampersan to '/0'
        return 1;} //if so return 1
    else
        return 0;}
return 0;
}
void func(int signum)
{    FILE *fp;
   fp = fopen("shell3.log", "a"); //open the file
   fprintf(fp, "Child process was terminated.\n");//write this in the file
   fclose(fp);

}
int cd(char *path)
{
    return chdir(path); //it takes a string called path and give it to chdir to make this be our current directory
}
int main()
{

    while(1)
    {

    char *args[max_word];
    char *line[max_char];

   printf("Shell >");
    int x=read_parse_line(args,line) ; //it returns 0 id there is no command
     int  back=is_background(args);
    if(x==0)
    {
    printf("\n");// if no command continue and get the next line
   continue;
    }

        if(strcmp(args[0],"cd")==0) //if the command is cd it goes to sd_func to change the current directory
        {
            if(cd(args[1])<0)
            {
                perror(args[1]);
            }
            continue;
        }
signal(SIGCHLD,func); //it goes to func when the SIGCHLD is active when the child process dies

        pid_t child_pid=fork(); //this fork means that all the coming line will be executed twice
        if(child_pid==0) //chile mode
        {

            execvp(args[0],args);// execute system commands if the command is valid it will not return and process will be terminated
            printf("Invalid Command\n");//if it returned so it is invalid and will print this
            continue;
        }
        else if (child_pid < 0)
        {
            perror("Fork failed"); //error in fork
            exit(1);
        }
        else//parent mode
        {
               if(back==0) //if the child process is running in foreground so parent must wait till it terminates
               {
               waitpid(child_pid,0);
              }
                else //if the child process is running in background so parent will not wait
                back=0;

        }

    }
    return 0;
}




