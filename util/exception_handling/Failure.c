//
// Created by Aimen Fahmi on 2020-03-28.
//

#include "Failure.h"

#include <stdio.h>
#include <stdlib.h>

void manageMallocFailures(void *pointer, const char *name) {
    if (!pointer) {
        printf("Memory allocation of %s failed", name);
        exit(-1);
    }
}