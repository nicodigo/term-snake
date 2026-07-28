# Roadmap: Snake en terminal (C, sin librerías gráficas)

Proyecto didáctico. Sin librerías de terceros. Nivel de partida: C básico (punteros, structs, unions, enums), poca práctica real (mismo nivel declarado para HTTPrueba).

## Fase 1: Terminal en modo raw
**Qué vas a construir:** un programa que lea una sola tecla sin necesitar Enter y sin que se refleje en pantalla, y que deje la terminal como estaba al salir.
**Depende de:** tu C previo.

**Paso a paso:**
1. **Modo canónico por defecto** — buscá qué significa que una terminal esté en "modo canónico" y qué comportamiento tiene por defecto antes de que tu programa reciba algo.
2. **Leer y guardar la configuración actual** — investigá qué función te permite obtener la configuración actual de la terminal antes de tocar nada, y por qué guardarla es necesario.
3. **El campo que agrupa el modo de línea** — dentro de la estructura de configuración, investigá qué campo agrupa las opciones relacionadas con cómo se procesa la entrada.
4. **Desactivar el modo de línea** — investigá, por separado del resto, qué flag específica hay que desactivar para que tu programa reciba cada tecla sin esperar un Enter.
5. **Desactivar el eco** — investigá, como pregunta aparte de la anterior, qué flag específica controla si el caracter tecleado aparece reflejado en la terminal.
6. **Aplicar y restaurar** — investigá qué función aplica una configuración modificada, y qué mecanismo de C te asegura ejecutar código de restauración incluso si el programa termina de forma distinta a la esperada.

**Señal de que terminaste la fase:** apretás una tecla sin Enter, no la ves reflejada en pantalla, y al terminar el programa la terminal vuelve a comportarse normal.

## Fase 2: Lectura de teclado no bloqueante
**Qué vas a construir:** que tu programa siga ejecutando el resto de la lógica aunque no haya ninguna tecla presionada en ese instante, en vez de quedarse esperando.
**Depende de:** fase 1.

**Paso a paso:**
1. **Qué significa que una lectura sea bloqueante** — investigá qué le pasa a tu programa cuando llama a una función de lectura estándar y todavía no hay datos disponibles.
2. **Opciones para lectura no bloqueante en la misma configuración de terminal** — investigá qué parámetros, dentro de la misma configuración que ya tocaste en la fase 1, permiten que una lectura devuelva el control aunque no haya tecla disponible.
3. **Distinguir "no hay tecla" de "se leyó una tecla"** — investigá cómo se identifica, en el valor que devuelve la función de lectura, cada uno de esos dos casos.

**Señal de que terminaste la fase:** un loop que imprime algo continuamente no se detiene esperando una tecla, pero igual detecta cuándo apretás una.

## Fase 3: Control de framerate
**Qué vas a construir:** que el loop anterior corra a una velocidad constante y controlada, en vez de correr tan rápido como pueda la CPU.
**Depende de:** fase 2.

**Paso a paso:**
1. **Por qué un loop sin control de tiempo es un problema** — pensá qué pasaría con la velocidad de tu juego en una máquina mucho más rápida o más lenta que la tuya si no controlás el tiempo entre iteraciones.
2. **Formas de obtener una marca de tiempo** — investigá qué funciones de C o del sistema existen para registrar un instante de tiempo dentro de tu programa. Buscá al menos dos opciones distintas.
3. **Garantías de cada opción de tiempo** — como pregunta aparte de la anterior, investigá qué garantías o unidades documenta cada una de las funciones que encontraste.
4. **Pausar la ejecución un tiempo determinado** — investigá qué función te permite pausar tu programa una cantidad específica de tiempo, y en qué unidad espera ese valor.

**Señal de que terminaste la fase:** tu loop avanza a un ritmo visualmente constante y predecible, no a la velocidad máxima que permite la CPU.

## Fase 4: Dibujar el tablero en la terminal
**Qué vas a construir:** un tablero rectangular (bordes y espacio interior) dibujado en la terminal, que se redibuje limpio en cada iteración sin que las versiones anteriores queden apiladas en pantalla.
**Depende de:** fase 3.

**Paso a paso:**
1. **Representar el tablero como datos** — pensá qué estructura de datos en C representaría una grilla 2D de celdas, antes de dibujar nada.
2. **Posicionar texto en un lugar específico de la terminal** — investigá qué mecanismo existe para mover el punto donde se escribe el próximo caracter a una fila y columna determinadas de la terminal.
3. **Limpiar el contenido previo entre frames** — investigá, como pregunta aparte, qué mecanismo permite borrar lo dibujado en el frame anterior antes de dibujar el siguiente.

**Señal de que terminaste la fase:** ves un tablero con bordes en la terminal, y al correr el loop no se apilan tableros nuevos debajo de los anteriores.

## Fase 5: Representar y mover la serpiente
**Qué vas a construir:** una serpiente de largo fijo que se mueve sola en una dirección dentro del tablero, todavía sin responder al teclado.
**Depende de:** fase 4.

**Paso a paso:**
1. **Representar un cuerpo de varios segmentos** — pensá qué estructura de datos en C representaría mejor una secuencia ordenada de posiciones, desde la cabeza hasta la cola, que además pueda crecer más adelante.
2. **Actualizar cada segmento en un frame** — pensá qué le tiene que pasar a cada segmento del cuerpo cuando la cabeza avanza una celda, para que el cuerpo "siga" a la cabeza en vez de estirarse infinitamente.
3. **Integrar el movimiento con el dibujo de la fase 4** — revisá qué parte de tu código de dibujo tenés que cambiar para que la serpiente aparezca en su posición actual en cada frame, en vez de en una posición fija.

**Señal de que terminaste la fase:** ves la serpiente moviéndose sola en línea recta dentro del tablero, frame a frame, sin que el jugador la controle todavía.

## Fase 6: Control de dirección con el teclado
**Qué vas a construir:** que las teclas de dirección cambien hacia dónde se mueve la cabeza de la serpiente en el próximo frame.
**Depende de:** fase 5 y fase 2.

**Paso a paso:**
1. **De tecla leída a cambio de estado** — pensá qué variable de tu programa tendría que actualizarse cuando se detecta una tecla de dirección, sin todavía tocar cómo se mueve la serpiente.
2. **El caso de la dirección opuesta** — pensá qué pasaría si el jugador presiona la dirección opuesta a la actual en un mismo frame, y por qué eso podría ser un problema para una serpiente con cuerpo.

**Señal de que terminaste la fase:** podés dirigir la serpiente con el teclado en las cuatro direcciones mientras el juego corre.

## Fase 7: Comida y crecimiento
**Qué vas a construir:** un ítem de comida en una posición del tablero que, al ser alcanzado por la cabeza, hace crecer el cuerpo un segmento y reaparece en otro lugar.
**Depende de:** fase 6.

**Paso a paso:**
1. **Elegir una posición al azar** — investigá qué funciones de C generan números pseudoaleatorios, y qué hay que hacer para que la secuencia no sea idéntica en cada ejecución del programa.
2. **Detectar que la cabeza llegó a la comida** — pensá qué condición sobre las coordenadas de la cabeza y de la comida indica que coinciden.
3. **Crecer el cuerpo sin romper el movimiento de la fase 5** — revisá tu lógica de actualización de segmentos: ¿qué tendrías que cambiar para que, en el frame en que la serpiente come, el último segmento no desaparezca como lo hace normalmente?

**Señal de que terminaste la fase:** la serpiente crece cada vez que alcanza la comida, y aparece una nueva comida en otra posición.

## Fase 8: Fin del juego
**Qué vas a construir:** que el juego termine y lo muestre en pantalla cuando la serpiente choca contra una pared o contra su propio cuerpo.
**Depende de:** fase 7.

**Paso a paso:**
1. **Choque contra los límites del tablero** — pensá qué condición sobre la posición de la cabeza indica que salió del área jugable.
2. **Choque contra el propio cuerpo** — como pregunta aparte de la anterior, pensá qué condición sobre la posición de la cabeza respecto al resto de los segmentos indica una colisión con el propio cuerpo.
3. **Terminar el programa sin romper la terminal** — revisá qué garantía de la fase 1 tenés que preservar para que, aunque el juego termine por una colisión, la terminal quede utilizable después de salir.

**Señal de que terminaste la fase:** el juego se detiene y avisa el fin cuando la serpiente choca contra una pared o contra sí misma, y la terminal funciona con normalidad después.

---

## Progresión completa
1. Terminal en modo raw
2. Lectura de teclado no bloqueante
3. Control de framerate
4. Dibujar el tablero en la terminal
5. Representar y mover la serpiente
6. Control de dirección con el teclado
7. Comida y crecimiento
8. Fin del juego
