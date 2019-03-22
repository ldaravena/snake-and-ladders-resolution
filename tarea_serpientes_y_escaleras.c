/*  Tarea Computacional de Matemáticas Discretas 2018-2
 *  "Escaleras y Serpientes"
 *
 *  Ingeniería Civil Informática
 *  Universidad de Concepción
 *
 *  Leonardo Aravena
 *  Vicente Coopman
 *  Felipe Henríquez
 *
 * 23 de noviembre de 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef struct{ //Estructura que contiene los datos para cada casilla.

    /*Arreglo para identificar las salidas que tiene cada casilla según el número de dado que salga.
    'salida[0]' representa el número de la casilla al llegar a ella. Normalmente es el mismo número de la casilla, pero si es
    una serpiente o escalera indica a donde lleva esa serpiente o escalera.*/
    int salida[7];

    //Indica si se ha visitado esa casilla, en el funcionamiento actual del algoritmo no se ocupa
    int visitado;

    //'padre' es el número de la casilla desde donde se llegó con la menor distancia.
    int padre;

    //'distancia' es el número de lanzamientos del dado requeridos para llegar a la casilla.
    int distancia;

    //'dado' es el número del dado con el que se llegó a la casilla desde la casilla 'padre'.
    int dado;

} Casilla;

//Estructura de utilidad para la cola donde cada casilla es un nodo
typedef struct Nodo{

    int casilla;
    struct Nodo * siguiente;

}tipoNodo;

typedef tipoNodo *pNodo;

typedef tipoNodo *Cola;


void agregar(pNodo *primero, pNodo *ultimo, int v){

    pNodo nuevo;

    nuevo = (pNodo)malloc(sizeof(tipoNodo));

    nuevo->casilla = v;

    nuevo->siguiente = NULL;

    if(*ultimo){

        (*ultimo)->siguiente = nuevo;
    }

    (*ultimo) = nuevo;

    if(!*primero){

        (*primero) = nuevo;
    }
}

int leer(pNodo *primero, pNodo *ultimo){

    pNodo nodo;

    int v;

    nodo = *primero;

    if(!nodo) return (-1);

    *primero = nodo->siguiente;

    v= nodo->casilla;

    free(nodo);

    if(!*primero){

        *ultimo=NULL;
    }

    return v;
}

void llegadaMeta(Casilla * tablero, int dim){
    pNodo primero = NULL;
    pNodo ultimo = NULL;

    /*  Función inspirada en el Algoritmo de Dijkstra para encontrar el menor número de lanzamientos para llegar a la meta  */

    int i, j, m, n, u;

    u=dim-1; //Meta (última casilla).

    tablero[0].distancia = 0; //La distancia de la casilla de partida es cero.

    /*  En este ciclo se revisan todas las casillas, menos la meta, y sus casillas de llegada según el número del dado que salga, del 1 al 6.
        Luego compara los números de lanzamientos (distancia) que hay hacia esa casilla y si tiene una menor distancia la reemplaza, siendo esta casilla
        el nuevo padre. También se completa el valor del número del dado que conduce a esa casilla  */
    i=0;

    agregar(&primero, &ultimo, 0);

    do{
        //CORRECCIÓN DEL ERROR DETECTADO EL 16/11/18 (se agrega la condición 'i==u')
        if(tablero[i].visitado == 1 || i==u){

            i=leer(&primero, &ultimo);
            continue;
        }

        tablero[i].visitado = 1;

        for(j=1; j<7; ++j){

            m = tablero[i].salida[j];

            if(tablero[m].distancia > (tablero[i].distancia + 1)){

                tablero[m].distancia = (tablero[i].distancia +1);

                tablero[m].padre = i;
                tablero[m].dado = j;

                agregar(&primero, &ultimo, m);
            }
        }

        i=leer(&primero, &ultimo);

    }while(i!=-1);
}

int main(){

    int n;      //Longitud del tablero.
    int m;      //Número de serpientes y escaleras.
    int dim;    //Dimensión del tablero (n*n)

    int meta;   //Indica si existe o no llegada a la meta.
    int u;      //Número de la última casilla (la meta).
    int i, j, k, l, p, q, r, s; //Variables auxiliares.

    int v;     //Variable para crear el tamaño del arreglo que se utiliza para entregar la información de como se llega a la meta.

    int bandera;
    int exit = 0;

    FILE *f; // puntero archivo
    char c; // character aux;
    char file_name[30];

    char salida[]="salir"; //String para salir del programa.

    printf("\nTarea Matematicas Discretas(2018-2): 'Escaleras y Serpientes'\nLeonardo Aravena, Vicente Coopman, Felipe Henriquez.\n\nIngrese el nombre completo del archivo para ejecutar el programa o escriba 'salir' para salir:\n\n");

    while(!exit){

        while(1){

            //Lee el nombre del archivo.
            scanf("%s",file_name);

            //Si se escribió "salir" se termina el programa.
            if(!strcmp(salida,file_name)){

                exit=1;
                break;
             }

            f=fopen(file_name,"r");
            if(f == NULL) printf("Archivo no encontrado, intente nuevamente:\n\n");
            if(f != NULL) break;
        }

        if(exit) break;

        fscanf(f,"%d", &n); //Se lee el primer valor.

        dim = n*n;  //Se calcula la dimensión.

        u = dim-1;  //Última casilla.

        //Se crea en memoria dinámica el arreglo que contiene la información del tablero de juego.
        Casilla *tablero = (Casilla *)malloc(dim*sizeof(Casilla));

        //Arreglo para guardar todas las casillas que no tengan como salida una serpiente o escalera.
        int casillasAlcanzables[dim];

        /*  En este ciclo se completan los valores iniciales del tablero, sin serpientes ni escaleras.
            'salida[0]' corresponde al mismo número de la casilla, y el resto de las llegadas es la suma del
            número de la casilla más los números del dado.  */
        for(i=0; i<dim; ++i){

            tablero[i].salida[0]=i;

            for(j=1; j<7; ++j){

                //Si la llegada es menor a la última casilla, la llegada es la suma de la casilla más los números del dado.
                if((i+j)< u){

                    tablero[i].salida[j]= i+j;
                }

                /*  Si la llegada es igual o mayor a la última casilla, la llegada es la última casilla.
                    No se puede avanzar más que a la última casilla */
                else{

                    tablero[i].salida[j]= u;
                }
            }

            /*  Cada casilla se marca como no vistada, sin padre (-1), a la distancia máxima (el máximo número entero menos uno)
                y con el número de dado que se llega a esa casilla como cero    */
            tablero[i].visitado = 0;
            tablero[i].padre = -1;
            tablero[i].distancia = INT_MAX -1;
            tablero[i].dado = 0;
        }

        tablero[0].padre = 0;

        //Para la última casilla se consideran los valores de salida como (-1):
        for(j=1; j<7; ++j){

            tablero[u].salida[j]= -1;
        }

        //Se leen la cantidad de serpientes y escaleras:
        fscanf(f,"%d", &m);

        /*  En este ciclo se completan los valore de las serpientes y escaleras.
            Se leen los valores y se toma el primer valor como el número de casilla y el segundo como 'salida[0]'*/
        for(i=0; i<m; ++i){

            fscanf(f,"%d %d",&p, &q);

            tablero[p].salida[0] = q;

            /*  Se completan los valores de salida con los mismos valores que tiene la casilla a la que conduce
                la serpiente o escalera */

            for(j=1; j<7; ++j){

                tablero[p].salida[j] = tablero[q].salida[j];
            }
        }

        /*  Este ciclo reemplaza las casillas a las que se llegan en caso de que existan serpientes o escaleras.
            En ese caso se coloca como salida la casilla que está al final de la serpiente o escalera*/

        for(i=0; i<u; ++i){

            for(j=1; j<7; ++j){

                //Número de la casilla a la que se llega cuando el dado da el número 'j'
                n = tablero[i].salida[j];

                /*  Casilla a la que se llega sin moverse, en general es la misma casilla, pero cambia en el caso de Serpientes
                    o escaleras */
                m = tablero[n].salida[0];

                tablero[i].salida[j] = m;
            }
        }

        /*  Se llama a la función para calcular la menor distancia a la meta (si es que existe). Se le envía el puntero
            al arreglo dinámico, la dimensión del tablero y la casilla desde la que se parte */

        llegadaMeta(tablero,dim);

        /*  Si luego de aplicar el algoritmo para encontrar la menor distancia a la última casilla esta tiene un padre distinto
            a (-1) significa que se encontró como llegar a la meta */
        if(tablero[u].padre != -1){

            /*  La variable 'v' se utiliza para la dimensión del arreglo que entrega la información de llegada a la meta.
                Por eso su valor es la distnacia que existe a la meta   */
            v = tablero[u].distancia;

            meta=1; //Se indica que la llegada a la meta existe.

        }
        //  Si el padre de la última casilla sigue siendo (-1) no existe llegada la meta
        else{

            printf("\n1) No es posible alcanzar la meta.\n");
            meta = 0;
        }

        //Se crea el arreglo para entregar los resultados.
        int resultados[v][4];

        j=u; //La variable 'j' toma el valor de la casilla de la meta.

        /*  En este ciclo se completan los valores de la secuencia de lanzamientos mínima requerida para llegar a la meta.
            Se completan desde la última casilla hasta el inicio    */

        for(i=v-1; i>=0 && meta; --i){

            //El primer valor que se guarda es el número de la casilla.
            resultados[i][0] = j;

            //Cuantos lanzamientos se requieren para llegar a esta casilla.
            resultados[i][1] = tablero[j].distancia;

            //Desde donde llegó esta casilla (quien es su 'padre')
            resultados[i][2] = tablero[j].padre;

            //Con que número de dado se llegó a esta casilla.
            resultados[i][3] = tablero[j].dado;

            //La siguiente casilla a revisar es el 'padre' de esta casilla.
            j = tablero[j].padre;
        }

        //Ciclo para imprimir en pantalla la secuencia de lanzamientos mínimo que se requiere para llegar a la meta.
        for(i=0; i<v && meta; ++i){

            if(i==0) printf("\n1) C");

            else printf("c");

            printf("on el lanzamiento %3d del dado, con el valor %3d se parte desde la casilla %3d hasta la casilla %3d", resultados[i][1], resultados[i][3], resultados[i][2], resultados[i][0]);

            if(i<v-1) printf(", luego");

            if(i==v-1) printf(", llegando a exitosamente a la meta.");

            printf("\n");
        }

        //Ciclo para obtener las casillas alcanzables.
        for(i=0, k=0; i<dim; ++i){

            //Son alcanzables las casillas cuya distancia es menor a (INT_MAX -1). La casilla de partida se considera alcanzable.

            if(tablero[i].distancia!= (INT_MAX -1)){

                casillasAlcanzables[k]=i;
                ++k;
            }
        }
        // arreglo destinado para guardar las casillas que pueden llegar a la meta
        int casillaLlegaMeta[k];
        // ciclo el cual llena de -1 el arreglo de casillaLlegaMeta
        for(i=0; i<k; ++i){
            casillaLlegaMeta[i]= -1;
        }
        //Por definicion, debido a que la meta llega a la meta.
        casillaLlegaMeta[0]=u;
        r=1; // indice auxiliar
        // ciclo que revisa si las casillas llegan a la meta, desde la meta a la casilla de inicio
        for(i=u-1; i>=0; --i){
            s=0;
            bandera=0;
            // ciclo para evitar repetidos, esta condicion del while es para que las escaleras no se comprueben
            while(casillaLlegaMeta[s]!=-1){
              if(tablero[i].salida[0]==casillaLlegaMeta[s]){
                bandera=1;
                break;
              }
              s++;
            }
            if(bandera) continue;
            // ciclo que revisa cada casilla de salida del tablero, si es alguna coincide con alguna casilla que llega a la meta, se guarda la posicion de origen y se rompe el ciclo.
            for(j=1; j<7; ++j){

              for(l=0; l<k && casillaLlegaMeta[l]!=-1; ++l){

                if(tablero[i].salida[j] == casillaLlegaMeta[l]){
                    casillaLlegaMeta[r]= tablero[i].salida[0];
                    r++;
                    bandera=1;
                    break;
                }
            }

            if(bandera)break;
          }
      }
        //Se imprimen las casillas alcanzables.

        printf("\n2) Casillas Alcanzables:\n");

        for(i=0; i<k; ++i){

            printf("%d",casillasAlcanzables[i]);

            if(i<(k-1)) printf(", ");

            else printf(".\n");
        }

        r=k-1;
        // ciclo que cuenta la cantidad de casillas que llegan a la meta.
        while(casillaLlegaMeta[r]==-1){

            r--;
        }
        // ciclo que borra las casillas que son alcanzables y que llegan a la meta,
        // para asi obtener las casillas que son alcanzables y que no llegan a la meta.
        for(i=0; i<k; ++i){
          for(j=0;j<r+1;j++){
            if(casillasAlcanzables[i]==casillaLlegaMeta[j]){
                casillasAlcanzables[i]=-1;
            }
          }
        }

        bandera=0;
        r=0;
        // ciclo que cuenta las casillas que son alcanzables, pero que no llegan a la meta.
        for(i=0;i<k;++i){
          if(casillasAlcanzables[i]!=-1)
            r++;
        }
        // se define una matriz, filas: la cantidad de casillas alcanzables y que no llegan a la meta
        // columnas: cantidad suficiente para guardar las casillas por las cuales se pasa para llegar desde el inicio a la casilla en cuestion.
        int llegarCasillas[r][u/2];
        int t=r; // variable auxiliar
        for(i=0;i<r;i++){
          for(j=0;j<u/2;j++){
            llegarCasillas[i][j]=-1;
          }
        }

        r=0;
        // Solo nos sirven las casillas iniciales, si es que hay un conjunto de casillas seguidas las cuales no llegan a la meta y no son Alcanzables

        for(i=0;i<k;++i){
            // si la casilla=-1 continuamos el for
            if(casillasAlcanzables[i]==-1 && !bandera){
                continue;
            }
            // si es que se llega a una distinta, se guarda la posicion con l
            else if(casillasAlcanzables[i]!=-1 && !bandera){
                bandera=1;
                l=i;
            }
            // si hay casillas seguidas, continuamos el for
            else if(casillasAlcanzables[i]!=-1 && bandera){
                continue;
            }
            // si se llega a una igual a -1 se guarda la informacion de como llegar desde el inicio hasta esa casilla
            else if (casillasAlcanzables[i]==-1 && bandera){
                bandera=0;
                j=1;
                llegarCasillas[r][0]=casillasAlcanzables[l];
                s=casillasAlcanzables[l];
                // ciclo que se ejecuta hasta cuando se llega a la casilla de inicio y guarda la informacion en la matriz
                while(1){
                  if(tablero[s].distancia!=0){
                    llegarCasillas[r][j]=tablero[s].padre;
                    s=llegarCasillas[r][j];
                    j++;
                  }
                  else
                    break;
                }
                r++;
            }
        }
        //Se imprime la informacion obtenida de como llegar a las casillas alcanzables pero que no pueden llegar a la meta
        printf("\n3) ");
        if(meta && t!=0){
          for(i=0;i<t && llegarCasillas[i][0]!=-1; i++){
            printf("Para llegar a la casilla %d, desde la cual no se puede llegar a la meta, tenemos: \n", llegarCasillas[i][0]);
            j=u/2-1;
            while(llegarCasillas[i][j]==-1){
              j--;
            }
            j--;
            for(;j>=0 && llegarCasillas[i][j+1]!=-1;j--){
              printf("Con el lanzamiento del dado %d , sacando %d con el dado, ", tablero[llegarCasillas[i][j]].distancia,tablero[llegarCasillas[i][j]].dado);
              printf("se llega a %d desde %d",llegarCasillas[i][j],llegarCasillas[i][j+1]);
              printf("\n");
            }
            printf("\n");
          }
        }
        else if(!meta){

          printf("Todas las casillas no llegan a la meta\n");

        }
        else{
          printf("Todas las casillas alcanzables llegan a la meta\n");
        }
        printf(" ////// FIN DE LA EJECUCIÓN ////// \n\n\n");

        printf("Ingrese el nombre del siguiente archivo o escriba 'salir' para salir:\n\n");

        fclose(f); //Cierra el archivo.

        free(tablero); //Se libera la memoria dinámica ocupada.
    }

    return(0);
}
