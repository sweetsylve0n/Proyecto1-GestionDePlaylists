#include "ConsoleApp.h"
#include "DurationUtils.h"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <windows.h>
#include <cstdlib>



ConsoleApp::ConsoleApp() = default;
ConsoleApp::~ConsoleApp() = default; //Constructor y destructor

void ConsoleApp::run() {
    if (!service.hasSongs()) {
        std::cout << "Parece que es la primera vez que usas el sistema!! ^0^/ \n";
        std::cout << "Debes crear al menos una cancion para continuar...\n";
        while (!service.hasSongs()) {
            createSong();
            if (!service.hasSongs()) {
                std::cout << "Aún no hay canciones. Debes crear al menos una para continuar.\n";
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
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(h, &info);
    WORD original = info.wAttributes;

    std::string title = " GESTION DE PLAYLISTS ";
    const int width = 50; // ancho total del marco
    int padLeft = (width - (int)title.size()) / 2;
    int padRight = width - padLeft - (int)title.size();

    std::string border(width, '=');


    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::cout << border << '\n';
    std::cout << '=' << std::string(padLeft, ' ')
        << title
        << std::string(padRight, ' ') << '=' << '\n';
    std::cout << border << '\n';

    SetConsoleTextAttribute(h, original);

    // Menú
    std::cout << "1. Crear cancion\n";
    std::cout << "2. Listar todas las canciones\n";
    std::cout << "3. Buscar cancion\n";
    std::cout << "4. Editar cancion\n";
    std::cout << "5. Eliminar cancion\n";
    std::cout << "6. Crear lista de reproduccion\n";
    std::cout << "7. Listar playlists\n";
    std::cout << "8. Agregar cancion a playlist\n";
    std::cout << "9. Listar canciones de una playlist\n";
    std::cout << "10. Eliminar cancion de una playlist\n";
    std::cout << "11. Eliminar playlist\n";
    std::cout << "12. Editar nombre de playlist\n";
    std::cout << "13. Reproducir playlist en orden\n";
    std::cout << "14. Reproducir playlist en aleatorio\n";
    std::cout << "0. Salir\n";
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
    case 0: std::cout << "Saliendo...\n"; break;
    default: std::cout << "Opcion no valida.\n"; break;
    }
    if (opt != 0) pauseAndClear();
}

/* -------------------- Canciones -------------------- */

void ConsoleApp::createSong() {
    std::string name = readLine("Ingresa el nombre de la cancion por favor : ");
    if (name.empty()) { std::cout << "Nombre vacio. Cancelaremos tu operacion :( \n"; return; }
    std::string artist = readLine("Ingresa el nombre del artista: ");
    if (artist.empty()) { std::cout << "Artista vacio. Cancelaremos tu operacion :( \n"; return; }
    std::string durStr = readLine("Ingresa la duracion en formato mm:ss por favor: ");
    int dur = parseDurationToSeconds(durStr);
    if (dur < 0) {
        std::cout << "Formato de duracion invalido. Usa mm:ss o segundos.\n";
        return;
    }
    Song s = service.createSong(name, artist, dur);
    std::cout << "Cancion creada. ID: " << s.id << "\n";
}

void ConsoleApp::listSongs() {
    service.listAllSongs();
}

void ConsoleApp::findSong() {
    std::string name = readSongName("Ingresa el nombre de la cancion por favor: ");
    Song* s = service.findSongByName(name);
    if (!s) std::cout << "Cancion no encontrada....\n";
    else std::cout << "ID: " << s->id << " | " << s->name << " - " << s->artist
        << " (" << formatSecondsToMMSS(s->durationSec) << ")\n";
}

void ConsoleApp::editSong() {
    std::string sname = readSongName("Ingresa el nombre de la cancion a editar: ");
    Song* s = service.findSongByName(sname);
    if (!s) { std::cout << "Cancion no encontrada...\n"; return; }
    std::string newName = readLine("Nuevo nombre: (presiona enter para mantener) ");
    std::string newArtist = readLine("Nuevo artista: (presiona enter para mantener) ");
    std::string durStr = readLine("Nueva duración (en formato mm:ss): (presiona enter para mantener) ");

    std::string finalName = newName.empty() ? s->name : newName;
    std::string finalArtist = newArtist.empty() ? s->artist : newArtist;
    int finalDur = s->durationSec;
    if (!durStr.empty()) {
        int parsed = parseDurationToSeconds(durStr);
        if (parsed < 0) { std::cout << "Formato de duración inválido. Cancelaremos tu edicion :( \n"; return; }
        finalDur = parsed;
    }

    if (service.editSongById(s->id, finalName, finalArtist, finalDur))
        std::cout << "Cancion actualizada.\n";
    else
        std::cout << "No se pudo actualizar la cancion...\n";
}

void ConsoleApp::deleteSong() {
    std::string sname = readSongName("Ingresa el nombre de la cancion a eliminar: ");
    Song* s = service.findSongByName(sname);
    if (!s) { std::cout << "Cancion no encontrada...\n"; return; }
    if (service.deleteSongById(s->id)) std::cout << "Cancion eliminada.\n";
    else std::cout << "No se pudo eliminar la cancion (No logramos encontrarla).\n";
}

/* -------------------- Playlists -------------------- */

void ConsoleApp::createPlaylist() {
    std::string name = readLine("Ingresa el nombre de la playlist: ");
    if (name.empty()) { std::cout << "Nombre vacio. Cancelaremos tu operacion :(\n"; return; }
    int id = service.createPlaylist(name);
    std::cout << "Playlist creada. ID: " << id << "\n";
}

void ConsoleApp::listPlaylists() {
    service.listPlaylists();
}

void ConsoleApp::addSongToPlaylist() {
    std::string pname = readLine("Ingresa el nombre de la playlist: ");
    int pid = service.findPlaylistIdByName(pname);
    if (pid < 0) { std::cout << "Playlist no encontrada...\n"; return; }

    std::string sname = readSongName("Ingresa el nombre de la cancion: ");
    Song* s = service.findSongByName(sname);
    if (!s) { std::cout << "Cancion no encontrada...\n"; return; }

    if (service.addSongToPlaylist(pid, s->id)) std::cout << "Cancion agregada a la playlist.\n";
    else std::cout << "No se pudo agregar (La playlist o cancion no existe, o ya esta agregada).\n";
}

void ConsoleApp::listSongsInPlaylist() {
    std::string pname = readLine("Ingresa el nombre de la playlist: ");
    int pid = service.findPlaylistIdByName(pname);
    if (pid < 0) { std::cout << "Playlist no encontrada...\n"; return; }
    service.listSongsInPlaylist(pid);
}

void ConsoleApp::removeSongFromPlaylist() {
    std::string pname = readLine("Ingresa el nombre de la playlist: ");
    int pid = service.findPlaylistIdByName(pname);
    if (pid < 0) { std::cout << "Playlist no encontrada...\n"; return; }

    std::string sname = readSongName("Ingresa el nombre de la cancion a eliminar de la playlist: ");
    Song* s = service.findSongByName(sname);
    if (!s) { std::cout << "Cancion no encontrada...\n"; return; }

    if (service.removeSongFromPlaylist(pid, s->id)) std::cout << "Cancion eliminada de la playlist.\n";
    else std::cout << "No se pudo eliminar (La playlist o cancion no existe).\n";
}

void ConsoleApp::deletePlaylist() {
    std::string pname = readLine("Ingresa el nombre de la playlist a eliminar: ");
    int pid = service.findPlaylistIdByName(pname);
    if (pid < 0) { std::cout << "Playlist no encontrada...\n"; return; }
    if (service.deletePlaylistById(pid)) std::cout << "Playlist eliminada.\n";
    else std::cout << "No se pudo eliminar la playlist.\n";
}

void ConsoleApp::editPlaylistName() {
    std::string pname = readLine("Ingresa el nombre de la playlist a editar: ");
    int pid = service.findPlaylistIdByName(pname);
    if (pid < 0) { std::cout << "Playlist no encontrada...\n"; return; }
    std::string newName = readLine("Nuevo nombre: ");
    if (newName.empty()) { std::cout << "Nombre vacio. Cancelaremos tu operacion :( \n"; return; }
    if (service.editPlaylistName(pid, newName)) std::cout << "Playlist renombrada.\n";
    else std::cout << "No se encontro la playlist.\n";
}

/* -------------------- Reproduccion -------------------- */

void ConsoleApp::playPlaylistInOrder() {
    std::string pname = readLine("Ingresa el nombre de la playlist a reproducir en orden: ");
    int pid = service.findPlaylistIdByName(pname);
    if (pid < 0) { std::cout << "Playlist no encontrada...\n"; return; }
    auto ids = service.getSongIdsInPlaylist(pid);
    if (ids.empty()) {
        std::cout << "La playlist no existe o no tiene canciones.\n";
        return;
    }
    auto songs = service.getSongsByIds(ids);
    std::cout << "Reproduciendo en orden (simulacion):\n";
    for (const auto& s : songs) {
        std::cout << "Reproduciendo: " << s.name << " - " << s.artist
            << " (" << formatSecondsToMMSS(s.durationSec) << ")\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }
    std::cout << "Reproduccion finalizada.\n";
}

void ConsoleApp::playPlaylistShuffle() {
    std::string pname = readLine("Ingresa el nombre de la playlist a reproducir en aleatorio: ");
    int pid = service.findPlaylistIdByName(pname);
    if (pid < 0) { std::cout << "Playlist no encontrada...\n"; return; }
    auto ids = service.getSongIdsInPlaylist(pid);
    if (ids.empty()) {
        std::cout << "La playlist no existe o no tiene canciones.\n";
        return;
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(ids.begin(), ids.end(), g);

    auto songs = service.getSongsByIds(ids);
    std::cout << "Reproduciendo en aleatorio (simulacion):\n";
    for (const auto& s : songs) {
        std::cout << "Reproduciendo: " << s.name << " - " << s.artist
            << " (" << formatSecondsToMMSS(s.durationSec) << ")\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
        
    }
    std::cout << "Reproduccion aleatoria finalizada.\n";
}




/* -------------------- Utilidades de entrada -------------------- */

std::string ConsoleApp::readLine(const std::string& prompt) const { //Metodo para leer una linea de texto desde la consola, mostrando un prompt. Devuelve la linea ingresada por el usuario.
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

int ConsoleApp::readInt(const std::string& prompt) const { //Si el usuario ingresa algo que no es un numero int, se muestra un mensaje de error y se vuelve a solicitar la entrada.
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        try {
            if (line.empty()) continue;
            int v = std::stoi(line);
            return v;
        }
        catch (...) {
            std::cout << "Entrada invalida. Ingresa un numero entero por favor.\n";
        }
    }
}

std::string ConsoleApp::readSongName(const std::string& prompt) const {
    std::string line = readLine(prompt);
    while (line.empty()) {
        std::cout << "Nombre vacio. Ingresa un nombre valido.\n";
        line = readLine(prompt);
    }
    return line;
}

void ConsoleApp::clearConsole() const {
    std::system("cls");
}

void ConsoleApp::pauseAndClear() const {
    std::system("pause");
    clearConsole();
}
