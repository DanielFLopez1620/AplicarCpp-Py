#include <iostream> //Libreria para entradas y salidas
#include <stdio.h> //Libreria estandar para manejo de archivos
#include <locale.h> //Libreria de formato de región
#include <time.h> //Libreria de tiempo y fechas
#include <string.h> //Libreria de manejo de cadena y caracteres
#include <cstdlib> //Libreria de usó general
#include <math.h> //Libreria para operaciones avanzadas
#ifdef _WIN32
#include<windows.h> //Libreria
#endif
using namespace std;
//constantes y estructuras
FILE *archivoj;//Puntero para la creacion de ususarios
FILE *reserva; //Puntero para eliminar los usuarios
FILE *records; //Puntero para la creacionde los archivos de records
FILE *informacion; //Puntero para la informacion del jugador
FILE *archivou; //Puntero para el registro ed codigo
FILE *archivop;//Puntero para el guardado de pausado

const int MJug = 5; //Constante para el tamaño de puntos y gamertag
const int M_filas=39;//Constante para el tamaño maximo de filas
const int M_columnas=119;//Constante para el tamaño de columnas
const int MJ=20; //Constante para el tamaño maximo de Jugadores
const int ML=15;//Constante para el maximo de letras

struct jugador //Estructura para tener todaas las propiedades del jugador
{
    char nom_jugador [ML];
    char ape_jugador [ML];
    char gamertag[ML]= {' '};
    int codigo=0;
    int partidas_ganadas=0;
    int juegos=0;
    char inicial;
    int mejor_puntaje=0;
    int puntos=0;
};
struct juego //Estructura para generar el pausado del juego con la informacion necesaria
{
    char tablero [M_filas][M_columnas];
    char tablero_pertenencia[M_filas][M_columnas];
    char tipo;
    jugador partida[5];
    int puntos[5]= {0};
    int turno=0;
    int fila=0;
    int num=5;
    int col=0;
};
//prototipos
void creacion_usuario(jugador gamer); //Como su nombre lo idica es para crear el registro con la informacion de los jugadores y asignar los puntos
int vectorizar(jugador jugadores[]); // Funcion creada para abarcar toda la informacion de los jugadores en un vector
void actualizar(int fila, int col, char tablero[][M_columnas]); // llenado y actualización de la matriz
void tablero_inicial(int fila, int col,char tablero[][M_columnas]);  //mostrar el tablero inicial con asterisco de ayuda visual
void tu_turno(int fila, int col,int &turno,char tablero[][M_columnas],jugador partida[],int puntos[],bool &quedarse,char tablero_pertenencia[][M_columnas]);  //lee coordenadas de movimiento, verifica y lo despliga, añadiendo puntos respectivos  //obtener la inicial de un nombre
bool valido(int x1,int x2,int y1, int y2); // verifica si las coordenadas son lineal y de largo 1
bool libre(int x,int y,char tablero[][M_columnas]); //verifica si el espacio aun esta libre
bool completo(int x,int y, int fila, int col,char tablero[][M_columnas],int &pos); // Verificar que un cuadro en el tablero este completo
bool faltan(char tablero[][M_columnas],int fila, int col);  //si aun existe casillas libres para el juego
void cuadrito_para(int turno, jugador partida[], int x, int y, int pos, char tablero[][M_columnas], int puntos[]); //Asignar quien se lleva el cuadro de los jugadores
void jugada_maquina(int fila, int col,char tablero[][M_columnas], int puntos[], int &turno, jugador partida[],char tablero_pertenencia[][M_columnas] );// Jugabilidad de la maquina
void jugada_aleatoria(int filas, int col,char tablero[][M_columnas],char tablero_pertenencia[][M_columnas]); //Función para el movimiento aleatorio de la maquina
void borrado_usuario(jugador gamer, string gamertag); //Eliminar un usuario de los registros por medio del gamertag
void borrar(); //Borrar el tablero
void asig_puntos(jugador jugadores[], jugador partida[],int num, int puntos[], int num_jugadores); //Asignación final para puntos, victorias y cuadros completados en su historial
void ganador(int puntos [], int num,jugador jugadores[], int num_jugadores, jugador partida[]); //Identifica por medio de los puntos cual es el ganador de la partida
void ver_record(jugador jugadores[], int num_jugadores, int opt); //Ver todos los puntajes
void desv(jugador jugadores[], int num_jugadores); // Ver todos los puntajes más altos
void mostrar_exportar( int num_jugadores, jugador jugadores [], int opt); // De acuerdo a la opción indica o se muestran en pantalla
void inicial(string nombres,char &inicial);//Para sacar las iniciales de iniciales


int main()
{
    jugador jugadores[20];//Vector de guardado de jugadores
    jugador gamer;//estrucutura auxiliar
    int num;
    int filas=9*2-1, columnas=9*2-1;  //asignación de tamaño de tableros
    int nivel=0;
    int opt=0; //variables de manejo de menú
    int turno=0, libres, puntos [MJug]= {0};
    char tablero [M_filas][M_columnas];
    char tablero_doc [M_filas][M_columnas];
    char iniciales[MJug];
    char apoyo[ML]= {' '};
    bool diponible = true;
    char comp;
    int ultimo_codigo = 0;
    string nom_aux;
    char tablero_pertenencia[M_filas][M_columnas];
    bool pausa = false;
    bool existe=false;
    bool quedarse = true;
    int num_jugadores=20;
    int posv, yo;
    juego pausado;
    int auxi;
    setlocale(LC_ALL,"");//obtención de formato regional por fecha
    num_jugadores=vectorizar(jugadores);//guardado de jugadores existentes
    cout<<"Bienvenido al juego de cuadrito del grupo #04"<<endl;
    do
    {
        cout<<"menú: "<<endl;//Menu de juego
        cout<<"\n\t  1) Jugar \n\t  2) Configurar dificultad \n\t  3) Administrar jugadores \n\t  4) Estadísticas \n\t 6) Salir \n\t"<<endl;
        cout<<"\tSu selección: ";
        cin>>opt;
        borrar();
        if (opt==1)
        {
            do
            {
                cout<<"Bienvenido al menu de cuadrito"<<endl;//Menu de jugabilidad
                cout<<"menú: "<<endl;
                cout<<"1) Contra la máquina \n\t  2) Multijugador \n\t  3) Instrucciones \n\t  4) Retomar juego pausado \n\t 5) Salir y volver al menu principal \n\t"<<endl;
                cout<<"\tSu selección: ";
                cin>>opt;
                borrar();
                if(opt==4) //Opcion de pausado
                {
                    archivop = fopen("pausado.txt","r");
                    if(archivoj == NULL)
                    {
                        cout<<"No tienes juegos en pausa..."<<endl;
                    }
                    else
                    {
                        fread(&pausado,sizeof(pausado),1,archivop);
                        fclose(archivop);
                        pausa=true;
                        if(pausado.tipo=='m')
                        {
                            opt=1;
                        }
                        else
                        {
                            opt=2;
                        }

                    }
                }
                if (opt==1)//Entrada a la jugabilidad de maquina
                {
                    jugador partida[5];
                    if(pausa&& pausado.tipo=='m') //Condicion para verificar si no viene de un juego pausado
                    {
                        cout<<"¿Desea continuar el juaego pausado? (sí=s)"<<endl;
                        cin>>comp;
                        if(comp=='s')
                        {
                            for(int i=0; i<M_filas; i++)
                            {
                                for(int j=0; j<M_columnas; j++) //Cargado de la matriz de pausado
                                {
                                    tablero[i][j]=pausado.tablero[i][j];
                                    tablero_pertenencia[i][j]=pausado.tablero_pertenencia[i][j];
                                }
                            }
                            filas=pausado.fila;
                            columnas=pausado.col;
                            num=pausado.num;
                            for(int j=0; j<num; j++)
                            {
                                puntos[j]=pausado.puntos[j];
                                partida[j]=pausado.partida[j];
                            }
                            turno=pausado.turno;
                        }
                        actualizar(filas,columnas,tablero);
                        quedarse=true;
                    }
                    else
                    {
                        pausa=false;
                    }
                    if(!pausa)
                    {
                        jugador maquina;//inicializacion de la mquina
                        maquina.inicial='m';
                        string maqui="Maquina";
                        strcpy(maquina.nom_jugador,maqui.c_str());
                        partida[1]=maquina;
                        existe=false;
                        while(!existe)
                        {
                            cout<<"Digite un nombre usuario: ";
                            getline(cin>>ws,nom_aux);
                            strcpy(apoyo,nom_aux.c_str());
                            for(int a=0; a<num_jugadores; a++)
                            {
                                int k=0;
                                for(int i=0; i<ML; i++)
                                {
                                    if(jugadores[a].gamertag[i]==apoyo[i])//Verificar si el jugador existe
                                    {
                                        k++;
                                    }
                                }
                                if(k==ML)
                                {
                                    cout<<"Jugador existente, puede proseguir"<<endl;
                                    num_jugadores = vectorizar(jugadores);
                                    partida[0]=jugadores[a];
                                    existe=true;
                                    break;
                                }
                            }
                            if(!existe)//Creacion de jugador en caso de no existencia
                            {
                                cout<<"Jugador no hallado"<<endl;
                                cout<<"1)Cree el jugador con el mismo gamertag"<<endl;
                                cout<<"2)presione cualquier otro número para cambiar de gamertag"<<endl;
                                cin>>opt;
                                borrar();
                                if(opt==1)
                                {
                                    creacion_usuario(gamer);//Creacion del jugador
                                    num_jugadores = vectorizar(jugadores);//Y se guarda en el vectorizar
                                    partida[0]=jugadores[num_jugadores-1];
                                    existe=true;

                                }
                                else
                                {

                                    existe=false;
                                }
                            }
                        }
                        for(int i=0; i<filas; i++)
                        {
                            for(int j=0; j<columnas; j++)
                            {
                                tablero_pertenencia[i][j]=' ';
                            }
                        }
                        tablero_inicial(filas,columnas,tablero);
                        cout<<"\n";
                        srand(time(0));
                        turno=rand()%2;
                        if(turno==1)
                        {
                            jugada_aleatoria(filas,columnas,tablero,tablero_pertenencia);
                            borrar();
                            actualizar(filas,columnas,tablero);//puerba inicial de la maquina
                            turno++;
                        }
                    }

                    while(faltan(tablero,filas,columnas)&& quedarse)
                    {

                        if(turno==0)
                        {
                            tu_turno(filas,columnas,turno,tablero,partida,puntos,quedarse,tablero_pertenencia);
                            borrar();
                            actualizar(filas,columnas,tablero);
                            turno++;
                        }
                        else if(turno==1)
                        {
                            jugada_maquina(filas,columnas,tablero,puntos,turno,partida,tablero_pertenencia);//llamamos la funcion de la jugabilidad de la maquina
                            borrar();
                            actualizar(filas,columnas,tablero);
                            turno++;
                        }
                        turno=turno%2;
                    }
                    if(quedarse == false)//Verificacion de pausa del juego
                    {
                        cout<<"Se ha pausado/salido del juego"<<endl;
                        pausado.col=columnas;
                        pausado.fila=filas;
                        pausado.num=2;
                        cout<<"num "<<pausado.num<<endl;
                        for(int j=0; j<pausado.num; j++)
                        {
                            pausado.puntos[j]=puntos[j];
                            pausado.partida[j]=partida[j];

                        }
                        for(int i=0; i<filas; i++)
                        {
                            for(int j=0; j<columnas; j++)
                            {
                                pausado.tablero[i][j]=tablero[i][j];
                                pausado.tablero_pertenencia[i][j]=tablero_pertenencia[i][j];
                            }
                        }
                        pausado.tipo='m';
                        pausado.turno=turno-1;
                        pausa=true;
                        archivop=fopen("pausado.txt","r+");//Guardado en el archivo de la partida
                        if(archivop==NULL)//Si no esta se crea
                        {
                            archivop=fopen("pausado.txt","w");
                            fclose(archivop);
                            archivop=fopen("pausado.txt","r+");
                        }
                        fwrite(&pausado,sizeof(pausado),1,archivop);
                        fclose(archivop);
                    }
                    else
                    {
                        cout<<"Ha finalizado el juego"<<endl;

                        cout<<"Puntos del jugador "<<partida[0].nom_jugador<<": "<<puntos[0]<<endl;

                        asig_puntos(jugadores,partida,1,puntos,num_jugadores);
                        ganador(puntos,2,jugadores,num_jugadores,partida);
                        desv(jugadores, num_jugadores);
                    }

                }
                else if (opt==2)//Jugabilidad de multijugador
                {
                    jugador partida[5]; //jugaodres de la partida
                    if(pausa && pausado.tipo=='l')
                    {
                        cout<<"¿Desea continuar el juaego pausado? (sí=s)"<<endl;
                        cin>>comp;
                        if(comp=='s')  //cargado de juego en pausa proveniente de un archivo
                        {
                            for(int i=0; i<M_filas; i++)
                            {
                                for(int j=0; j<M_columnas; j++)
                                {
                                    tablero[i][j]=pausado.tablero[i][j];
                                    tablero_pertenencia[i][j]=pausado.tablero_pertenencia[i][j];
                                }
                            }
                            filas=pausado.fila;
                            columnas=pausado.col;
                            num=pausado.num;  //asiganción para continuar juego pausado
                            for(int j=0; j<num; j++)
                            {
                                puntos[j]=pausado.puntos[j];
                                partida[j]=pausado.partida[j];
                            }
                            turno=pausado.turno;
                            actualizar(filas,columnas,tablero);  //mostrado del ultimo registro de tablero
                            quedarse=true;
                        }
                    }
                    else
                    {
                        pausa=false;
                    }
                    if(!pausa)   //inicio de nuevo juego
                    {
                        quedarse=true;
                        cout<<"Digite el número de jugadores de esta partida: ";
                        cin>> num;
                        for(int i=0; i<num; i++)
                        {
                            existe=false;
                            cout<<"Digite el gamertag del usuario:";
                            cin>> nom_aux;
                            strcpy(apoyo,nom_aux.c_str());
                            //comprobación de existencia
                            for(int a=0; a<num_jugadores; a++)
                            {
                                int k=0;
                                for(int m=0; m<ML; m++)
                                {
                                    if(jugadores[a].gamertag[m]==apoyo[m])//verificación de existencia de jugadores
                                    {
                                        k++;
                                    }
                                }
                                if(k==ML)
                                {
                                    cout<<"Jugador existente, puede proseguir"<<endl;
                                    num_jugadores = vectorizar(jugadores);
                                    partida[i]=jugadores[a];
                                    existe=true;
                                    break;
                                }
                            }
                            if(!existe)//Crea el jugador en caso de inexistencia
                            {
                                cout<<"Jugador no hallado"<<endl;
                                cout<<"1)Cree el jugador con el mismo gamertag"<<endl;
                                cout<<"2)presione cualquier otro número para cambiar de gamertag"<<endl;
                                cin>>opt;
                                borrar();
                                if(opt==1)
                                {
                                    creacion_usuario(gamer);  //crear jugadores
                                    num_jugadores = vectorizar(jugadores);  //actualizar datos en memoria local
                                    partida[i]=jugadores[num_jugadores-1];
                                }
                                else
                                {
                                    i--;
                                }

                            }
                        }
                        for(int i=0; i<num; i++) //ciclo de obtención de iniciales
                        {

                            cout<<"Incial jugador #"<<i<<" :"<<partida[i].inicial<<endl;  //mostrar iniciales de jugadores
                        }


                        tablero_inicial(filas, columnas,tablero);  //inicialización de tablero
                        for(int i=0; i<num_jugadores; i++)  //despejar el vector de puntos
                        {
                            puntos[i]=0;
                        }
                    }
                    num_jugadores=vectorizar(jugadores);
                    while(faltan(tablero,filas, columnas) && (quedarse == true)) //ciclo de juego
                    {
                        tu_turno(filas,columnas,turno,tablero,partida,puntos,quedarse,tablero_pertenencia);
                        borrar();
                        actualizar(filas, columnas,tablero);
                        turno++;  //guía para contadores ,vectores y cuadros completados
                        if(turno>= num)  //reasignacion de turno
                        {
                            turno=0;
                        }
                    }
                    if(quedarse== false)//verificación de pausa de juego
                    {
                        cout<<"Se ha pausado/salido del juego"<<endl;
                        pausado.col=columnas;
                        pausado.fila=filas;  //guardado de datos
                        pausado.num=num;
                        cout<<"num "<<pausado.num<<endl;
                        for(int j=0; j<pausado.num; j++)
                        {
                            pausado.puntos[j]=puntos[j];
                            pausado.partida[j]=partida[j];

                        }
                        for(int i=0; i<filas; i++)
                        {
                            for(int j=0; j<columnas; j++)
                            {
                                pausado.tablero[i][j]=tablero[i][j];
                                pausado.tablero_pertenencia[i][j]=tablero_pertenencia[i][j];
                            }
                        }
                        pausado.tipo='l';
                        pausado.turno=turno-1;
                        pausa=true;
                        archivop=fopen("pausado.txt","r+");  //escritura de datos guardados en archivo
                        if(archivop==NULL)
                        {
                            archivop=fopen("pausado.txt","w");
                            fclose(archivop);
                            archivop=fopen("pausado.txt","r+");
                        }
                        fwrite(&pausado,sizeof(pausado),1,archivop);
                        fclose(archivop);
                    }
                    else
                    {
                        cout<<"Ha finalizado el juego"<<endl;
                        for(int i=0; i<num; i++)
                        {
                            cout<<"Puntos del jugador "<<partida[i].nom_jugador<<": "<<puntos[i]<<endl;
                        }
                        asig_puntos(jugadores,partida,num,puntos,num_jugadores);
                        ganador(puntos,num,jugadores,num_jugadores,partida);  // asignación de puntos y guardado de datos
                    }
                    desv(jugadores, num_jugadores);// escritura con desvectorizacion
                }
                else if (opt==3)   //mostrado de instrucciones
                {
                    cout << "Intrucciones de juego: "<<endl;
                    cout << "1) Complete la mayor cantidad de cuadros para ganar. "<<endl;
                    cout << "2) Si completa un cuadro recibe un turno extra."<<endl;
                    cout << "3) Los movimientos se realizan por posiciones en x1, y1, x2, y2 cuyo origen está en la esquina superior izuquierda."<<endl;
                    cout << "4) Los movimientos son válidos si y solo si no se superponen a otros, son continuos y en espacios válidos. "<<endl;
                    break;
                }
            }while (opt!=5);
        }
        else if (opt==2)  //configuraciónd de tablero
        {
            do
            {
                cout<<"Bienvenido al menu de escogencia de dificultad"<<endl;
                cout<<"menú: "<<endl;
                cout<<"\n\t  1) Principiante \n\t  2) Intermedio \n\t  3) Avanzado \n\t  4) Libre \n\t 5) Salir y volver al menu principal \n\t"<<endl;
                cout<<"\tSu selección: ";
                cin>>opt;
                borrar();
                if (opt==1)
                {
                    filas=9*2-1;  // asignación para un tablero de coordenadas y de suficiente espacio.
                    columnas=9*2-1;
                    break;
                }
                else if (opt==2)
                {
                    filas=16*2-1;
                    columnas=16*2-1;
                    break;
                }
                else if (opt==3)
                {
                    filas=16*2-1;
                    columnas=30*2-1;
                    break;
                }
                else if (opt==4)   //asignación libre con verificación.
                {
                    cout <<"¿Cuantas filas desea tener? maximo 20"<<endl;
                    cin >> auxi;
                    if(auxi>20 || auxi<=0)
                    {
                        cout<< "Porfavor dijite menos de 20 filas o mas de 0 filas"<<endl;
                        cin >> filas;
                        filas = filas*2-1;
                    }
                    else
                    {
                        filas=auxi*2-1;
                    }
                    cout <<"¿Cuantas columnas desea tener? maximo 60"<<endl;
                    cin >> auxi;
                    if(auxi>60 || auxi<=0)
                    {
                        cout<< "Porfavor dijite menos de 60 columnas o mas de 0 columnas"<<endl;
                        cin >> columnas;
                        columnas = columnas*2-1;
                    }
                    else
                    {
                        columnas=auxi*2-1;
                    }
                    break;
                }
            }
            while(opt!=5);
        }
        else if (opt==3)   //cambio de jugadores y actualización de datos
        {
            cout<<"\t1)Crear jugador \n\t2)Eliminar Jugador \n\tOtro numero para volver al menu"<<endl;
            cin>> opt;
            borrar();
            if(opt == 1)
            {
                creacion_usuario(gamer);  //creación con código autoincremental
                num_jugadores=vectorizar(jugadores);
            }
            else if(opt == 2)
            {
                cout<<"Digite el gamertag a eliminar:";
                //cin>> nom_aux;
                getline(cin>>ws,nom_aux);
                borrado_usuario(gamer,nom_aux);   //eliminación física de usuario
                num_jugadores=vectorizar(jugadores);
            }
            cout<<"Regresando al menú"<<endl;
        }
        else if (opt==4)//Exporte y mostrado de datos
        {
            cout<<"Mostrado y exportacion:\n\t1)Mejores puntajes\n\t2)Ver info del jugador\n\t3)Reporte mejores puntajes3\n\t4)Reporte mejores puntajes\n\tOtra opción para volver al menú"<<endl;
            cin>> opt;
            switch(opt)
            {
            case 1:
                ver_record(jugadores,num_jugadores,opt);   //mostrado en pantalla (varia con opt)
                break;
            case 2:
                mostrar_exportar(num_jugadores,jugadores,opt);  //mostrado de usuario (varia con opt)
                break;
            case 3:
                ver_record(jugadores,num_jugadores,opt);  //Exportes de datos.
                break;
            case 4:
                mostrar_exportar(num_jugadores,jugadores,opt);  //esporte de datos
                break;
            default:
                cout<<"Redireccionando al menú"<<endl;
            }
            cout<<"Volviendo al menú"<<endl;
        }
    }

    while (opt!=6);   //salida final del programa
    cout<<"Gracias por jugar"<<endl;
    return 0;   //retorno final
}
//--------------------------------------------------------Jugabilidad-------------------------------------------------
void jugada_maquina(int fila, int col,char tablero[][M_columnas], int puntos[], int &turno, jugador partida[],char tablero_pertenencia[][M_columnas])
{

    int contador_llenos=0, pos;
    bool salir=false, gan=false;
    for(int i=0; i<fila; i++)  //iteración en la filas del tablero
    {
        for(int j=0; j<col; j++)  //iteración en las columnas del tablero
        {
            if(tablero_pertenencia[i][j]==partida[turno].inicial)// verificación de las jugadas realizadas por el usuario
            {
                if((i)%2==0 && (j)%2!=0)  //verificación de posición para robar cuadro
                {
                    if(tablero_pertenencia[i+1][j-1]==partida[turno].inicial&&tablero_pertenencia[i+1][j+1]==partida[turno].inicial&&tablero_pertenencia[i+2][j]==' ')
                    {
                        tablero_pertenencia[i+2][j]='m';
                        tablero[i+2][j]='-';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i+2,j,fila, col,tablero,pos);
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,  i+2,  j,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;
                    }
                    else if(tablero_pertenencia[i+1][j-1]==' '&&tablero_pertenencia[i+1][j+1]==partida[turno].inicial&&tablero_pertenencia[i+2][j]==partida[turno].inicial)  //robar cuadro
                    {
                        tablero_pertenencia[i+1][j-1]='m';
                        tablero[i+1][j-1]='|';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i+1,j-1,fila, col,tablero,pos);
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,  i+1,j-1,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;
                    }
                    else if(tablero_pertenencia[i+1][j-1]==partida[turno].inicial&&tablero_pertenencia[i+1][j+1]==' '&&tablero_pertenencia[i+2][j]==partida[turno].inicial) //robo de cuadro
                    {
                        tablero_pertenencia[i+1][j+1]='m';
                        tablero[i+1][j+1]='|';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i+1,j+1,fila, col,tablero,pos);
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,  i+1,j+1,  pos,   tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;
                    }
                }
                else if(i%2!=0&&j%2==0)
                {
                    if(tablero_pertenencia[i-1][j+1]==' '&&tablero_pertenencia[i][j+2]==partida[turno].inicial&&tablero_pertenencia[i+1][j+1]==partida[turno].inicial)
                    {
                        tablero_pertenencia[i-1][j+1]='m';
                        tablero[i-1][j+1]='-';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i-1,j+1,fila, col,tablero,pos);
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,  i-1,j+1,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno en caso de victoria
                        }
                        break;
                    }
                }
            }
            if(tablero_pertenencia[i][j]=='m')  //en caso de que sea movimiento de la maquina en busca de ganar puntos
            {
                if((i)%2==0 && (j)%2!=0)//lectura de tablero para posicionar - ó |
                {
                    if(tablero[i+1][j-1]==' ')
                    {
                        tablero_pertenencia[i+1][j-1]='m';
                        tablero[i+1][j-1]='|';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i+1,j-1,fila, col,tablero,pos);
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,  i+1,j-1,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;
                    }
                    else if(tablero[i+1][j+1]==' ')//ubicación de movimiento en busca de completar un cuadro
                    {
                        tablero_pertenencia[i+1][j+1]='m';
                        tablero[i+1][j+1]='|';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i+1,j+1,fila, col,tablero,pos);
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,  i+1,j+1,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;
                    }
                    else if(tablero[i+2][j]==' ')   //continuación de completar cuadro
                    {
                        tablero_pertenencia[i+2][j]='m';
                        tablero[i+2][j]='-';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i+2,j,fila, col,tablero,pos);  //verifiación de completar cuadro
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,  i+2,j,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;
                    }
                    else if(tablero[i-1][j-1]==' ') //viabilidad de movimiento
                    {
                        tablero_pertenencia[i-1][j-1]='m';
                        tablero[i-1][j-1]='|';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i-1,j-1,fila, col,tablero,pos);// verificación de completar cuadro
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,  i-1,j-1,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;
                    }
                    else if(tablero[i-1][j+1]==' ')  //lectura de posible movimientos
                    {
                        tablero_pertenencia[i-1][j+1]='m';
                        tablero[i-1][j+1]='|';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i-1,j+1,fila, col,tablero,pos);
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,  i-1,j+1,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;
                    }
                    else if(tablero[i-2][j]==' ')
                    {
                        tablero_pertenencia[i-2][j]='m';
                        tablero[i-2][j]='-';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i-2,j,fila, col,tablero,pos);
                        if(gan)
                        {
                            cuadrito_para( turno,  partida, i-2,j,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;
                    }
                }
                else if(i%2!=0&&j%2==0)
                {
                    if(tablero[i-1][j+1]==' ')
                    {
                        tablero_pertenencia[i-1][j+1]='m';
                        tablero[i-1][j+1]='-';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i-1,j+1,fila, col,tablero,pos);
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,i-1,j+1,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;
                    }
                    else if(tablero[i][j+2]==' ')// validación de movimiento
                    {
                        tablero_pertenencia[i][j+2]='m';
                        tablero[i][j+2]='|';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i,j+2,fila, col,tablero,pos);
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,  i,j+2,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;
                    }
                    else if(tablero[i+1][j+1]==' ') //lectura de posible moviiento
                    {
                        tablero_pertenencia[i+1][j+1]='m';
                        tablero[i+1][j+1]='-';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i+1,j+1,fila, col,tablero,pos);
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,  i+1,j+1,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;
                    }
                    else if(tablero[i-1][j-1]==' ')// posición para - de posible movimiento
                    {
                        tablero_pertenencia[i-1][j-1]='m';
                        tablero[i-1][j-1]='-';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i-1,j-1,fila, col,tablero,pos);
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,  i-1,j-1,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;
                    }
                    else if(tablero[i][j-2]==' ')  //asignación en caso de hallar posible movimiento
                    {
                        tablero_pertenencia[i][j-2]='m';
                        tablero[i][j-2]='|';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i,j-2,fila, col,tablero,pos);// verificación de completar
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,  i,j-2,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;
                    }
                    else if(tablero[i+1][j-1]==' ') //última posibilidad de cuadro para buscar completar alguno
                    {
                        tablero_pertenencia[i+1][j-1]='m';
                        tablero[i+1][j-1]='-';
                        contador_llenos++;
                        salir=true;
                        gan = completo(i+1,j-1,fila, col,tablero,pos);
                        if(gan)
                        {
                            cuadrito_para( turno,  partida,  i+1,j-1,  pos,  tablero,  puntos);
                            puntos[turno]+=1;  //suma de puntos
                            turno--; //para repetición de turno
                        }
                        break;

                    }
                }
            }
            if(salir)
            {
                break;
            }
        }
    }
    if(contador_llenos==0 && faltan(tablero,fila,col))
    {
        jugada_aleatoria(fila,col,tablero,tablero_pertenencia); // sino detecta patron realiza una ejecución alaetaoria en el tablero
    }
    return;
}
void jugada_aleatoria(int filas, int col,char tablero[][M_columnas],char tablero_pertenencia[][M_columnas])
{
    int fila, columna;
    do
    {
        fila=rand()%filas;
        columna=rand()%col;
    }
    while((fila%2==0 && columna%2==0)||(fila%2!=0 && columna%2!=0));//Hacemos un bucle con la condicion de que sea distinto de multiples porque interfereria con la estructura del tablero
    if(fila%2==0 && columna%2!=0)
    {
        tablero[fila][columna]='-';
        tablero_pertenencia[fila][columna]='m';
    }
    else if(fila%2!=0 && columna%2==0)
    {
        tablero[fila][columna]='|';
        tablero_pertenencia[fila][columna]='m';
    }
    return;
}
void cuadrito_para(int turno, jugador partida[], int x, int y, int pos,  char tablero[][M_columnas], int puntos[])
{
    cout<<"Se ha completado un cuadro, el jugador "<<partida[turno].nom_jugador<<" obtiene un punto y otro turno"<<endl;
    switch(pos)  //donde va ubicada la inicial para el cuadro completado
    {
    case 1:
        tablero[x-1][y]= partida[turno].inicial;
        break;
    case 2:
        tablero[x+1][y]= partida[turno].inicial;
        break;
    case 3:
        tablero[x][y+1]= partida[turno].inicial;
        break;
    case 4:
        tablero[x][y-1]= partida[turno].inicial;
        break;
    case 5:
        tablero[x+1][y]= partida[turno].inicial;
        tablero[x-1][y]= partida[turno].inicial;
        puntos[turno]+=1;
        break;
    case 6:
        tablero[x][y+1]= partida[turno].inicial;
        tablero[x][y-1]= partida[turno].inicial;
        puntos[turno]+=1;
        break;
    default:
        cout<<"Ha ocurrido un error durante el turno"<<endl;
    }
    return;
}
void tu_turno(int fila, int col,int &turno,char tablero[][M_columnas],jugador partida[],int puntos[],bool &quedarse,char tablero_pertenencia[][M_columnas])
{
    int x1,x2,y1,y2,x,y,pos;
    bool gan;
    cout<<"Es el turno del jugador: "<<partida[turno].nom_jugador<<endl;  //lectura de coordenadas
    cout<<"Digite la coordena x1: ";
    cin>> x1;
    cout<<"Digite la coordena y1: ";
    cin>> y1;
    cout<<"Digite la coordena x2: ";
    cin>> x2;
    cout<<"Digite la coordena y2: ";
    cin>> y2;
    cout<<endl;
    if(x1 <=0 || x2<=0 || y1<=0 || y2<=0)  //condición de salida del juego
    {
        quedarse = false;
    }
    else
    {
        x= x1+x2-2;  //asignación para coordenada de matriz tablero
        y= y1+y2-2;
        while(!(valido(x1,x2,y1,y2)) || !(libre(x,y,tablero)))  //verificación de coordenadas, si son válidas
        {
            cout<<"Movimiento inválido, vuelva a digitar su movimiento por favor: "<<endl;
            cout<<"Digite la coordena x1: ";
            cin>> x1;
            cout<<"Digite la coordena y1: ";
            cin>> y1;
            cout<<"Digite la coordena x2: ";
            cin>> x2;
            cout<<"Digite la coordena y2: ";
            cin>> y2;
            cout<<endl;
            x= x1+x2-2;
            y= y1+y2-2;
        }
        if(x%2 == 0 && y%2 !=0)  //asignación de movimiento de acuerdo a posicion en la matriz
        {
            tablero[x][y]= '-';
            tablero_pertenencia[x][y]=partida[turno].inicial;
        }
        else
        {
            tablero[x][y]='|';
            tablero_pertenencia[x][y]=partida[turno].inicial;
        }
        gan = completo(x,y,fila, col,tablero,pos);  //si se ha completado un cuadro
        if(gan==true)
        {
            cuadrito_para( turno,  partida,  x,  y,  pos,  tablero,  puntos);
            puntos[turno]+=1;  //suma de puntos
            turno--; //para repetición de turno
        }
    }
    return;
}
//-------------------------------------------------Relacionadas al juego y manejo-----------------------------------------------------------
void tablero_inicial(int fila, int col,char tablero[][M_columnas])
{
    for(int i=0; i<fila; i++)  //llenado inicial de la matriz de juego y tablero
    {
        if(i%2==0)
        {
            for(int j=0; j<col; j++)
            {
                if(j%2==0)
                {
                    tablero[i][j]='*';
                    cout<<tablero[i][j]<<" ";
                }
                else
                {
                    tablero[i][j]=' ';
                    cout<<tablero[i][j]<<" ";
                }
            }
        }
        else
        {
            for(int j=0; j<col; j++)
            {
                tablero[i][j]=' ';
                cout<<tablero[i][j]<<" ";

            }
        }
        cout<<endl;
    }
    cout<<endl<<endl;
    return;
}
void actualizar(int fila, int col, char tablero[][M_columnas])
{
    for(int k=0; k<fila; k++)  //llenado y actualización de la matriz
    {
        for(int l=0; l<col; l++)
        {
            cout<<tablero[k][l]<<" ";
        }
        cout<<endl;
    }
    return;
}
bool valido(int x1,int x2,int y1, int y2)
{
    int cf,cc;
    cf=pow((x2-x1),2);
    cc=pow((y2-y1),2);
    if(cf > 1 || cc > 1)  //si los coordenadas suman más para ser continuas, se descartan
    {
        return false;
    }
    if(cf == 1 && cc == 1)  //si los dos coordenadas son iguales
    {
        return false;
    }
    return true;
}

bool libre(int x,int y, char tablero[][M_columnas])
{
    if(tablero[x][y]==' ') //verifica si el espacio está libre
    {
        return true;
    }
    return false;
}

bool completo(int x,int y,int fila, int col,char tablero[][M_columnas],int &pos)
{
    pos= 0;  //indicador de victoria con las coordenadas: 1 arriba, 2 abajo, 3 derecha y 4 izquierda
    if(tablero[x][y]=='-'  && x!=0 && x!=fila)
    {
        if((tablero[x+1][y-1]=='|' && tablero[x+1][y+1]== '|' && tablero[x+2][y]=='-')&&(tablero[x-1][y-1]=='|' && tablero[x-1][y+1]== '|' && tablero[x-2][y]=='-'))
        {
            pos = 5;
            return true;
        }
        if((tablero[x+1][y-1]=='|' && tablero[x+1][y+1]== '|' && tablero[x+2][y]=='-') )
        {
            pos = 2;
            return true;
        }
        if((tablero[x-1][y-1]=='|' && tablero[x-1][y+1]== '|' && tablero[x-2][y]=='-') )
        {
            pos = 1;
            return true;
        }
    }
    else if(tablero[x][y]=='-'  && x==0)
    {
        if((tablero[x+1][y-1]=='|' && tablero[x+1][y+1]== '|' && tablero[x+2][y]=='-') )
        {
            pos = 2;
            return true;
        }
    }
    else if(tablero[x][y]=='-' && x== fila)
    {
        if((tablero[x-1][y-1]=='|' && tablero[x-1][y+1]== '|' && tablero[x-2][y]=='-') )
        {
            pos = 1;
            return true;
        }
    }
    else if(tablero[x][y]=='|' && y!=0 && y!=col)
    {

        if((tablero[x][y+2]=='|' && tablero[x+1][y+1]=='-' && tablero[x-1][y+1]=='-')&&tablero[x][y-2]=='|' && tablero[x+1][y-1]=='-' && tablero[x-1][y-1]=='-')
        {
            pos = 6;
            return true;
        }
        if(tablero[x][y+2]=='|' && tablero[x+1][y+1]=='-' && tablero[x-1][y+1]=='-')
        {
            pos = 3;
            return true;
        }
        if(tablero[x][y-2]=='|' && tablero[x+1][y-1]=='-' && tablero[x-1][y-1]=='-')
        {
            pos = 4;
            return true;
        }
    }
    else if(tablero[x][y]=='|' && y==0)
    {
        if(tablero[x][y+2]=='|' && tablero[x+1][y+1]=='-' && tablero[x-1][y+1]=='-')
        {
            pos = 3;
            return true;
        }
    }
    else if(tablero[x][y]=='|' && y== col)
    {
        if(tablero[x][y-2]=='|' && tablero[x+1][y-1]=='-' && tablero[x-1][y-1]=='-')
        {
            pos = 4;
            return true;
        }
    }
    return false;  //si no ha completado ningún cuadrodo no manda nada
}
bool faltan(char tablero[][M_columnas],int fila, int col)
{
    for(int c=1; c<fila; c=c+2)
    {
        for(int d=1; d<col; d=d+2)
        {
            if(tablero[c][d]==' ')
            {
                return true; // si aún hay espacios continua el juego
            }
        }
    }
    return false;  //sino parar el juego
}
void asig_puntos(jugador jugadores[], jugador partida[],int num, int puntos[], int num_jugadores)
{
    for (int in=0; in<num; in++)
    {
        for (int j=0; j<num_jugadores; j++)
        {
            if(strcmp(partida[in].gamertag,jugadores[j].gamertag)==0)
            {
                jugadores[j].juegos++;
                jugadores[j].puntos+=puntos[in];
                if(jugadores[j].mejor_puntaje<puntos[in])
                {
                    jugadores[j].mejor_puntaje=puntos[in];
                }
                partida[in]=jugadores[j];
                break;
            }
            else if(j==num_jugadores-1)
            {
                cout<<"Ha ocurrido un problema en la asignación de datos"<<endl;
            }
        }
    }
    cout<< "Asignación completa, gracias por jugar"<<endl;
    return;
}
void ganador(int puntos [], int num,jugador jugadores[], int num_jugadores, jugador partida[])
{
    jugador vencedor;
    int puntaje_aux=0, puntero;

    for (int i=0; i<num_jugadores; i++)
    {
        if(puntaje_aux<puntos[i])
        {
            puntaje_aux=puntos[i];
            puntero=i;
        }
    }
    partida[puntero].partidas_ganadas++;
    for (int a=0; a<num_jugadores; a++)
    {
        if(strcmp(partida[puntero].gamertag,jugadores[a].gamertag)==0)
        {
            jugadores[a]=partida[puntero];
            cout<<"Ganador: "<<jugadores[a].nom_jugador<<" o "<<jugadores[a].gamertag<<endl;
            cout<<"---------------------------------Felicitaciones---------------------------------------"<<endl;
            break;
        }
    }

    return;
}
//-----------------------------------------------------Manejo de archivos y exportes-----------------------------------------------------------
void borrar()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void inicial(string nombres,char &inicial)

{
    inicial= nombres[0];  //uso de la inicial de la cadena para llenado
    cout<<inicial;
    return;
}
void creacion_usuario(jugador gamer)
{
    jugador aux;
    string nom;
    int contador=0;
    int cod;
    cout<<"Digite el nombre del jugador: ";
    getline(cin>>ws,nom);
    strcpy(aux.nom_jugador,nom.c_str());
    cout<<"Digite el apellido del jugador: ";
    getline(cin>>ws,nom);
    strcpy(aux.ape_jugador,nom.c_str());
    cout<<"Digite el gamertag: ";
    getline(cin>>ws,nom);
    strcpy(aux.gamertag,nom.c_str());
    archivoj = fopen("lista_jugadores.txt","r+");
    archivou = fopen("ultimo_cod.txt","r+");
    if(archivoj == NULL)
    {
        archivoj = fopen("lista_jugadores.txt","w");
        fclose(archivoj);
        archivoj = fopen("lista_jugadores.txt","r+");
    }
    if(archivou == NULL)
    {
        archivou = fopen("ultimo_cod.txt","w");
        fclose(archivou);
        archivou = fopen("ultimo_cod.txt","r+");
    }

    fread(&gamer, sizeof(gamer),1,archivoj);
    fread(&cod, sizeof(cod),1,archivou);

    while(!feof(archivoj))
    {
        contador++;
        int j=0;
        for(int i=0; i<ML; i++)
        {
            if(aux.gamertag[i]==gamer.gamertag[i])
            {
                j++;
            }
        }
        if(j==ML)
        {
            cout<<"El jugador existe, puede continuar"<<endl;
            fclose(archivoj);
            return;
        }
        fread(&gamer, sizeof(gamer),1,archivoj);

    }
    if(contador <= 20)
    {
        cout<<"Jugador no existente, se procederá a la creación del nuevo jugador:"<<endl;
        aux.codigo = cod;
        cod++;
        inicial(aux.gamertag,aux.inicial);
        fwrite(&aux,sizeof(aux),1,archivoj);
        fseek(archivou,0,SEEK_SET);
        fwrite(&cod,sizeof(cod),1,archivou);
        cout<<"Jugador creado correctamente..."<<endl;
        cout<<"Codigo del jugador:"<< aux.codigo<<endl;

    }
    else
    {
        cout<<"Memoria llena, borre jugadores para crear uno nuevo, o acceda con uno ya creado"<<endl;
    }
    _fcloseall();
    return;
}
int vectorizar(jugador jugadores[])
{
    jugador gamer;
    int liar;
    archivoj = fopen("lista_jugadores.txt","r+");
    if(archivoj == NULL)
    {
        archivoj = fopen("lista_jugadores.txt","w");
        fclose(archivoj);
        archivoj = fopen("lista_jugadores.txt","r+");
    }
    for(int i=0; !feof(archivoj); i++)
    {
        fread(&jugadores[i], sizeof(jugadores[i]),1,archivoj);
        liar=i;
    }
    return liar;
}
void borrado_usuario(jugador gamer, string gamertag)
{
    char comparar[ML]= {' '};
    bool comprobar = false;
    int j=0;
    strcpy(comparar,gamertag.c_str());
    archivoj = fopen("lista_jugadores.txt","r+");
    if(archivoj == NULL)
    {
        fclose(archivoj);
        cout<<"Archivo inexistente, no hay jugadores creados"<<endl;
    }
    else
    {
        reserva = fopen("reserva.txt","w");
        fclose(reserva);
        reserva = fopen("reserva.txt","r+");
        fread(&gamer, sizeof(gamer),1,archivoj);
        while(!feof(archivoj))
        {
            if(!(strcmp(gamer.gamertag,comparar)==0))
            {
                fwrite(&gamer, sizeof(gamer),1,reserva);
            }
            else
            {
                comprobar = true;
            }
            fread(&gamer, sizeof(gamer),1,archivoj);
        }
        _fcloseall();
        remove("lista_jugadores.txt");
        rename("reserva.txt","lista_jugadores.txt");
        if(comprobar)
        {
            cout<<"Eliminación correctamente realizada"<<endl;
        }
        else
        {
            cout<<"Archivo reescrito pero jugador a borrar no hallado"<<endl;
        }
    }
    return;
}
void desv(jugador jugadores[], int num_jugadores)
{
    jugador gamer;
    archivoj = fopen("lista_jugadores.txt","r+");
    fseek(archivoj,0,SEEK_SET);
    for(int i=0; i<num_jugadores; i++)
    {
        fwrite(&jugadores[i], sizeof(jugadores[i]),1,archivoj);
    }
    fclose(archivoj);
    return ;
}
void ver_record(jugador jugadores[], int num_jugadores, int opt)
{
    char nombre[]= "MejoresPuntajes_";
    char fin[] =".txt";
    char doc[50]="";
    int aux;
    jugador comparacion [MJ];
    jugador auxiliar;
    int pos=5;
    int con = 0;
    char date[30]; //arreglo char para la fecha
    time_t actual; //variable requerida para manejo de tiempo
    struct tm * timeinfo; //estructura requerida para el manejo de tiempo
    for (int a=0; a<num_jugadores; a++)
    {
        comparacion[a]=jugadores[a];
    }

    for (int i=0; i<num_jugadores-1; i++)
    {
        for(int j=0; j<num_jugadores-i-1; j++)
        {
            if(comparacion[j].mejor_puntaje<comparacion[j+1].mejor_puntaje)
            {
                auxiliar=comparacion[j];
                comparacion[j]=comparacion[j+1];
                comparacion [j+1]=auxiliar;
            }
        }
    }
    if(comparacion[0].mejor_puntaje==comparacion[1].mejor_puntaje)
    {
        for (int i=0; i<num_jugadores-1; i++)
        {
            for(int j=0; j<num_jugadores-i-1; j++)
            {
                if(comparacion[j].puntos<comparacion[j+1].puntos)
                {
                    auxiliar=comparacion[j];
                    comparacion[j]=comparacion[j+1];
                    comparacion [j+1]=auxiliar;
                }
            }
        }
        if(comparacion[0].puntos==comparacion[1].puntos)
        {
            for (int i=0; i<num_jugadores-1; i++)
            {
                for(int j=0; j<num_jugadores-i-1; j++)
                {
                    if((comparacion[j].partidas_ganadas/comparacion[j].juegos)<(comparacion[j+1].partidas_ganadas/comparacion[j+1].juegos))
                    {
                        auxiliar=comparacion[j];
                        comparacion[j]=comparacion[j+1];
                        comparacion [j+1]=auxiliar;
                    }
                }
            }
        }
    }


    if (opt==1)
    {
        if(num_jugadores<5)
        {
            pos=num_jugadores;
        }
        cout<<"Mejores puntajes: "<<endl;
        for (int k=0; k<pos; k++)
        {
            cout<<k+1 << ") "<<comparacion[k].nom_jugador<< "puntos totales: "<<comparacion[k].puntos<<endl;;
            cout<<"Con un mejor puntaje de partida de : "<<comparacion [k].mejor_puntaje<<endl;
        }
    }
    else
    {
        time(&actual); //lectura de tiempo
        timeinfo = localtime( &actual); //cambio de formato de tiempo
        strcpy(date,ctime(&actual)); //cambio a char para ser copiado en date
        strcat(doc,nombre);
        strcat(doc,date);
        //strcat(doc,fin);
        for(int i=40; i<44; i++)
        {
            doc[i]= fin[con];
            con++;
        }
        doc[19]=doc[23]=doc[26]=doc[35]='_';
        doc[29]=doc[32]=',';
        cout<<doc;
        records = fopen(doc, "w"); //creo el archivo desde ceros
        fclose(records);
        records = fopen(doc, "r+");
        for (int b=0; b<pos; b++)
        {
            auxiliar = comparacion [b];
            fwrite(&auxiliar, sizeof(auxiliar),1,records);
        }
        fclose(records);
    }
    return;
}
void mostrar_exportar( int num_jugadores, jugador jugadores [], int opt)
{
    char nombre[]= "InformarJugador_";
    char fin[] =".txt";
    char cod [50]="";
    char date[30]; //arreglo char para la fecha
    time_t actual; //variable requerida para manejo de tiempo
    struct tm * timeinfo; //estructura requerida para el manejo de tiempo
    int pos;
    char alias [ML];
    string gamertag;
    bool perdido=true;
    int aux;
    int con = 0;
    jugador asistente;

    while (perdido)
    {
        cout << "Digite el gamertag del jugador que quiere buscar"<<endl;
        cin >> gamertag;
        strcpy(alias,gamertag.c_str());
        for (int j=0; j<num_jugadores; j++)
        {
            if(strcmp(alias,jugadores[j].gamertag)==0)
            {
                aux=j;
                perdido=false;
                break;
            }
        }
    }

    if(opt==4)
    {
        time(&actual); //lectura de tiempo
        timeinfo = localtime( &actual); //cambio de formato de tiempo
        strcpy(date,ctime(&actual)); //cambio a char para ser copiado en date
        strcat(cod,nombre);
        strcat(cod,date);
        //strcat(cod,fin);
        for(int i=40; i<44; i++)
        {
            cod[i]= fin[con];
            con++;
        }
        cout<<"Entró"<<endl;
        cod[29]=cod[32]=',';
        cout<<cod<<endl;
        informacion = fopen(cod, "w"); //creo el archivo desde ceros
        fclose(informacion);
        informacion = fopen(cod, "r+");
        asistente=jugadores[aux];
        fwrite(&asistente, sizeof(asistente),1,informacion);
        fclose(informacion);
    }
    else
    {
        cout<<"El jugador se llama "<<jugadores[aux].nom_jugador<<", con apellido "<<jugadores[aux].ape_jugador;
        cout<<", el gamertag "<<jugadores[aux].gamertag<<endl;
        cout<<"Tiene un puntaje maximo en el juego de: "<<jugadores[aux].mejor_puntaje<<endl;
        cout<<"Tiene un puntaje acumulado de: "<<jugadores[aux].puntos<<endl;
        cout<<"Tiene una tasa de victorias de: "<<100*jugadores[aux].partidas_ganadas/jugadores[aux].juegos<<"%"<<endl;
        cout<<"Victorias: "<<jugadores[aux].partidas_ganadas<<endl;
        cout<<"Partidas jugadas: "<<jugadores[aux].juegos<<endl;
    }

    return;
}
//-------------------------------------------------------Finalización de delcaración de funciones----------------------------------------------------------
