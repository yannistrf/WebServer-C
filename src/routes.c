#include "routes.h"

MapRoutes map_routes;

void ParseRoutesFile()
{
    FILE* fp = fopen("routes.conf", "r");
    char* line = NULL;
    size_t len = 0;
    int lines = 0;
    while (getline(&line, &len, fp) != -1)
        lines++;
    free(line); // must free after using getline

    map_routes.lines = lines;
    map_routes.routes = malloc(sizeof(char*) * lines);
    map_routes.templates = malloc(sizeof(char*) * lines);

    rewind(fp);
    line = NULL;
    len = 0;
    int line_cnt = 0;
    while (getline(&line, &len, fp) != -1)
    {
        char* route = strtok(line, ": ");
        printf("route = %s\n", route);
        map_routes.routes[line_cnt] = malloc(sizeof(char) * strlen(route) + 1);
        strcpy(map_routes.routes[line_cnt], route);

        char* template = strtok(NULL, "\n");
        printf("template = %s\n", template);
        map_routes.templates[line_cnt] = malloc(sizeof(char) * strlen(template) + 1);
        strcpy(map_routes.templates[line_cnt], template);

        line_cnt++;
    }
    free(line);
    fclose(fp);

    // TODO: check if template file exists
    for (int i = 0; i < lines; i++)
        printf("%s => %s\n", map_routes.routes[i], map_routes.templates[i]);
}

void DestroyMapRoutes()
{
    for (int i = 0; i < map_routes.lines; i++)
    {
        free(map_routes.routes[i]);
        free(map_routes.templates[i]);
    }

    free(map_routes.routes);
    free(map_routes.templates);
}

const char* GetRouteTemplate(char* route)
{
    for (int i = 0; i < map_routes.lines; i++)
    {
        if (!strcmp(route, map_routes.routes[i]))
            return map_routes.templates[i];
    }

    return NULL;
}
