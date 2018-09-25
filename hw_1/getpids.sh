#!/bin/zsh

name=$(whoami)

ps aux | grep -v $name | grep -v grep | cut -c10-14 

