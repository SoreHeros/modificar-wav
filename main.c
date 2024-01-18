#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char chunkId[4];
    unsigned int chunkSize;
    char format[4];
    char subchunk1ID[4];
    unsigned int subchunk1Size;
    unsigned short int audioFormat;
    unsigned short int numChanels;
    unsigned int sampleRate;
    unsigned int byteRate;
    unsigned short int blockAlign;
    unsigned short int bitsPerSample;
    char subchunk2ID[4];
    unsigned int subchunk2Size;
    void * data;
}wav;


wav * cargar_wav(char * filename){

    FILE * archivo;
    archivo = fopen(filename, "rb");

    if(archivo == NULL){
        printf("ERROR: Archivo no encontrado");
        return NULL;
    }


    wav * audio = malloc(sizeof(wav));

    printf("\n\n\n\n");

    printf("\nChunkID:\t");
    fread(audio->chunkId, 1, 4, archivo);
    printf("%4s", audio->chunkId);


    printf("\nChunkSize:\t");
    fread(&audio->chunkSize, 4, 1, archivo);
    printf("%u", audio->chunkSize);

    printf("\nFormat: \t");
    fread(audio->format, 1, 4, archivo);
    printf("%4s", audio->format);

    printf("\n");

    printf("\nSubchunk1Id:\t");
    fread(audio->subchunk1ID, 1, 4, archivo);
    printf("%4s", audio->subchunk1ID);

    printf("\nSubchunk1size:\t");
    fread(&audio->subchunk1Size, 4, 1, archivo);
    printf("%u", audio->subchunk1Size);

    printf("\nAudioFormat:\t");
    fread(&audio->audioFormat, 2, 1, archivo);
    printf("%u", audio->audioFormat);

    printf("\nNumChanels:\t");
    fread(&audio->numChanels, 2, 1, archivo);
    printf("%u", audio->numChanels);

    printf("\nSampleRate:\t");
    fread(&audio->sampleRate, 4, 1, archivo);
    printf("%u", audio->sampleRate);

    printf("\nByteRate:\t");
    fread(&audio->byteRate, 4, 1, archivo);
    printf("%u", audio->byteRate);

    printf("\nBlockAlign:\t");
    fread(&audio->blockAlign, 2, 1, archivo);
    printf("%u", audio->blockAlign);

    printf("\nBitsPerSample:\t");
    fread(&audio->bitsPerSample, 2, 1, archivo);
    printf("%u", audio->bitsPerSample);

    printf("\n");

    printf("\nSubchunk2Id:\t");
    fread(audio->subchunk2ID, 1, 4, archivo);
    printf("%4s", audio->subchunk2ID);

    printf("\nSubchunk2size:\t");
    fread(&audio->subchunk2Size, 4, 1, archivo);
    printf("%u", audio->subchunk2Size);

    printf("\nData:      \t");
    audio->data = malloc(audio->subchunk2Size);

    fread(audio->data, audio->bitsPerSample / 8, audio->subchunk2Size / (audio->bitsPerSample / 8), archivo);

    printf("cargada\n");

    fclose(archivo);

    return audio;
}

void guardar_wav(wav * audio){
    char * nombre = malloc(1000);
    FILE * archivo;

    printf("\nIntroduce el nombre del archivo guardado\n");
    scanf("%s", nombre);

    archivo = fopen(nombre, "wb");

    fwrite(audio->chunkId, 1, 4, archivo);
    fwrite(&audio->chunkSize, 4, 1, archivo);
    fwrite(audio->format, 1, 4, archivo);
    fwrite(audio->subchunk1ID, 1, 4, archivo);
    fwrite(&audio->subchunk1Size, 4, 1, archivo);
    fwrite(&audio->audioFormat, 2, 1, archivo);
    fwrite(&audio->numChanels, 2, 1, archivo);
    fwrite(&audio->sampleRate, 4, 1, archivo);
    fwrite(&audio->byteRate, 4, 1, archivo);
    fwrite(&audio->blockAlign, 2, 1, archivo);
    fwrite(&audio->bitsPerSample, 2, 1, archivo);
    fwrite(audio->subchunk2ID, 1, 4, archivo);
    fwrite(&audio->subchunk2Size, 4, 1, archivo);
    fwrite(audio->data, audio->bitsPerSample / 8, audio->subchunk2Size / (audio->bitsPerSample / 8), archivo);

    fclose(archivo);
    free(nombre);
}

void cerrar_wav(wav * audio){
    printf("\ncerrando wav...\n");
    if(audio == NULL) {
        printf("El wav no existe\n");
        return;
    }

    free(audio->data);
    free(audio);

    printf("wav cerrado con exito!\n");
}

void atrasar_wav(wav * audio){
    printf("Introduce los segundos que retrasar el archivo\n");
    double segundos;
    scanf("%lf", &segundos);

    void * new_data = malloc(((unsigned long long int)(segundos * audio->byteRate * audio->numChanels)) + audio->subchunk2Size);

    memset(new_data, 0,((unsigned long long int)(segundos * audio->byteRate * audio->numChanels)));

    memcpy(new_data + ((unsigned long long int)(segundos * audio->byteRate * audio->numChanels)), audio->data, audio->subchunk2Size);

    free(audio->data);
    audio->data = new_data;

    audio->subchunk2Size = ((unsigned long long int)(segundos * audio->byteRate * audio->numChanels)) + audio->subchunk2Size;

    audio->chunkSize = 36 + audio->subchunk2Size;
}

void alargar_wav(wav * audio){
    printf("Introduce los segundos que alargar el archivo\n");
    double segundos;
    scanf("%lf", &segundos);

    void * new_data = malloc(((unsigned long long int)(segundos * audio->byteRate * audio->numChanels)) + audio->subchunk2Size);

    memset(new_data + audio->subchunk2Size, 0,((unsigned long long int)(segundos * audio->byteRate * audio->numChanels)));

    memcpy(new_data, audio->data, audio->subchunk2Size);

    free(audio->data);
    audio->data = new_data;

    audio->subchunk2Size = ((unsigned long long int)(segundos * audio->byteRate * audio->numChanels)) + audio->subchunk2Size;

    audio->chunkSize = 36 + audio->subchunk2Size;
}

void to_mono_wav(wav * audio){
    //todo
}

void to_stereo_wav(wav * audio){
    //todo
}

int main(){

    wav * audio = NULL;
    char * menu = malloc(100);

    do {
        if (audio == NULL) {
            printf("\nIntroduzca el nombre del archivo a cargar (o exit para salir)\n");
            scanf("%s", menu);
            audio = cargar_wav(menu);
        } else {

            printf("\nIntroduce lo que quieres hacer:\n");
            printf("cerrar: cerrar el archivo para abrir otro\n");
            printf("guardar: guarda un archivo\n");
            printf("atrasar: atrasa el inicio del archivo\n");
            printf("alargar: alarga el final del archivo\n");
            printf("mono: convierte un archivo a mono\n");
            printf("stereo: convierte un archivo a stereo\n");
            printf("exit: salir\n");

            scanf("%s", menu);

            if (!strcmp(menu, "cerrar")) {
                cerrar_wav(audio);
                audio = NULL;
            } else if (!strcmp(menu, "guardar"))
                guardar_wav(audio);
            else if (!strcmp(menu, "atrasar"))
                atrasar_wav(audio);
            else if (!strcmp(menu, "alargar"))
                alargar_wav(audio);
            else if (!strcmp(menu, "mono"))
                to_mono_wav(audio);
            else if (!strcmp(menu, "stereo"))
                to_stereo_wav(audio);
        }

    }while(strcmp(menu, "exit"));

    cerrar_wav(audio);
}