#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void play_sound(const char *filename) {
    char command[256];
    sprintf(command, "aplay %s -d 10", filename);
    system(command);
}

void *play_sound_parallel(void *filename) {
    play_sound((const char *)filename);

    pthread_exit(NULL);
}

int main() {
    const char *filename = "sus.wav";

    pthread_t thread;
    pthread_create(&thread, NULL, play_sound_parallel, (void *)filename);
    pthread_detach(thread);

    for (int i = 0; i < 100; ++i) {
        printf("SUSU");
    }
    int  a;
    scanf("%d",&a);
    return 0;
}
