<!-- coderabbit-ai-generated:v1 -->

# Notas de revision de la alpha

La primera pasada de CodeRabbit termino con **11 hallazgos, 2 avisos sin verificar y 1 archivo sin revisar**. La CLI no identifico ese archivo en el resumen. No se presenta como una revision sin incidencias.

## Hallazgos atendidos

- La salida binaria de la prueba C++ se alteraba por la traduccion de saltos de linea de Windows: se usa `_O_BINARY`. Las pruebas cruzadas de Windows lo confirmaron.
- La copia repetida de la consola de PyInstaller podia anidar una version dentro de otra: se sustituyo ese flujo por un ZIP nuevo con el Python embebible oficial y sin dependencia de PyInstaller.
- La espera de STATUS en la prueba UDP podia prolongarse indefinidamente si solo llegaban HELLO: ahora comparte un plazo total de 10 segundos y filtra el intento.
- La geometria extrema podia agotar memoria antes de aplicar el presupuesto de simulacion: el adaptador valida valores finitos, bordes y cobertura total antes de invocar el constructor del indice. Se prueba que no se llama a `build_level` en los casos rechazados. No fue necesario modificar la dependencia.
- Se atendieron los dos avisos sin verificar: conjunto de triggers visuales constante, precedencia explicita y clave de dos jugadores corregida de `kA13` a `kA10`. La segunda correccion se contrasto con una fuente activa independiente y tiene pruebas para aceptar offset musical 1 y rechazar dos jugadores.

## Politica conservada

El aviso sobre mecanicas no soportadas encontradas en una rama futura propone seguir corrigiendo el estado real. Se conserva la politica documentada: suspender el intento al detectar una mecanica que el modelo no representa. No se relaja esa garantia por una sugerencia de revision. No se afirma que toda divergencia oculta pueda detectarse comparando los campos observables.

## Dependencia fijada sin cambios

Los archivos de `worker/vendor` conservan exactamente la revision indicada en UPSTREAM.json y su licencia MIT. Cinco avisos se refieren a rutas que la consola distribuida no utiliza: compilacion automatica concurrente, `available()` con exports incompletos, `find_path(input_period=0)` y dos caches del motor de triggers. La consola usa una DLL precompilada, un solo hilo de simulacion, busqueda propia y `triggers=False`; los triggers dinamicos se rechazan. El limite de exports incompletos tambien se captura al cargar el nivel para informar el error.

Otro aviso identifica codigo inalcanzable en la colision de wave con pendientes de la referencia Python. El nucleo C++ conserva la misma politica existente. Sin el juego ni una especificacion de fisica verificada para ese caso, no se cambia la fisica ni se hace que las pruebas esperen otro comportamiento. Queda como limitacion de fidelidad para contrastar en GD.

## Verificacion

La suite incluye comparacion del C++ con la referencia Python en 80 combinaciones de modo, tamano y velocidad y compara byte por byte las decisiones de busqueda a 80/400 ticks. Esto valida coherencia interna, no precision de Geometry Dash. Las pruebas en el juego, FPS y sincronizacion CBF siguen pendientes.

La busqueda de 400 ticks del caso sintetico de un pincho superior agotaba el presupuesto de 6 ms con conversiones por paso en Python; tras mover el bucle al adaptador C++ externo produjo una decision en 0,162 ms en este host. Es una muestra de consulta, no una medicion de FPS ni una promesa de latencia.

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:f1358539443496d137238c0342adf40aa8ff4d2f3d888acad1a020c11a185905 -->

## Segunda pasada

La segunda pasada termino y guardo **15 comentarios: 11 major y 4 minor**. Se corrigieron los tres comentarios del proyecto: las grabaciones ahora verifican cierre y publicacion atomica y retienen una recuperacion acotada; la configuracion de bindings vuelve a intentar una descarga fallida; el contador y los bits del historial avanzan al completarse `PlayerObject::update`, no por cada `processCommands`. Los pasos cero no avanzan el modelo; un paso parcial, personalizado o no finito suspende la correccion hasta reiniciar. Hay pruebas del reloj y de los fallos de escritura. La secuencia real de hooks sigue pendiente de probar en GD.

Los otros 12 comentarios estan en la dependencia fijada. El aviso de tabla de objetos ausente es incorrecto: `gdgen/data/objects_2.2081.json` existe, no esta ignorado y se incluye y comprueba byte por byte en el paquete. Su atributo `-diff` evita enviar 122000 lineas al revisor. El empaquetador ahora tambien falla si falta cualquier archivo de UPSTREAM.json.

Se mantienen sin modificar los otros 11 comentarios de la dependencia: dos avisos repetidos de compilacion automatica/exports, estados de otro simulador, dos limites/resultado de busqueda y seis casos de dual inicial o triggers dinamicos. La consola no compila al arrancar ni llama a `available` o `find_path`; utiliza un solo simulador propietario por estado, rechaza dual antes de construirlo y usa `triggers=False` rechazando triggers dinamicos. No se aplica una correccion de fisica no verificada ni se ocultan estos avisos como si la revision hubiera quedado limpia. La limitacion wave/pendiente de la primera pasada tambien permanece pendiente de contrastar con el juego.

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:5c8b5deaaaf2be37f392e8679e477a8cf2a21e9935a3e8a50873bc6bcd10cd83 -->

## Tercera pasada: conexion interrumpida

Tras las correcciones se ejecuto una tercera revision del diff. La CLI termino con `TRPCWebSocketClosedError` / `WebSocket closed`, codigo 1, sin resultado final. No cuenta como revision completada ni como confirmacion de cero hallazgos. Las dos pasadas anteriores y sus avisos permanecen registrados arriba. Las comprobaciones automatizadas y del paquete si terminaron correctamente; no sustituyen la prueba dentro del juego. El script PowerShell recibe tambien `rev-parse --quiet` para consultar HEAD aun no creado sin emitir un error nativo; su orquestacion completa sigue sin ejecutarse en Windows.

## Fuente de la cabecera

<!-- /coderabbit-ai-generated:v1 -->

<!-- /coderabbit-ai-generated:v1 -->

[Formato documentado de la cadena interna del nivel](https://github.com/gd-programming/gddocs/blob/860029207cedc7f1a183216a2165c70eff72b24d/docs/resources/client/level-components/inner-level-string.md): `kA10` indica dos jugadores; `kA13` indica desplazamiento de la cancion. El repositorio gddocs estaba activo, no archivado, al consultarlo.
