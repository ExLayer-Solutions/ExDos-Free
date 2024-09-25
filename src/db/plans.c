#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../headers/db/plans.h"

Plan **get_all_plans() {
    Plan **p = (Plan **)malloc(sizeof(Plan *) * 1);

    cFile *fplan = Openfile(PLAN_DB_PATH);
    if(!fplan->fd)
        return NULL;

    fplan->Read(fplan);
    str *raw_data = string(fplan->data);
    Map *json_plans = decode_json(raw_data->data);

    for(int i = 0; i < json_plans->idx; i++) {
      ;  JsonField *field = (JsonField *)((Map *)json_plans)->keys[i];

        if(field->STRUCTURE_PATH->CountChar(field->STRUCTURE_PATH, '/') > 1) {
            // new plan
            printf("%s: %s\n", field->Key->data, field->Value->data);
        }
    }

    return p;
}