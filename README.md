# Proyecto1-GestionDePlaylists

Una aplicacion de consola en C++ que implementa un sistema de gestion y reproduccion de playlists musicales. Este proyecto utiliza estructuras de datos fundamentales como punteros, nodos, listas enlazadas simples y dobles.

---

## Descripcion General

Proyecto1-GestionDePlaylists es una solucion que simula las funcionalidades basicas de una plataforma de musica digital. Permite crear, organizar y reproducir canciones en playlists personalizadas, todo desde una interfaz de menu por consola.

---

## Caracteristicas Implementadas

### Gestion de Canciones

- Crear canciones con nombre, artista y duracion
- Listar todas las canciones disponibles en el sistema
- Buscar cancion especifica por su ID
- Editar datos de una cancion existente
- Eliminar cancion del sistema (con eliminacion automatica de referencias en playlists)

### Gestion de Playlists

- Crear nuevas listas de reproduccion con nombre personalizado
- Listar todas las playlists con el numero de canciones que contienen
- Agregar canciones existentes a una playlist especifica
- Listar canciones de una playlist seleccionada
- Eliminar canciones de una playlist sin afectar la cancion original
- Eliminar playlist completa
- Renombrar playlists existentes

### Reproduccion

- Reproduccion en orden: simula la reproduccion secuencial de canciones
- Reproduccion aleatoria (Shuffle): simula la reproduccion con canciones mezcladas

---

## Arquitectura

### Componentes Principales

**ConsoleApp.h / ConsoleApp.cpp**: Interfaz de usuario que gestiona el menu interactivo y controla el flujo de la aplicacion.

**PlaylistService.h / PlaylistService.cpp**: Servicio central que implementa la logica de negocio, gestion de canciones, playlists y operaciones de busqueda, modificacion y eliminacion.

**SinglyLinkedList.h**: Template que implementa lista enlazada simple con operaciones push_back(), push_front(), findById(), removeIf() y traverse().

**DoublyLinkedList.h**: Template que implementa lista doblemente enlazada con punteros bidireccionales y acceso eficiente desde ambos extremos.

---

## Estructuras de Datos Utilizadas

### Lista Enlazada Simple (SinglyLinkedList)

- Cada nodo contiene: data (dato generico) y next (puntero al siguiente nodo)
- Operaciones O(1): insercion al inicio y final
- Operaciones O(n): busqueda y acceso secuencial
- Uso: almacena canciones, playlists e IDs de canciones dentro de playlists

### Lista Doblemente Enlazada (DoublyLinkedList)

- Cada nodo contiene: data, next y prev (punteros bidireccionales)
- Mantiene punteros a head y tail para acceso desde ambos extremos
- Eliminacion mas eficiente que listas simples
- Incluida para referencia y posibles extensiones futuras

---

## Conceptos Implementados

### Punteros y Memoria Dinamica

- Uso de new y delete para crear y destruir nodos
- Navegacion entre nodos mediante punteros next y prev
- Destructores que liberan automaticamente toda la memoria

### Listas Enlazadas

Ventajas:
- Insercion y eliminacion O(1) en posiciones conocidas
- Crecimiento dinamico sin preocuparse por capacidad
- No hay fragmentacion de memoria innecesaria


### Templates

- SinglyLinkedList<T> y DoublyLinkedList<T> son genericas
- Se reutiliza codigo para diferentes tipos: Song, int, PlaylistInternal
- Permite flexibilidad y mantenimiento simplificado

### Integridad de Datos

- Cuando se elimina una cancion, se eliminan todas sus referencias en playlists
- Cuando se elimina una playlist, no se afectan las canciones originales
- Validacion de entrada para nombres vacios y formatos incorrectos

---

## Notas Tecnicas

### Manejo de IDs

- nextSongId y nextPlaylistId se incrementan automaticamente
- IDs comienzan en 1 y son unicos durante la sesion
- Cada nuevo elemento recibe un ID secuencial

### Validacion

- Se valida que nombres no esten vacios
- Se valida formato de duracion (mm:ss o segundos)
- Se verifica existencia de playlists y canciones antes de operaciones

### Reproduccion Simulada

- Usa std::this_thread::sleep_for() para simular tiempo entre canciones
- Shuffle utiliza std::random_shuffle() para mezclar canciones aleatoriamente

---

## Estado del Desarrollo

### Porcentaje Avanzado: 80%

El proyecto ha completado exitosamente la mayoria de las funcionalidades base. La aplicacion es completamente funcional para crear, gestionar y reproducir canciones en playlists.

### Requerimientos Completados

- RF-01: Crear canciones
- RF-02: Listar todas las canciones
- RF-03: Consultar cancion especifica por ID
- RF-04: Editar datos de cancion existente
- RF-05: Eliminar cancion del sistema
- RF-06: Crear lista de reproduccion
- RF-07: Agregar cancion a lista de reproduccion
- RF-08: Listar canciones de una lista de reproduccion
- RF-09: Reproducir cancion en orden secuencial
- RF-10: Reproducir cancion en orden aleatorio
- RF-11: Editar lista de reproduccion
- RF-12: Eliminar cancion de lista de reproduccion
- RF-13: Eliminar lista de reproduccion

### Requerimientos Pendientes de Desarrollo

#### 1. Persistencia de Datos (Prioridad Alta)

Permitir guardar y cargar datos (canciones y playlists) desde archivos de almacenamiento persistente.

Alcance:
- Implementar serializacion de datos en archivos (JSON, CSV o binario)
- Cargar datos al iniciar la aplicacion
- Guardar cambios automaticamente o por solicitud del usuario
- Manejar errores de lectura/escritura de archivos
- Ubicacion: Storage.h / Storage.cpp

#### 2. Busqueda Avanzada de Canciones (Prioridad Media)

Implementar capacidades de busqueda sofisticadas mas alla de buscar solo por ID.

Alcance:
- Buscar canciones por nombre (busqueda parcial o exacta)
- Buscar canciones por artista
- Buscar canciones por duracion (rango)
- Filtros combinados (nombre AND artista)
- Ordenamiento de resultados (alfabetico, por duracion, etc.)
- Ubicacion: PlaylistService (nuevos metodos), ConsoleApp (interfaz)

---

## Proximos Pasos

Para completar el desarrollo al 100%:

1. Implementar sistema de persistencia para guardar biblioteca de canciones y playlists
2. Desarrollar motor de busqueda avanzada con multiples criterios de filtrado
3. Realizar pruebas unitarias al sistema completo


