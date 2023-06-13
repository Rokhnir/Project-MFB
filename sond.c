#include <stdio.h>
#include <portaudio.h>
#include <pthread.h>
#include <sndfile.h>

#define FILENAME "./sus.wav"

typedef struct {
    SNDFILE* file;
    SF_INFO fileInfo;
    PaStream* stream;
} SoundData;

void* loopThread(void* arg) {
    while (1) {
        printf("Boucle\n");
        // Effectuez ici les opérations de la boucle
    }
    return NULL;
}

int main() {
    // Initialisation de PortAudio
    Pa_Initialize();

    // Ouverture du fichier audio avec libsndfile
    SF_INFO fileInfo;
    SNDFILE* file = sf_open(FILENAME, SFM_READ, &fileInfo);
    if (!file) {
        printf("Impossible de charger le fichier audio.\n");
        return 1;
    }

    // Configuration de PortAudio
    PaStream* stream;
    Pa_OpenDefaultStream(&stream, 0, fileInfo.channels, paFloat32, fileInfo.samplerate, paFramesPerBufferUnspecified, NULL, NULL);

    // Démarrage du thread de boucle
    pthread_t thread;
    pthread_create(&thread, NULL, loopThread, NULL);

    // Lecture de la musique
    Pa_StartStream(stream);
    Pa_Sleep(500);  // Temps de démarrage pour éviter les artefacts audio

    // Lecture et conversion du contenu du fichier
    const int BUFSIZE = 1024;
    float buffer[BUFSIZE * fileInfo.channels];
    int readCount;
    while ((readCount = sf_readf_float(file, buffer, BUFSIZE)) > 0) {
        Pa_WriteStream(stream, buffer, readCount);
    }

    // Fermeture du fichier audio et de PortAudio
    sf_close(file);
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    // Attendre la fin du thread
    pthread_join(thread, NULL);

    return 0;
}
