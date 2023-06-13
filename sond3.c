#include <stdio.h>
#include <portaudio.h>
#include <pthread.h>
#include <sndfile.h>

typedef struct {
    SNDFILE* file;
    SF_INFO fileInfo;
    PaStream* stream;
} SoundData;

void* audioThread(void* arg) {
    SoundData* data = (SoundData*)arg;
    const int BUFSIZE = 1024;
    float buffer[BUFSIZE * data->fileInfo.channels];
    int readCount;

    while ((readCount = sf_readf_float(data->file, buffer, BUFSIZE)) > 0) {
        Pa_WriteStream(data->stream, buffer, readCount);
    }

    sf_close(data->file);
    Pa_StopStream(data->stream);
    Pa_CloseStream(data->stream);
    Pa_Terminate();

    return NULL;
}

void launchAudioThread(const char* filename) {
    // Initialisation de PortAudio
    Pa_Initialize();

    // Ouverture du fichier audio avec libsndfile
    SF_INFO fileInfo;
    SNDFILE* file = sf_open(filename, SFM_READ, &fileInfo);
    if (!file) {
        printf("Impossible de charger le fichier audio.\n");
        return;
    }

    // Configuration de PortAudio
    PaStream* stream;
    Pa_OpenDefaultStream(&stream, 0, fileInfo.channels, paFloat32, fileInfo.samplerate, paFramesPerBufferUnspecified, NULL, NULL);

    // Création des données audio pour le thread
    SoundData soundData;
    soundData.file = file;
    soundData.fileInfo = fileInfo;
    soundData.stream = stream;

    // Démarrage du thread de lecture audio
    pthread_t audioThreadID;
    pthread_create(&audioThreadID, NULL, audioThread, (void*)&soundData);

    // Attendre la fin du thread de lecture audio
    pthread_join(audioThreadID, NULL);
}

int main() {
    const char* filename = "./sus.wav";
    launchAudioThread(filename);

    return 0;
}
