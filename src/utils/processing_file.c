#include "./utils.h"

open_file_t open_file(const char *path) {
    char *real_path = realpath(path, NULL);
    open_file_t open_file;
    FILE *file;
    file = fopen(real_path, "rb");

    if (file == NULL){
        printf("Error opening file");
        open_file.content = NULL;
        return open_file;
    }

    fseek(file, 0, SEEK_END);
    open_file.size = ftell(file);
    open_file.content = malloc(sizeof(char) * open_file.size);
    fseek(file,0,SEEK_SET);
    fread(open_file.content,open_file.size,1,file);
    fclose(file);

    return open_file;
}

file_t *processing_file(config_t *config, const char* path) {
    file_t *f = malloc(sizeof(file_t));
    DIR *dir;
    DIR *dir2;
    f->path = path;
    dir = opendir(path);
    dir2 = opendir(path);
    struct dirent *dp;

    if(!strcmp(get_extension(path),"")) {
        //DIRECTORY
        if(dir) {
            int length = 0;
            while ((dp = readdir(dir)) != NULL) {
                length += strlen(dp->d_name) + 1;
            }
            char *tmp_content = malloc(sizeof(char) * length);
            while ((dp = readdir(dir2)) != NULL) {
                strcat(tmp_content, dp->d_name);
                strcat(tmp_content, "\n");
            }
            f->content = tmp_content;
            f->status = "200";

        }
        else {
            f->status = "404";
        }
    }
    else {
        f->content_type = get_content_type_by_extension(config, get_extension(path));
        if (f->content_type == NULL) {
            f->status = "415";
            f->content = "Unsupported Media type";
        } else {
            open_file_t _open_file = open_file(path);
            if (_open_file.content == NULL) {
                f->status = "404";
                f->content_type = get_content_type_by_extension(config, get_extension("default"));
            } else {
                f->status = "200";
                f->content = _open_file.content;
                f->content_size = _open_file.size;
            }
        }
    }

    return f;
}

char *get_extension(const char *path) {
    int has_slash = 0;
    char *l_path;
    for (int i = 0; path[i] != '\0'; i++) {
        if (path[i] == '/') {
            has_slash = 1;
        }
    }
    if (has_slash) {
        l_path = strrchr(path, '/');
    }
    char *ext = strrchr(has_slash ? l_path : path, '.');
    if(!ext || ext == path) return "";
    return ext + 1;
}