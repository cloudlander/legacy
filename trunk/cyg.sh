#!/bin/bash



sed -e "s/printf/_printf/g" | sed -e "s/malloc/_malloc/g" | sed -e "s/getchar/_getchar/g" | sed -e "s/free/_free/g" | sed -e "s/realloc/_realloc/g" | sed -e "s/atoi/_atoi/g" | sed -e "s/strcmp/_strcmp/g" | \

sed -e "s/exit/_exit/g" | sed -e "s/\.comm _exception_value,4,4/\.comm _exception_value,4/g" | sed -e "s/\.comm _exception_type,4,4/\.comm _exception_type,4/g" | sed -e "s/\.comm _exception_dimension,4,4/\.comm _exception_value,4/g" | sed -e "/\.type/ d" #| sed -e "s/main/_main/"

