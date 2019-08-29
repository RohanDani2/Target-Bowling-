#include "sound.h"
#include "song.h"

//100ms is equivalent to a 1/16th note based on 150BPM (beats per minute)
#define DURATION 100

// TODO: Implement this function
bool PlaySong_nonblocking(song_t song)
{
    static int i = 0;

    while (i < song.note_count)
    {
        if (PlayNote_nonblocking(song.notes[i]))
        {
            i++;
            return false;
        }
        else
        {
            return false;
        }

    }
    i = 0;
    return true;

}

// This function does not need any change
void PlaySong_blocking(song_t song)
{
    int i = 0;
    for (i = 0; i < song.note_count; i++)
        PlayNote_blocking(song.notes[i]);
}

void InitSongList_Sandman(song_t *song_p)
{
    // ENTER SANDMAN OPENING RIFF
    song_p->note_count = 10;

    song_p->notes[1].note_name = note_f4S;
    song_p->notes[1].note_length = 4 * DURATION;

    song_p->notes[2].note_name = note_c5;
    song_p->notes[2].note_length = 2 * DURATION;

    song_p->notes[3].note_name = note_c4;
    song_p->notes[3].note_length = 5 * DURATION;

    song_p->notes[4].note_name = note_c5;
    song_p->notes[4].note_length = 2 * DURATION;

    song_p->notes[5].note_name = note_e5;
    song_p->notes[5].note_length = 2 * DURATION;

    song_p->notes[6].note_name = note_g4;
    song_p->notes[6].note_length = 2 * DURATION;

    song_p->notes[7].note_name = note_f4S;
    song_p->notes[7].note_length = 4 * DURATION;

    song_p->notes[8].note_name = note_c5;
    song_p->notes[8].note_length = 2 * DURATION;

    song_p->notes[9].note_name = note_c4;
    song_p->notes[9].note_length = 5 * DURATION;

    song_p->notes[10].note_name = note_c5;
    song_p->notes[10].note_length = 2 * DURATION;
//
}

void InitSongList_Fight(song_t *song_p)
{

    song_p->note_count = 22;

    song_p->notes[0].note_name = note_silent;
    song_p->notes[0].note_length = 1 * DURATION;

    song_p->notes[1].note_name = note_g4;
    song_p->notes[1].note_length = 2 * DURATION;

    song_p->notes[2].note_name = note_a4;
    song_p->notes[2].note_length = 2 * DURATION;

    song_p->notes[3].note_name = note_b4;
    song_p->notes[3].note_length = 2 * DURATION;

    song_p->notes[4].note_name = note_c5;
    song_p->notes[4].note_length = DURATION;

    song_p->notes[5].note_name = note_silent;
    song_p->notes[5].note_length = DURATION;

    song_p->notes[6].note_name = note_c5;
    song_p->notes[6].note_length = 6 * DURATION;

    song_p->notes[7].note_name = note_e5;
    song_p->notes[7].note_length = DURATION;

    song_p->notes[8].note_name = note_silent;
    song_p->notes[8].note_length = DURATION;

    song_p->notes[9].note_name = note_e5;
    song_p->notes[9].note_length = 6 * DURATION;

    song_p->notes[10].note_name = note_f5;
    song_p->notes[10].note_length = DURATION;

    song_p->notes[11].note_name = note_silent;
    song_p->notes[11].note_length = DURATION;

    song_p->notes[12].note_name = note_f5;
    song_p->notes[12].note_length = 6 * DURATION;

    song_p->notes[13].note_name = note_f5S;
    song_p->notes[13].note_length = 8 * DURATION;

    song_p->notes[14].note_name = note_g5;
    song_p->notes[14].note_length = 2 * DURATION;

    song_p->notes[15].note_name = note_g4;
    song_p->notes[15].note_length = 2 * DURATION;

    song_p->notes[16].note_name = note_a4;
    song_p->notes[16].note_length = 2 * DURATION;

    song_p->notes[17].note_name = note_c5;
    song_p->notes[17].note_length = 2 * DURATION;

    song_p->notes[18].note_name = note_d5;
    song_p->notes[18].note_length = 2 * DURATION;

    song_p->notes[19].note_name = note_c6;
    song_p->notes[19].note_length = 10 * DURATION;

    song_p->notes[20].note_name = note_silent;
    song_p->notes[20].note_length = 2 * DURATION;

    song_p->notes[21].note_name = note_c6;
    song_p->notes[21].note_length = DURATION;

    song_p->notes[22].note_name = note_silent;
    song_p->notes[22].note_length = 1 * DURATION;

}

