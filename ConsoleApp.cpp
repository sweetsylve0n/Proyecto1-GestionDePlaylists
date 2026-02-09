#include "ConsoleApp.h"
#include "DurationUtils.h"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

ConsoleApp::ConsoleApp() = default;
ConsoleApp::~ConsoleApp() = default;

void ConsoleApp::run() {
    if (!service.hasSongs()) {
        cout << "Parece que es la primera vez que usa el sistema.\n";
        cout << "Debe crear al menos una cancion para continuar.\n";
        while (!service.hasSongs()) {
            createSong();
            if (!service.hasSongs()) {
                cout << "Aún no hay canciones. Debe crear al menos una para continuar.\n";
            }
        }
    }

    int opt = -1;
    do {
        showMainMenu();
        opt = readInt("Opcion: ");
        handleOption(opt);
    } while (opt != 0);
}

void ConsoleApp::showMainMenu() const {
    cout << "\n--- GESTION DE PLAYLISTS ---\n";
    cout << "1. Crear cancion\n";
    cout << "2. Listar todas las canciones\n";
    cout << "3. Buscar cancion por ID\n";
    cout << "4. Editar cancion\n";
    cout << "5. Eliminar cancion\n";
    cout << "6. Crear lista de reproduccion\n";
    cout << "7. Listar playlists\n";
    cout << "8. Agregar cancion a playlist\n";
    cout << "9. Listar canciones de una playlist\n";
    cout << "10. Eliminar cancion de una playlist\n";
    cout << "11. Eliminar playlist\n";
    cout << "12. Editar nombre de playlist\n";
    cout << "13. Reproducir playlist en orden\n";
    cout << "14. Reproducir playlist en aleatorio\n";
    cout << "0. Salir\n";
}

void ConsoleApp::handleOption(int opt) {
    switch (opt) {
    case 1: createSong(); break;
    case 2: listSongs(); break;
    case 3: findSong(); break;
    case 4: editSong(); break;
    case 5: deleteSong(); break;
    case 6: createPlaylist(); break;
    case 7: listPlaylists(); break;
    case 8: addSongToPlaylist(); break;
    case 9: listSongsInPlaylist(); break;
    case 10: removeSongFromPlaylist(); break;
    case 11: deletePlaylist(); break;
    case 12: editPlaylistName(); break;
    case 13: playPlaylistInOrder(); break;
    case 14: playPlaylistShuffle(); break;
    case 0: cout << "Saliendo...\n"; break;
    default: cout << "Opcion no valida.\n"; break;
    }
}

/* -------------------- Canciones -------------------- */

void ConsoleApp::createSong() {
    string name = readLine("Ingrese nombre de la cancion: ");
    if (name.empty()) { cout << "Nombre vacio. Operacion cancelada.\n"; return; }
    string artist = readLine("Ingrese artista: ");
    if (artist.empty()) { cout << "Artista vacio. Operacion cancelada.\n"; return; }
    string durStr = readLine("Ingrese duracion (mm:ss o ss): ");
    int dur = parseDurationToSeconds(durStr);
    if (dur < 0) {
        cout << "Formato de duracion invalido. Use mm:ss o segundos.\n";
        return;
    }
    Song s = service.createSong(name, artist, dur);
    cout << "Cancion creada. ID: " << s.id << "\n";
}

void ConsoleApp::listSongs() {
    service.listAllSongs();
}

void ConsoleApp::findSong() {
    int id = readInt("Ingrese ID de la cancion: ");
    Song* s = service.findSongById(id);
    if (!s) cout << "Cancion no encontrada.\n";
    else cout << "ID: " << s->id << " | " << s->name << " - " << s->artist
        << " (" << formatSecondsToMMSS(s->durationSec) << ")\n";
}

void ConsoleApp::editSong() {
    int id = readInt("Ingrese ID de la cancion a editar: ");
    Song* s = service.findSongById(id);
    if (!s) { cout << "Cancion no encontrada.\n"; return; }
    string newName = readLine("Nuevo nombre (enter para mantener): ");
    string newArtist = readLine("Nuevo artista (enter para mantener): ");
    string durStr = readLine("Nueva duración (mm:ss o ss) (enter para mantener): ");

    string finalName = newName.empty() ? s->name : newName;
    string finalArtist = newArtist.empty() ? s->artist : newArtist;
    int finalDur = s->durationSec;
    if (!durStr.empty()) {
        int parsed = parseDurationToSeconds(durStr);
        if (parsed < 0) { cout << "Formato de duración inválido. Edición cancelada.\n"; return; }
        finalDur = parsed;
    }

    if (service.editSongById(id, finalName, finalArtist, finalDur))
        cout << "Cancion actualizada.\n";
    else
        cout << "No se pudo actualizar la cancion.\n";
}

void ConsoleApp::deleteSong() {
    int id = readInt("Ingrese ID de la cancion a eliminar: ");
    if (service.deleteSongById(id)) cout << "cancion eliminada.\n";
    else cout << "No se pudo eliminar la cancion (no existe).\n";
}

/* -------------------- Playlists -------------------- */

void ConsoleApp::createPlaylist() {
    string name = readLine("Ingrese nombre de la playlist: ");
    if (name.empty()) { cout << "Nombre vacio. Operacion cancelada.\n"; return; }
    int id = service.createPlaylist(name);
    cout << "Playlist creada. ID: " << id << "\n";
}

void ConsoleApp::listPlaylists() {
    service.listPlaylists();
}

void ConsoleApp::addSongToPlaylist() {
    int pid = readInt("Ingrese ID de la playlist: ");
    int sid = readInt("Ingrese ID de la Cancion: ");
    if (service.addSongToPlaylist(pid, sid)) cout << "Cancion agregada a la playlist.\n";
    else cout << "No se pudo agregar (playlist o cancion no existe, o ya esta agregada).\n";
}

void ConsoleApp::listSongsInPlaylist() {
    int pid = readInt("Ingrese ID de la playlist: ");
    service.listSongsInPlaylist(pid);
}

void ConsoleApp::removeSongFromPlaylist() {
    int pid = readInt("Ingrese ID de la playlist: ");
    int sid = readInt("Ingrese ID de la Cancion a eliminar de la playlist: ");
    if (service.removeSongFromPlaylist(pid, sid)) cout << "Cancion eliminada de la playlist.\n";
    else cout << "No se pudo eliminar (playlist o cancion no existe en la playlist).\n";
}

void ConsoleApp::deletePlaylist() {
    int pid = readInt("Ingrese ID de la playlist a eliminar: ");
    if (service.deletePlaylistById(pid)) cout << "Playlist eliminada.\n";
    else cout << "No se pudo eliminar la playlist.\n";
}

void ConsoleApp::editPlaylistName() {
    int pid = readInt("Ingrese ID de la playlist a editar: ");
    string newName = readLine("Nuevo nombre: ");
    if (newName.empty()) { cout << "Nombre vacio. Operacion cancelada.\n"; return; }
    if (service.editPlaylistName(pid, newName)) cout << "Playlist renombrada.\n";
    else cout << "No se encontro la playlist.\n";
}

/* -------------------- Reproduccion -------------------- */

void ConsoleApp::playPlaylistInOrder() {
    int pid = readInt("Ingrese ID de la playlist a reproducir en orden: ");
    auto ids = service.getSongIdsInPlaylist(pid);
    if (ids.empty()) {
        cout << "La playlist no existe o no tiene canciones.\n";
        return;
    }
    auto songs = service.getSongsByIds(ids);
    cout << "Reproduciendo en orden (simulado):\n";
    for (const auto& s : songs) {
        cout << "Reproduciendo: " << s.name << " - " << s.artist
            << " (" << formatSecondsToMMSS(s.durationSec) << ")\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }
    cout << "Reproduccion finalizada.\n";
}

void ConsoleApp::playPlaylistShuffle() {
    int pid = readInt("Ingrese ID de la playlist a reproducir en aleatorio: ");
    auto ids = service.getSongIdsInPlaylist(pid);
    if (ids.empty()) {
        cout << "La playlist no existe o no tiene canciones.\n";
        return;
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(ids.begin(), ids.end(), g);

    auto songs = service.getSongsByIds(ids);
    cout << "Reproduciendo en aleatorio (simulado):\n";
    for (const auto& s : songs) {
        cout << "Reproduciendo: " << s.name << " - " << s.artist
            << " (" << formatSecondsToMMSS(s.durationSec) << ")\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }
    cout << "Reproduccion aleatoria finalizada.\n";
}

/* -------------------- Utilidades de entrada -------------------- */

string ConsoleApp::readLine(const string& prompt) const {
    cout << prompt;
    string line;
    std::getline(cin, line);
    return line;
}

int ConsoleApp::readInt(const string& prompt) const {
    while (true) {
        cout << prompt;
        string line;
        std::getline(cin, line);
        try {
            if (line.empty()) continue;
            int v = stoi(line);
            return v;
        }
        catch (...) {
            cout << "Entrada invalida. Ingrese un numero entero.\n";
        }
    }
}