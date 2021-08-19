# -*- coding: UTF-8 -*-
#-----------IMPORTACIONES Y LIBRERÍAS----------------------------------
import numpy as np
#Comandos trabajados: savetxt,array,loadtxt
import os as o 
#Comandos trabajados: system,name,remove,getcwd
from time import sleep
#---------------parte gráfica de la jugabilidad-------------------------
def tablero_inicial(tablero,filas,columnas):
    for y in range(filas):
        if y%2 == 0:
            for x in range(columnas):
                if float(x%2) ==0:
                    tablero[y][x]='+'
                else:                
                    tablero[y][x]=' '
        else:
            for x in range(columnas):                
                tablero[y][x]=' '
def mostrar_tablero(tablero,filas,columnas):  #corregir interfaz gráfica
    auxc = int((columnas + 1)/2)
    auxs = int((filas + 1)/2)
    orden = 0
    if auxs<9:
        print("  ",end="")
    else:
        print("   ",end="")
    for n in range(auxc):
        if auxc>9:
            print(n+1, end="    ")
        else:
            print(n+1,end="   ")
    print()
    for i in range(filas):
        if float(i%2)== 0:
            if auxs>9:
                print(orden+1, end="  ")
            else:
                print(orden+1,end=" ")
            orden+=1
        else:
            if auxs>9:
                print("  ", end="  ")
            else:
                print(" ",end=" ")
            orden+=1
        for j in range(columnas):
            print(tablero[i][j],end=" ")
        print()
def despliegue_menu():
    print("+","-"*21,"+")
    print("+ 1) Nueva Partida    +")
    print("+ 2) Continuar Juego  +")
    print("+ 3) Jugadores        +")
    print("+ 4) Estadísticas     +")
    print("+ 5) Salir            +")
    print("+","-"*21,"+")

#------------------------condiciones de juego y configuración interna--------------------
def valido(x1,x2,y1,y2):
    conf1 = (x1-x2)**2
    conf2 = (y1-y2)**2
    if conf1 >= 1 and conf2 >= 1:
        return False
    else:
        return True
def libre(a,b):
    if tablero[a][b]==' ':
        return True
    else:
        return False
def completo(x,y,fila,columna,tablero):
    pos = 0
    # 1: / 2: / 3: /4: / 5: / 6:
    if tablero[x][y]=='-' and x!=0 and x!=fila:
        if (tablero[x+1][y-1]=='|' and tablero[x+1][y+1]=='|' and tablero[x+2][y]=='-')and(tablero[x-1][y-1]=='|' and tablero[x-1][y+1]=='|' and tablero[x-2][y]=='-'):
            pos = 5
        elif tablero[x+1][y-1]=='|' and tablero[x+1][y+1]=='|' and tablero[x+2][y]=='-':
            pos = 2
        elif tablero[x-1][y-1]=='|' and tablero[x-1][y+1]=='|' and tablero[x-2][y]=='-':
            pos = 1
    elif tablero[x][y]=='-' and x==0:
        if tablero[x+1][y-1]=='|' and tablero[x+1][y+1]=='|' and tablero[x+2][y]=='-':
            pos = 2
    elif tablero[x][y]=='-' and x==fila:
        if tablero[x-1][y-1]=='|' and tablero[x-1][y+1]=='|' and tablero[x-2][y]=='-':
            pos = 1
    elif tablero[x][y]=='|' and y!=0 and y!=columna:
        if (tablero[x][y+2]=='|' and tablero[x+1][y+1]=='-' and tablero[x-1][y+1]=='-') and (tablero[x][y-2]=='|' and tablero[x+1][y-1]=='-' and tablero[x-1][y-1]=='-'):
            pos = 6
        elif tablero[x][y+2]=='|' and tablero[x+1][y+1]=='-' and tablero[x-1][y+1]=='-':
            pos = 3
        elif tablero[x][y-2]=='|' and tablero[x+1][y-1]=='-' and tablero[x-1][y-1]=='-':
            pos = 4
    elif tablero[x][y]=='|' and y==0:
        if tablero[x][y+2]=='|' and tablero[x+1][y+1]=='-' and tablero[x-1][y+1]=='-':
            pos = 3
    elif tablero[x][y]=='|' and y == columna:
        if tablero[x][y-2]=='|' and tablero[x+1][y-1]=='-' and tablero[x-1][y-1]=='-':
            pos = 4
    return pos
def cuadro_para(turno,jugadores,x,y,pos,tablero,puntos):
    print(jugadores[turno]," ha completado un cuadro")
    nombre = jugadores[turno]
    inicial = nombre[0]
    if pos == 1:
        tablero[x-1][y]= inicial
    elif pos == 2:
        tablero[x+1][y]= inicial
    elif pos == 3:
        tablero[x][y+1]= inicial
    elif pos == 4:
        tablero[x][y-1]= inicial
    elif pos == 5:
        tablero[x+1][y]= inicial
        tablero[x-1][y]= inicial
        puntos[turno]+=1
    elif pos == 6:
        tablero[x][y+1]= inicial
        tablero[x][y-1]= inicial
        puntos[turno]+=1
    print("El jugador ha ganado otro turno por completar un cuadro")
    return
def lista_jugadores(jugadores,num,puntos):
    for i in range (num):
        puntos[i]=0
        print("Jugador #",i,": ")
        nombre = input("Digite el nombre del jugador: ")
        if i>0:
            for j in range (i):
                aux_nom = jugadores[j]
                while nombre[0] == aux_nom[0]:
                    nombre = input("Digite otro nombre para el jugador con inicial diferente")
            jugadores[i]= nombre
        else:
            jugadores[0]= nombre  
def clear():
    if o.name == "nt":
        o.system("cls")
    else:
        o.system("clear")

def guardado_tablero(filas,columnas,tablero,ultimo,jugadores,puntos,num):
    txt = " ultimo_juego.txt"
    line1 = str(filas) + " " + str(columnas) + " " + str(ultimo) + " " + str(num)+ "\n"
    line2 = ""
    line3 = ""
    for j in range(num):
        line2 += str(jugadores[j]) + " "
        line3 += str(puntos[j]) + " "
    copia_pausa = [[0 for i in range(columnas)]for j in range(filas)]
    for x in range(filas):
        for y in range(columnas):
            copia_pausa[x][y]= ord(tablero[x][y])
    guardado = np.array(copia_pausa)
    if txt in o.listdir(".") and o.path.isfile(txt) :
        o.remove(o.getcwd + "/" + txt)
        print("Se ha reemplazado un archivo de partida pausada")
    np.savetxt("ultimo_juego.txt",guardado)
    print("Tablero guardado con éxito")
    datos = open("datos_juego.txt","w")
    #orden de guardado de datos: fila,columna,turno,num_jugadores,jugadores,puntos
    datos.write(line1)
    datos.write(line2)
    datos.write("\n")
    datos.write(line3)
    print("Datos guardados correctamente")
    datos.close()

def carga_datos(jugadores,puntos,tablero):
    matriz = "ultimo_juego.txt"
    txt = "datos-juego.txt"
    if not (txt in o.listdir(".") and o.path.isfile(txt)):
        print("Error accediendo a los registros")
        fila = columna = turno = num = -1
        return fila,columna,turno,num
    recarga = open(txt,"r")
    cargados = np.loadtxt(matriz)
    line1 = recarga.readline()
    line1 = line1.split()
    fila = int(line1[0])
    columna = int(line1[1])
    turno = int(line1[2])
    num = int(line1[3])
    line2 = recarga.readline()
    line2 = line2.split()
    line3 = recarga.readline()
    line3 = line3.split()
    for j in range(num):
        jugadores[j]= line2[j]
        puntos[j]= int(line3[j])
    for x in range(fila):
        for y in range(columna):
            tablero[x][y]=chr(cargados[x][y])
    recarga.close()
    o.remove(o.getcwd()+ matriz)
    o.remove(o.getcwd()+ txt)
    print("Datos de partida cargados correctamente, recuerde al cargarlos no se podrá jugar la misma partida posteriormente")
    return fila,columna,turno,num

#------------------------jugabilidad y desarrollo---------------------------------------
def tu_turno(fila,columna,turno,tablero,jugadores,puntos,num):
    print("Es el turno del jugador: ",jugadores[turno])
    x=y=x1=x2=y1=y2=con=0
    while not(valido(x1,x2,y1,y2) and libre(x,y)):
        if con>0:
            print("Movimiento inválido,vuelva a digitar por favor:")
        print("Recuerde que para pausar el juego debe oprimir -1")
        x1=int(input("Digite la coordenada x1: "))
        if x1 == -1:
            guardado_tablero(fila, columna, tablero, turno, jugadores, puntos, num)
            turno = -1
            return turno
        y1=int(input("Digite la coordenada y1: "))
        if y1 == -1:
            guardado_tablero(fila, columna, tablero, turno, jugadores, puntos, num)
            turno = -1
            return turno
        x2=int(input("Digite la coordenada x2: "))
        if x2 == -1:
            guardado_tablero(fila, columna, tablero, turno, jugadores, puntos, num)
            turno = -1
            return turno
        y2=int(input("Digite la coordenada y2: "))
        if y2 == -1:
            guardado_tablero(fila, columna, tablero, turno, jugadores, puntos, num)
            turno = -1
            return turno
        x = x1+x2-2
        y = y1+y2-2
        con+=1
    if x%2 == 0 and y%2!=0:
        tablero[x][y]='-'
    else:
        tablero[x][y]='|'
    pos = completo(x,y,fila,columna,tablero)
    if pos!= 0:
        puntos[turno]+=1
        cuadro_para(turno,jugadores,x,y,pos,tablero,puntos)
        sleep(1)
    else:
        turno +=1
        if turno >= num:
            turno = 0
    return turno

def disponibles(tablero,fila,columna):
    for i in range(fila):
        for j in range(columna):
            if tablero[i][j]==' ':
                return True
    return False
#------------------------DESARROLLO DEL MAIN CUADRITO---------------------------------
jugadores=[' ',' ',' ',' ',' ']
puntos=[0,0,0,0,0]
filas = columnas = 0
turno = 0
seguir = True
print("Bienvenido al juego de cuadrito de Daniel López, versión en Python")
despliegue_menu()
opt = int(input("Digite su opción: "))
clear()
if opt==1:
    print("Cargando tablero vacio y espacio de juego,¿está listo?")
    filas = int(input("Digite el número de filas: "))
    columnas = int(input("Digite el número de columnas: "))
    filas = filas*2-1
    columnas = columnas*2-1
    tablero = [['°' for i in range(25)]for i in range (25)]
    num= int(input("Digite el número de jugadores: "))
    lista_jugadores(jugadores,num,puntos)
    print("El juego empieza: ")
    tablero_inicial(tablero,filas,columnas)
    while(True):
        mostrar_tablero(tablero,filas,columnas)
        turno = tu_turno(filas,columnas,turno,tablero,jugadores,puntos,num)
        if turno == -1:
            print("Se ha detenido el juego, gracias por jugar")
            break
        seguir = disponibles(tablero,filas, columnas)
        if not seguir:
            print("No ha movimientos disponibles, el juego ha terminado")
            break
        clear()
    print()
elif opt==2:
    print("Buscando en el historial de partida, recuerde verificar que el guardado del juego se realizo correctamente")
    filas,columnas,turno,num = carga_datos(jugadores, puntos, tablero)
    if filas != -1 and num !=-1:
        print("El juego continua: ")
        while(True):
            mostrar_tablero(tablero,filas,columnas)
            turno = tu_turno(filas,columnas,turno,tablero,jugadores,puntos,num)
            if turno == -1:
                print("Se ha detenido el juego, gracias por jugar")
                break
            seguir = disponibles(tablero,filas, columnas)
            if not seguir:
                print("No ha movimientos disponibles, el juego ha terminado")
                break
            clear()
    else:
        print("Volviendo al menú principal...")
elif opt==3:
    print("A continuación se muestra las opciones disponibles para jugador")
elif opt==4:
    print("Cargando datos y estadísticas")
print("-"*15)
print("Gracias por venir, espero haya disfrutado del juego")


