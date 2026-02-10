#include "PlaylistService.h"
#include "DurationUtils.h"
#include <iostream>
#include <algorithm> 
#include <vector> 

using namespace std; //Aqui si usamos el namespace std para evitar escribirlo cada vez, ya que esta clase hace un uso intensivo de elementos de la libreria estandar como string, vector, cout, y demas

PlaylistService::PlaylistService() : nextSongId(1), nextPlaylistId(1) {}
PlaylistService::~PlaylistService() = default; //Constructor y destructor 

Song PlaylistService::createSong(const string& name, const string& artist, int durationSec) {
    Song s(nextSongId++, name, artist, durationSec);
    songs.push_back(std::move(s));

    Song* stored = songs.findById(nextSongId - 1);
	if (stored) return *stored; // Devolvemos la canción recién creada desde la lista para asegurar que el ID es correcto y que se ha almacenado correctamente

    Song fallback;
    fallback.id = nextSongId - 1;
    fallback.name = name;
    fallback.artist = artist;
    fallback.durationSec = durationSec;
    return fallback; 
}

void PlaylistService::listAllSongs() const {
    if (songs.isEmpty()) {
        cout << "No hay canciones registradas.\n";
        return;
    }
    songs.traverse([](const Song& s) {
        cout << "ID: " << s.id << " | " << s.name << " - " << s.artist 
            << " (" << formatSecondsToMMSS(s.durationSec) << ")\n"; 
        });
}

Song* PlaylistService::findSongById(int id) { // Buscamos la canción por ID y devolvemos un puntero a la canción si se encuentra y nullptr si no se encuentra
    return songs.findById(id);
}

bool PlaylistService::editSongById(int id, const string& newName, const string& newArtist, int newDurationSec) {
    Song* s = songs.findById(id);
    if (!s) return false;
    s->name = newName;
    s->artist = newArtist;
    s->durationSec = newDurationSec;
    return true;
}

// Helper
void PlaylistService::removeSongReferences(int songId) {
    playlists.forEach([songId](PlaylistInternal& p) {
		p.songIds.removeIf([songId](const int& id) { return id == songId; }); // Eliminamos la referencia a la canción en cada playlist
        });
}

bool PlaylistService::deleteSongById(int id) {
    removeSongReferences(id);

    return const_cast<SinglyLinkedList<Song>&>(songs).removeById(id);
}

int PlaylistService::createPlaylist(const string& name) {
    PlaylistInternal p(nextPlaylistId++, name);
	playlists.push_back(std::move(p)); //Usamos el move para evitar copias innecesarias 
    return nextPlaylistId - 1;
}

// Helper
PlaylistService::PlaylistInternal* PlaylistService::findPlaylistById(int id) {
	return playlists.findIf([id](const PlaylistInternal& p) -> bool { // Buscamos la playlist por ID y devolvemos un puntero a la playlist si se encuentra y nullptr si no se encuentra
        return p.id == id;
        });
}

bool PlaylistService::addSongToPlaylist(int playlistId, int songId) {
    Song* s = songs.findById(songId); 
    if (!s) return false;

    PlaylistInternal* p = findPlaylistById(playlistId);
    if (!p) return false;

    int* found = p->songIds.findIf([songId](const int& id) { return id == songId; });
    if (found) return false; 
	p->songIds.push_back(songId); // Agregamos la canción a la playlist, aqui no usamos el move porque estamos agregando un int
    return true;
}

bool PlaylistService::removeSongFromPlaylist(int playlistId, int songId) {
    PlaylistInternal* p = findPlaylistById(playlistId);
    if (!p) return false;
    return p->songIds.removeIf([songId](const int& id) { return id == songId; });
}

void PlaylistService::listPlaylists() const {
    if (playlists.isEmpty()) {
        cout << "No hay listas de Reproduccion.\n";
        return;
    }
    playlists.traverse([](const PlaylistInternal& p) {
        cout << "Playlist ID: " << p.id << " | " << p.name << " | Canciones: " << p.songIds.size() << "\n";
        });
}

//Resto de funciones crud de playlists

void PlaylistService::listSongsInPlaylist(int playlistId) const {
    bool found = false;
    playlists.traverse([&](const PlaylistInternal& p) {
        if (p.id == playlistId) {
            found = true;
            if (p.songIds.isEmpty()) {
                cout << "La lista no tiene canciones.\n";
                return;
            }
            p.songIds.traverse([&](const int& sid) {
                Song* s = const_cast<SinglyLinkedList<Song>&>(songs).findById(sid);
                if (s) {
                    cout << "ID: " << s->id << " | " << s->name << " - " << s->artist
                        << " (" << formatSecondsToMMSS(s->durationSec) << ")\n";
                }
                else {
                    cout << "ID: " << sid << " | (Cancion no encontrada en el sistema)\n";
                }
                });
        }
        });
    if (!found) cout << "Playlist con ID " << playlistId << " no encontrada.\n";
}

bool PlaylistService::deletePlaylistById(int playlistId) {
    return const_cast<SinglyLinkedList<PlaylistInternal>&>(playlists).removeIf([playlistId](const PlaylistInternal& p) {
        return p.id == playlistId;
        });
}

bool PlaylistService::hasSongs() const {
    return !songs.isEmpty();
}

std::vector<int> PlaylistService::getSongIdsInPlaylist(int playlistId) const {
    std::vector<int> result;
    playlists.traverse([&](const PlaylistInternal& p) {
        if (p.id == playlistId) {
            p.songIds.traverse([&](const int& sid) {
                result.push_back(sid);
                });
        }
        });
    return result;
}

std::vector<Song> PlaylistService::getSongsByIds(const std::vector<int>& ids) const {
    std::vector<Song> result;
    for (int id : ids) {
        Song* s = const_cast<SinglyLinkedList<Song>&>(songs).findById(id);
        if (s) result.push_back(*s); 
    }
    return result;
}

bool PlaylistService::editPlaylistName(int playlistId, const std::string& newName) {
    PlaylistInternal* p = findPlaylistById(playlistId);
    if (!p) return false;
    p->name = newName;
    return true;
}