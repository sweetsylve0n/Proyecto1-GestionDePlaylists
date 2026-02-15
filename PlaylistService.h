#pragma once

#include "Song.h"
#include "SinglyLinkedList.h"
#include <string>
#include <vector>

class PlaylistService {
public:
    PlaylistService();
    ~PlaylistService();

    // (RF-01, RF-02, RF-03, RF-04, RF-05)
	Song createSong(const std::string& name, const std::string& artist, int durationSec); //usamos el std:: como consejo de buenas practicas para evitar confusiones con otras librerias
    void listAllSongs() const;

    Song* findSongById(int id);
    Song* findSongByName(const std::string& name);
    bool editSongById(int id, const std::string& newName, const std::string& newArtist, int newDurationSec);
    bool deleteSongById(int id);

    // (RF-06..RF-13)
    int createPlaylist(const std::string& name);
    bool addSongToPlaylist(int playlistId, int songId);
    bool removeSongFromPlaylist(int playlistId, int songId);
    void listPlaylists() const;
    void listSongsInPlaylist(int playlistId) const;
    bool deletePlaylistById(int playlistId);

    // Estado 
    bool hasSongs() const;
    std::vector<int> getSongIdsInPlaylist(int playlistId) const;
	std::vector<Song> getSongsByIds(const std::vector<int>& ids) const; // Devuelve una lista de canciones dadon un vector de ids

    // Editar playlist (renombrar)
    bool editPlaylistName(int playlistId, const std::string& newName);

    //Buscar por nombre por intuicion del usuario
    int findPlaylistIdByName(const std::string& name) const;

    private:
        struct PlaylistInternal {
            int id;
            std::string name;
            SinglyLinkedList<int> songIds;
            PlaylistInternal() : id(0), name("") {}
		    PlaylistInternal(int _id, const std::string& _name) : id(_id), name(_name) {} // Constructor para inicializar id y nombre de la playlist
        };

        SinglyLinkedList<Song> songs;
	    SinglyLinkedList<PlaylistInternal> playlists; //Manejamos las playlists con una estructura interna que incluye la lista de ids de canciones 
        int nextSongId;
        int nextPlaylistId;

        // Helpers
        PlaylistInternal* findPlaylistById(int id);
        void removeSongReferences(int songId); 
    };