#ifndef SONG_H_
#define SONG_H_

#include "sound.h"

typedef enum {done, notDone} SongStatus;

// This structure consists of an array of song notes (song_note_t *notes) and its length (unsigned int note_count)
// Any song can be implemented in such structure.
typedef struct {
    unsigned int note_count;
    song_note_t  notes[100];
} song_t;


// The two functions that initialize the song structure with the notes for these two songs
void InitSongList_Sandman(song_t *);
void InitSongList_Fight(song_t *);

// The blocking version
// input: song structure
// output: nothing
// It plays a song in its entirety while blocking all other operations
void PlaySong_blocking(song_t song);

// The non-blocking version
// input: song structure
// output: bool that represents whether the song is over or not
// It plays a song one note at a time
bool PlaySong_nonblocking(song_t song);


#endif /* SONG_H_ */
