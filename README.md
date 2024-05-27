# Tarea-3-estructura
## Descripción

**Programa 8-puzzle**
Este programa permite resolver el problema del 8 puzzle, ya sea el que está propuesto en el programa u otros., el usuaro solo debe seleccionar qué tipo de búsqueda desea hacer, DFS, BFS o mejor primero.

---
## Cómo compilar y ejecutar

Dado que el sistema está diseñado para ser accesible y fácil de probar, recomendamos usar [Repl.it](http://repl.it/) para una rápida configuración y ejecución. Sigue estos pasos para comenzar:

1. Visita [Repl.it](https://repl.it/).
2. Usa el link de abajo que te llevará directamente a la aplicación
3. [Repl.it](https://replit.com/join/mgscwdxppz-milovanfuentesl).
4. Presiona el botón "Run" para compilar y ejecutar la aplicación.

## Funcionalidades
### Funcionando correctamente:

- Funciona de manera correcta la búsqueda por profundidad (DFS).
- Funciona de manera correcta la búsqueda por anchura (BFS).
- Si bien funciona, no es como esperamos, la busqueda mejor primero (best_first).

  
### Problemas conocidos: 
-La función mejor primero (best_first) no encuentra realmente la solución con menos pasos, en el ejemplo encuentra la solución en 14, en cambio, el BFS la encuentra en 12, produciendo una duda al respecto de su correcto funcionamiento. No sabemos cuál es el problema que causa esto, tampoco la solución.

### A mejorar:
-Mejorar la interfaz de usuario.
-Mejorar/corregir la funcion best_first.

### Ejemplo de uso:

  **Paso 1: Escoger la opción búsqueda por profundidad (DFS).**

  ````c
Ingrese su opción: 1
========================================
     BÚSQUEDA POR PROFUNDIDAD (DFS)
========================================
//La profundidad del puzzle para encontrar la solución en este caso es 15.
Ingrese la profundidad máxima de búsqueda: 15

¡Solución encontrada!
Cantidad de iteraciones realizadas: 951

Estado inicial:
x 2 8 
1 3 4 
6 5 7 

Paso N°1:
Acción: Abajo
1 2 8 
x 3 4 
6 5 7
...
````
El sistema busca mediante el algoritmo DFS si hay una solución, se limita a una altura para que no caiga en una iteración infinita.

  **Paso 2: Escoger la opción búsqueda por anchura (BFS).**
  ````c
Ingrese su opción: 2
========================================
       BÚSQUEDA POR ANCHURA (BFS)
========================================

¡Solución encontrada!
Cantidad de iteraciones realizadas: 1407

Estado inicial:
x 2 8 
1 3 4 
6 5 7 

Paso N°1:
Acción: Abajo
1 2 8 
x 3 4 
6 5 7
````
El programa busca la solución mediante el sistema BFS.

  **Paso 3: Escoger la opción mejor primero (best_first).**
````c
========================================
BÚSQUEDA POR MEJOR PRIMERO (best_first)
========================================

¡Solución encontrada!
Cantidad de iteraciones realizadas: 14

Estado inicial:
x 2 8 
1 3 4 
6 5 7 

Paso N°1:
Acción: Abajo
1 2 8 
x 3 4 
6 5 7
````
El sistema busca la solución mediante el camino más corto y con menos iteraciones posibles con la fucnión best_first.

### Contribuciones
## Oscar Ruiz
- Ciclo principal DFS.
- Función isFinal.
- Función best_first.
- Función state_equal.
- **Auto-evaluación**: 3 (Aporte excelente)

## Milovan Fuentes
- Función transition.
- Función get_adyacents_nodes.
- Función BFS.
- Interfaz.
- **Auto-evaluación**: 3 (Aporte excelente)
  
