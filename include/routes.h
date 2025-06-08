#pragma once

#include "common.h"

typedef struct
{
    char** routes;
    char** templates;
    int lines;
} MapRoutes;

void ParseRoutesFile();
void DestroyMapRoutes();
const char* GetRouteTemplate(char* route);