/*Programa completo - MENU*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>

#define SIZE_BUFFER 512
#define STDOUT 1

void directorioActual();
void verDirectorio();

void verPermisosFichero();
char* permisosF(mode_t);

void cambiarNombreDirectorio();
void cambiarNombreFichero();
void verContenidoFichero();
void copiarFichero();
void moverFichero();
void eliminarDirectorio();
void eliminarFichero();
void cambiarPermisos();
void crearFichero();
void compContenidoFichero();
void buscarAFichero();



int main(void){
    int opcion;

    //MENU

    while(1){
        printf("\n");
        printf("-------------- MENU -----------\n\n");
        printf("1. Saber directorio actual\n");
        printf("2. Listar directorio actual\n");
        printf("3. Ver permisos de un archivo\n");
        printf("4. Cambiar nombre de directorio\n");
        printf("5. Cambiar nombre de archivo\n");
        printf("6. Ver contenido de un archivo\n");
        printf("7. Copiar archivo\n");
        printf("8. Mover Archivo\n");
        printf("9. Eliminar directorio\n");
        printf("10. Eliminar archivo\n");
        printf("11. Cambiar permisos archivo\n");
        printf("12. Crear archivo\n");
        printf("13. Comparar contenido de archivos\n");
        printf("14. Buscar archivo\n");
        printf("15. Salir\n");

        printf("\nSeleccione una opcion del menu: ");
        scanf("%d",&opcion);


        switch(opcion){
            case 1:
                directorioActual();
                break;

            case 2:
                verDirectorio();
                break;
            
            case 3:
                verPermisosFichero();
                break;
            
            case 4:
                cambiarNombreDirectorio();
                break;
            
            case 5:
                cambiarNombreFichero();
                break;
            
            case 6:
                verContenidoFichero();
                break;
            
            case 7:
                copiarFichero();
                break;
            
            case 8:
                moverFichero();
                break;
            
            case 9:
                eliminarDirectorio();
                break;
            
            case 10:
                eliminarFichero();
                break;
            
            case 11:
                cambiarPermisos();
                break;
           
            case 12:
                crearFichero();
                break;
           
            // case 13:
           
            case 14:
                buscarAFichero();
                break;
           
            case 15:
                printf("Saliendo...Hasta pronto!\n");
                return 0;
           
            default: 
                printf("Opcion incorrecta\n");
        }
        
    }
    return 0;
}

//FUNCIONES

//1
void directorioActual(){
    char buffer[SIZE_BUFFER];
    printf("El directorio actual es: %s\n",getcwd(buffer,SIZE_BUFFER));
}
//2
void verDirectorio(){
    char ruta[SIZE_BUFFER];
    printf("\nIntroduce ruta de directorio (Relativa o completa): ");
    scanf("%s",ruta);

    DIR *dir;
    struct dirent *mi_dirent;

    if(ruta == NULL){
        printf("No has introducido directorio\n");
        exit(-1);
    }
    if ((dir = opendir(ruta))== NULL){
        perror("opendir");
        exit(-1);
    }
    while((mi_dirent = readdir(dir))!=NULL)
        printf("%s\n",mi_dirent->d_name);

    closedir(dir);
}
//3
void verPermisosFichero(){
    int fd,size;
   
    char file[SIZE_BUFFER];
    struct stat info;

    printf("Introduce nombre del archivo: \n");
    scanf("%s",file);

    if((fd = open(file, O_RDWR))==-1){
        perror("open");
        exit(-1);
    }
    stat(file,&info);

    size = info.st_size;

    printf("Persmisos en BITS: %04o\n",info.st_mode & 07777);
    printf("Permisos en Char: USR-GRP-OTH %s\n",permisosF(info.st_mode));
}
//3.1
char* permisosF(mode_t mode){

    static char permisos[10];
        
    strcpy(permisos,"---------");

    //Usuario
    if(mode & S_IRUSR)
        permisos[0] = 'r';
        
    if(mode & S_IWUSR)
        permisos[1] = 'w';
        
    if(mode & S_IXUSR)
            permisos[2] = 'x';
        
        //Grupo
    if(mode & S_IRGRP)
        permisos[3] = 'r';

    if(mode & S_IWGRP)
        permisos[4] = 'w';

    if(mode & S_IXGRP)
        permisos[5] = 'x';

        //Otro
    if(mode & S_IROTH)
        permisos[6] = 'r';

    if(mode & S_IWOTH)
        permisos[7] = 'w';

    if(mode & S_IXOTH)
        permisos[8] = 'x';

    return permisos;
}
//4
void cambiarNombreDirectorio(){
    char newName[SIZE_BUFFER];
    char name[SIZE_BUFFER];
    
    printf("Introduce directorio que quiere renombrar: \n");
    scanf("%s",name);
    printf("Introduce numero nombre: \n");
    scanf("%s",newName);

    if(rename(name,newName) == 0){
        printf("Directorio renombrado con exito. %s, ahora se llama %s\n",name,newName);
    }else{
        perror("Rename");
    
    }
}
//5
void cambiarNombreFichero(){
    char newName[SIZE_BUFFER];
    char name[SIZE_BUFFER];
    
    printf("Introduce nombre de Fichero que quiere renombrar: \n");
    scanf("%s",name);
    printf("Introduce numero nombre: \n");
    scanf("%s",newName);

    if(rename(name,newName)== 0){
        printf("Directorio renombrado con exito. %s, ahora se llama %s\n",name,newName);
    }else{
        perror("Rename");
    
    }
}
//6
void verContenidoFichero(){
    int fd,r;
    char name[SIZE_BUFFER];

    printf("Introduce nombre del fichero que quieres leer: ");
    scanf("%s",name);

    char buffer[SIZE_BUFFER];

    if((fd = open(name,O_RDWR)) == -1){
        perror("open");
        exit(-1);
    }
    r = read(fd,buffer,SIZE_BUFFER);

    write(STDOUT,buffer,r);
    close(fd);
}
//7
void copiarFichero(){
    char origen[SIZE_BUFFER];
    char destino[SIZE_BUFFER];

    printf("Introduce origen: ");
    scanf("%s",origen);
    printf("Introduce destino: ");
    scanf("%s",destino);

    if(link(origen,destino)==-1){
        perror("error link\n");
        exit(-1);
    }
    if(unlink(origen)==-1){
        perror("error unlink\n");
        exit(-1);
    }
}


//8
void moverFichero(){
    char newName[SIZE_BUFFER];
    char name[SIZE_BUFFER];
    
    printf("Introduce nombre de Fichero que quiere mover: \n");
    scanf("%s",name);
    printf("Introduce numero ruta: \n");
    scanf("%s",newName);

    if(rename(name,newName)== 0){
        printf("Archivo movido con exito. %s, ahora esta en %s\n",name,newName);
    }else{
        perror("Move");
    
    }
}


//9
void eliminarDirectorio(){
    char dir[256];

    printf("Introduce directorio que quiere eliminar (ruta completa o relativa): ");
    scanf("%s",dir);

    

    if(rmdir(dir)==0){
        printf("Directorio eliminado");

    }else{
        perror("remove");
        exit(-1);
    }
}
//10
void eliminarFichero(){
    char fichero[256];

    printf("Introduce fichero que quiere eliminar (ruta completa o relativa): ");
    scanf("%s",fichero);

    if(remove(fichero)==0){
        printf("Fichero eliminado");

    }else{
        perror("remove");
        exit(-1);
    }
}

//11
void cambiarPermisos(){

    char archivo[256];
        
    printf("Introduce nombre del archivo: ");
        
    scanf("%s",archivo);

    mode_t chPermisos;

    printf("Introduce permisos en octal para el fichero %s: ",archivo);
    scanf("%o",&chPermisos);

    if(chmod(archivo,chPermisos)==-1){
            perror("Error chmod");
            exit(-1);
    }
    printf("Permisos cambiados correctamente a %o para el archivo %s\n",chPermisos,archivo);
}
//12
void crearFichero(){
    char name[SIZE_BUFFER];
    printf("Introduce nombre para fichero nuevo: ");
    scanf("%s",name);

    FILE *fichero = fopen(name,"w");
    if(fichero == NULL){
        perror("createFile");
        exit(-1);
    }
    printf("Fichero '%s', creado con exito\n",name);

    fclose(fichero);
}

//14
void buscarAFichero(){
    char fichero[SIZE_BUFFER];
    char dirb[SIZE_BUFFER];

    printf("\nIntroduce nombre del fichero que quieres buscar: ");
    scanf("%s",fichero);

    printf("\nIntroduce directorio donde quieres buscar: ");
    scanf("%s",dirb);


    DIR *dir;
    struct dirent *mi_dirent;

    if((dir = opendir(dirb))== NULL){
        perror("opendir");
        exit(-1);
    }
    
    while(mi_dirent!=NULL){
        if((mi_dirent = readdir(dir))!= NULL){
            if(strcmp(mi_dirent->d_name,fichero)==0){
                printf("Fichero %s encontrado\n",fichero);
                closedir(dir);
                return;
            }
        }
    }
    

}