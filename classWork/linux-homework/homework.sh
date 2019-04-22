#!/bin/bash
#read -p "input value:" var;
#if [[$var==q|| $var==Q]];
#then
#	exit;
#fi
for((i=1;i<=$#;i++));
do
	echo $i|sed 's/\.\|-\|+\|%\|\^//g'|grep [^0-9] >/dev/null && echo parameter $i is not a number || echo $i is a number	
done
