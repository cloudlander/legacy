#!/bin/bash



sed -e "s/printf/_printf/g" | sed -e "s/malloc/_malloc/g" | \

sed -e "s/exit/_exit/g" | sed -e "s/\.comm _exception_value,4,4/\.comm _exception_value,4/g" | sed -e "s/\.comm _exception_type,4,4/\.comm _exception_type,4/g" | sed -e "s/\.comm _exception_dimension,4,4/\.comm _exception_value,4/g" | sed -e "/\.type/ d" | sed -e "s/main/_main/"

