#!/bin/sh
username=root
password=iamxgl

scripts=`find -name "*.sql"`
for script in $scripts
do
    mysql -u$username -p$password < $script
    echo $script done
done

echo init database done
