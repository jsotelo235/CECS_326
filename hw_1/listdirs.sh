#!/bin/zsh

cd /usr

foreach directory ( lib bin src)
 echo "Current directory is: $directory \n"
 cd $directory/
 ls -l
 cd ..
end


