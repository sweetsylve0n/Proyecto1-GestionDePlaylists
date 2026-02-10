#pragma once
#include "PlaylistService.h"
#include <string>

//Aqui instanciamos los services y la logica, gracias a esto mantenemos el main limpio

class ConsoleApp {
public:
    ConsoleApp();
	~ConsoleApp(); // Constructor y destructor
    void run();

private:
    PlaylistService service;

    // Menú y manejo de opciones
    void showMainMenu() const;
    void handleOption(int opt);

    // Operaciones relacionadas con canciones
    void createSong();
    void listSongs();
    void findSong();
    void editSong();
    void deleteSong();

    // Operaciones relacionadas con playlists
    void createPlaylist();
    void listPlaylists();
    void addSongToPlaylist();
    void listSongsInPlaylist();
    void removeSongFromPlaylist();
    void deletePlaylist();
    void editPlaylistName();

    // Reproduccion
    void playPlaylistInOrder();
    void playPlaylistShuffle();

    // Utilidades de entrada
    std::string readLine(const std::string& prompt) const;
    int readInt(const std::string& prompt) const;
};
