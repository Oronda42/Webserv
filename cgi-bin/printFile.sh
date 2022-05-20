#!bin/sh

CAT=/bin/cat

echo Content-type: text/plain
echo ""

if [[ -x $CAT]]
then
		$CAT $1 | sort
else
		echo cannot find command 
fi
