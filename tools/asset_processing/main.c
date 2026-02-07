#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "cjson/cJSON.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int offset_x, offset_y, width, height;
} FrameData;

int main() {
    // Carrega imagem
    int img_width, img_height, channels;
    unsigned char* img = stbi_load("../player.png", &img_width, &img_height, &channels, 4);
    if (!img) {
        printf("Erro ao carregar imagem\n");
        return 1;
    }

    int frame_width = 32;
    int frame_height = 32;
    int frames_x = img_width / frame_width;
    int frames_y = img_height / frame_height;

    cJSON* root = cJSON_CreateArray(); // Array principal de animações

    for (int fy = 0; fy < frames_y; fy++) {
        cJSON* animObj = cJSON_CreateObject();
        cJSON_AddStringToObject(animObj, "NomeAnimacao", ""); // Nome vazio

        cJSON* framesArr = cJSON_CreateArray(); // Frames da linha
        int realFrames = 0;

        for (int fx = 0; fx < frames_x; fx++) {
            int min_x = frame_width, min_y = frame_height, max_x = 0, max_y = 0;
            int empty = 1;

            for (int y = 0; y < frame_height; y++) {
                for (int x = 0; x < frame_width; x++) {
                    int px = fx * frame_width + x;
                    int py = fy * frame_height + y;
                    int offset = (py * img_width + px) * 4; // RGBA
                    unsigned char a = img[offset + 3];
                    if (a != 0) {
                        empty = 0;
                        if (x < min_x) min_x = x;
                        if (y < min_y) min_y = y;
                        if (x > max_x) max_x = x;
                        if (y > max_y) max_y = y;
                    }
                }
            }

            if (empty) continue; // ignora frame vazio

            FrameData f = {min_x, min_y, max_x - min_x + 1, max_y - min_y + 1};
            cJSON* fobj = cJSON_CreateObject();
            cJSON_AddNumberToObject(fobj, "offset_x", f.offset_x);
            cJSON_AddNumberToObject(fobj, "offset_y", f.offset_y);
            cJSON_AddNumberToObject(fobj, "width", f.width);
            cJSON_AddNumberToObject(fobj, "height", f.height);

            cJSON_AddItemToArray(framesArr, fobj);
            realFrames++;
        }

        cJSON_AddNumberToObject(animObj, "QtdSprites", realFrames);
        cJSON_AddItemToObject(animObj, "frames", framesArr);
        cJSON_AddItemToArray(root, animObj);
    }

    char* json_str = cJSON_Print(root);
    FILE* f = fopen("player.json", "w");
    fprintf(f, "%s", json_str);
    fclose(f);

    cJSON_Delete(root);
    stbi_image_free(img);
    free(json_str);

    return 0;
}
