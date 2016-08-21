#!/bin/bash

echo "Comentarios do commit: "
read MSG

git add .
git commit -m "$MSG"
git push -u origin master
