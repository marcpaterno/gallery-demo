#/bin/bash

for p in $(ups active | tail -n +2 | cut -f 1 -d ' '  | tr [:lower:] [:upper:])
do 
  du -sk $(printenv ${p}_DIR)
done


