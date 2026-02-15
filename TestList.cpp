#include <cassert>
#include <iostream>
#include "PlaylistService.h"
#include "DurationUtils.h"

//Voy a comentar momentaneamente el main.cpp para hacer las pruebas 

int main() {
    PlaylistService svc;

    // Test 1: Crear cancion
    Song s1 = svc.createSong("CancionTest", "ArtistaTest", parseDurationToSeconds("2:30"));
    assert(s1.id == 1);
    assert(svc.hasSongs());
    std::cout << "Test 1 200 OK: Cancion creada correctamente.\n";
    svc.listAllSongs(); // mostrar todas las canciones

    // Test 2: Buscar cancion por ID y nombre
    Song* byId = svc.findSongById(s1.id);
    assert(byId != nullptr && byId->name == "CancionTest");
    Song* byName = svc.findSongByName("CancionTest");
    assert(byName != nullptr && byName->artist == "ArtistaTest");
    std::cout << "Test 2 200 OK: Busqueda por ID y nombre correctas.\n";

    // Test 3: Crear playlist y agregar cancion
    int pid = svc.createPlaylist("ListaTest");
    bool added = svc.addSongToPlaylist(pid, s1.id);
    assert(added);
    std::cout << "Test 3 200 OK: Playlist creada y cancion agregada.\n";
    svc.listPlaylists(); // mostrar playlists
    svc.listSongsInPlaylist(pid); // mostrar canciones en la playlist

    // Test 4: Eliminar cancion y limpiar referencias
    std::cout << "Eliminando cancion ID " << s1.id << "...\n";
    bool deleted = svc.deleteSongById(s1.id);
    assert(deleted);
    auto idsAfter = svc.getSongIdsInPlaylist(pid);
    assert(idsAfter.empty());
    std::cout << "Test 4 200 OK: Cancion eliminada y referencias limpias.\n";
    svc.listSongsInPlaylist(pid); // mostrar playlist 

    // Test 5: Renombrar playlist
    bool renamed = svc.editPlaylistName(pid, "ListaRenombrada");
    assert(renamed);
    int pidByName = svc.findPlaylistIdByName("ListaRenombrada");
    assert(pidByName == pid);
    std::cout << "Test 5 200 OK: Playlist renombrada correctamente.\n";
    svc.listPlaylists(); // mostrar playlists 

    // Test 6: Validacion de duracion
    int durValid = parseDurationToSeconds("3:15");
    int durInvalid = parseDurationToSeconds("3:60");
    assert(durValid == 195);
    assert(durInvalid == -1);
    std::cout << "Test 6 200 OK: Validacion de duracion correcta.\n";

    std::cout << "\nTodos los tests pasaron correctamente, yaaaayyyyy\n";
    return 0;
}
