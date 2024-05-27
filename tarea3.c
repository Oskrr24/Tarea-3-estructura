#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>
#include "tdas/map.h"
#include "tdas/stack.h"
#include "tdas/queue.h"

typedef struct {
    int square[3][3]; //Esta matriz representa el tablero del 8 puzzle
    int x;  // La posicion x del numero 0
    int y;  // La posicion y del numero 0
    List* actions; //La lista de acciones que se deben realizar para llegar al estado objetivo
    List* Ady;
    int depth;
    int steps; 
    int action;
} State;

//Función determina cuántas filas y columnas debe desplazarse para alcanzar su ubicación correcta
int distancia_L1(State* state) {
    int ev=0;
    int k=1;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++){
            int val=state->square[i][j];
            if (val==0) continue;
            int ii=(val)/3;
            int jj=(val)%3;
            ev+= abs(ii-i) + abs(jj-j);
        }
    return ev;
}

//Esta funcion imprime el estado actual del puzzle
void imprimirEstado(const State *estado) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (estado->square[i][j] == 0)
                printf("x "); 
            else
                printf("%d ", estado->square[i][j]);
        }
        printf("\n");
    }
}

//Esta función determina si el estado actual del puzzle es igual al estado objetivo
int isFinal(State* state){
    int goal[3][3]= {
    {0,1,2},
    {3,4,5},
    {6,7,8}
    };
    for(int i=0; i<3 ;i++){
        for(int j = 0; j < 3; j++){
            if(state->square[i][j] != goal[i][j]) return 0;
        }
    }
    return 1;
}

//Esta función crea un nuevo estado que es el resultado de aplicar la acción especificada al estado actual
State* transition(State* state, int action){
    State* newState = malloc(sizeof(State));
    newState->x = newState->y = -1;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            newState->square[i][j] = state->square[i][j];
        }
    }
    int x = state->x;
    int y = state->y;
    newState->actions = list_create();
    int* action_ptr = (int*)list_first(state->actions);
    while(action_ptr != NULL)
        {
            list_pushBack(newState->actions, action_ptr);
            action_ptr = (int*)list_next(state->actions);
        }
    list_pushBack(newState->actions, &action);
    newState->action = action;

    if (action == 1) { // arriba
        if (x > 0) {
            newState->square[x][y] = newState->square[x - 1][y];
            newState->square[x - 1][y] = 0;
            newState->x = x - 1;
            newState->y = y;
        }
    } else if (action == 2) { // abajo
        if (x < 2) {
            newState->square[x][y] = newState->square[x + 1][y];
            newState->square[x + 1][y] = 0;
            newState->x = x + 1;
            newState->y = y;
        }
    } else if (action == 3) { // izquierda
        if (y > 0) {
            newState->square[x][y] = newState->square[x][y - 1];
            newState->square[x][y - 1] = 0;
            newState->x = x;
            newState->y = y - 1;
        }
    } else if (action == 4) { // derecha
        if (y < 2) {
            newState->square[x][y] = newState->square[x][y + 1];
            newState->square[x][y + 1] = 0;
            newState->x = x;
            newState->y = y + 1;
        }
    }

    if (newState->x  < 0 || newState->x  >= 3 || newState->y < 0 || newState->y >= 3) {
        free(newState);
        return NULL; 
    }
    return newState;
}

//Esta función va generando nuevos estados provando las acciones posibles llamando a la función transition
//y luego ingresando el estado creado en la lista de estados adyacentes
void get_adyacents_node(State* state, List* Ady){
    for(int i=1; i<=4; i++){
        State* newState = transition(state, i);
        if(newState != NULL){
            list_pushBack(Ady, newState);
        }    
    }
}

//Esta función nos ayuda a crear un mapa que contiene los estados adyacentes de cada estado
int state_equal(void *a, void *b) {
    State *state1 = (State*)a;
    State *state2 = (State*)b;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (state1->square[i][j] != state2->square[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

//Esta funcion imprime los estados el paso a paso el cual se llega al estado final en cada iteracion de imprime 
// el paso en el que se esta, la acción que se produce y el estado del puzzle
void imprimirFinal(State* state, Map* parentMap, int cont){
    printf("\n¡Solución encontrada!\n");
    printf("Cantidad de iteraciones realizadas: %d\n\n", cont);
    List* path = list_create();
    while (state != NULL) {
        list_pushFront(path, state);
        MapPair* pair = map_search(parentMap, state);
        state = pair ? pair->value : NULL;
    }
    State* step = list_first(path);
    int paso = 1;
    while (step != NULL) {
        if (paso == 1) {
            printf("Estado inicial:\n");
            imprimirEstado(step);
            step = list_next(path);
            puts("");
        }
        printf("Paso N°%d:\n", paso);
        printf("Acción: ");
        switch(step->action)
            {
                case 1:
                    printf("Arriba\n");
                    break;
                case 2:
                    printf("Abajo\n");
                    break;
                case 3:
                    printf("Izquierda\n");
                    break;
                case 4:
                    printf("Derecha\n");
                    break;
            }
        imprimirEstado(step);
        step = list_next(path);
        puts("");
        paso++;
    }
    list_clean(path);
}

/*La función dfs realiza una búsqueda en profundidad en el estado inicial del puzzle hasta una profundidad 
máxima especificada por maxDepth. La función busca alcanzar un estado objetivo final y utiliza una pila 
(stack) para gestionar los estados no explorados y un mapa (map) para registrar los estados visitados y sus 
predecesores.*/
void dfs(State* initial, int maxDepth) {
    Stack* stack = stack_create();
    Map* parentMap = map_create(state_equal);
    stack_push(stack, initial);
    map_insert(parentMap, initial, NULL);
    int cont = 0;
    while (list_size(stack) != 0) {
        State* state = stack_pop(stack);
        int depth = state->depth;

        if (isFinal(state)) {
            imprimirFinal(state,parentMap,cont);
            break;
        }
        
        if (list_size(state->actions) >= maxDepth) {
            continue;
        }

        List* Ady = list_create();
        get_adyacents_node(state, Ady);
        State* newState = (State*)list_first(Ady);
        while (newState != NULL) {
            newState->depth = depth + 1;
            if (map_search(parentMap, newState) == NULL) {
                stack_push(stack, newState);
                map_insert(parentMap, newState, state);
            }
            newState = (State*)list_next(Ady);
        }
        list_clean(Ady);
        cont++;
    }
    
    if (list_size(stack) == 0) {
        printf("No se encontró solución\n");
    }
    
    stack_clean(stack);
    map_clean(parentMap);
    free(parentMap);
}

/*La función bfs implementa una búsqueda en amplitud para el 8-puzzle. Utiliza una cola para explorar los 
estados en orden de nivel y un mapa para evitar visitar estados ya explorados y para poder reconstruir el 
camino hacia el estado final una vez encontrado. Si se encuentra el estado final, se imprime la solución. Si 
no, se indica que no se encontró solución tras agotar la búsqueda.*/
void bfs(State* initial) {
    Queue* queue = queue_create();
    Map* parentMap = map_create(state_equal);
    queue_insert(queue, initial);
    map_insert(parentMap, initial, NULL);
    int cont = 0;
    while (list_size(queue) != 0) {
        State* state = queue_remove(queue);
        int depth = state->depth;
        if (isFinal(state)) {
            imprimirFinal(state,parentMap,cont);
            break;
        }

        List* Ady = list_create();
        get_adyacents_node(state, Ady);
        State* newState = (State*)list_first(Ady);
        while (newState != NULL) {
            newState->depth = depth + 1;
            if (map_search(parentMap, newState) == NULL) {
                queue_insert(queue, newState);
                map_insert(parentMap, newState, state);
            }
            newState = (State*)list_next(Ady);
        }
        list_clean(Ady);
        cont++;
    }
    map_clean(parentMap);
    
    if (list_size(queue) == 0) {
        printf("No se encontró solución\n");
    }
    queue_clean(queue);
}

/*La función best_first implementa una búsqueda de mejor primero para el 8-puzzle. Utiliza una cola de prioridad para seleccionar el siguiente estado a explorar en función de su heurística (distancia L1). Si se encuentra el estado final, se imprime la solución. Si no, se indica que no se encontró solución tras agotar la búsqueda. La función gestiona estados adyacentes, asegura que no se repiten estados ya visitados y mantiene un mapa de predecesores para poder reconstruir el camino hacia el estado final.*/
void best_first(State* initial) {
    Heap* heap = heap_create(); 
    Map* parentMap = map_create(state_equal); 
    map_insert(parentMap, initial, NULL); 
    heap_push(heap, initial, distancia_L1(initial));

    int cont = 0; 

    while (heap_top(heap) != NULL) {
        State* state = heap_top(heap);
        heap_pop(heap); 
        if (isFinal(state)) { 
            imprimirFinal(state,parentMap,cont);
            return;
        }

        List* Ady = list_create();
        get_adyacents_node(state, Ady);

        State* bestState = NULL;
        int bestCost = 30;

        State* newState = (State*)list_first(Ady);
        while (newState != NULL) {

            if (map_search(parentMap, newState) == NULL) { 
                int cost = distancia_L1(newState);
                if (cost <= bestCost) {
                    if (bestState != NULL) {
                        free(bestState); 
                    }
                    bestState = newState;
                    bestCost = cost;
                } else {
                    free(newState); 
                }
            } 
            else {
                free(newState); 
            }
            newState = (State*)list_next(Ady);
        }

        if (bestState != NULL) {
            heap_push(heap, bestState, bestCost); 
            map_insert(parentMap, bestState, state); 
        }

        list_clean(Ady); 
        cont++; 
    }

    if (heap_top(heap) == NULL) {
        printf("No se encontr처 solución\n");
    }

    map_clean(parentMap); 
    free(parentMap);
}


int main() {

    State estado_inicial = {
        {{0, 2, 8}, 
         {1, 3, 4},
         {6, 5, 7}, 
        },  
        0,0
    };
    estado_inicial.actions = list_create();

    printf("Estado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);

    printf("Distancia L1:%d\n", distancia_L1(&estado_inicial));
    int opcion;
    do {
        puts("========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");

        puts("1) Búsqueda en Profundidad");
        puts("2) Buscar en Anchura");
        puts("3) Buscar Mejor Primero");
        puts("4) Salir");
        
        printf("Ingrese su opción: ");
        scanf(" %d", &opcion);
        int profundidad;
        switch (opcion) {
        case 1:
            puts("========================================");
            puts("     BÚSQUEDA POR PROFUNDIDAD (DFS)");
            puts("========================================");
            puts("//La profundidad del puzzle para encontrar la solución en este caso es 15.");
            printf("Ingrese la profundidad máxima de búsqueda: ");
            
            scanf("%d", &profundidad);
            
            dfs(&estado_inicial, profundidad);
          break;
        case 2:
            puts("========================================");
            puts("       BÚSQUEDA POR ANCHURA (BFS)");
            puts("========================================");
          bfs(&estado_inicial);
          break;
        case 3:
            puts("========================================");
            puts("BÚSQUEDA POR MEJOR PRIMERO (best_first)");
            puts("========================================");
          best_first(&estado_inicial);
          break;
        }
        presioneTeclaParaContinuar();
        limpiarPantalla();

  } while (opcion != 4);

  return 0;
}
