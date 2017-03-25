function ls() {
arg=$@
if [ $# -eq 0 ]; then
	arg=$(echo *)
fi
for i in $arg; do
	if [ -d $i ]; then
		echo -n 'd';
	elif [ -u $i ]; then
		echo -n 's';
	else
		echo -n '-';
	fi;
	if [ -r $i ]; then echo -n 'r'; else echo -n '-'; fi;
	if [ -w $i ]; then echo -n 'w'; else echo -n '-'; fi;
	if [ -x $i ]; then echo -n 'x'; else echo -n '-'; fi;
	echo " $i";
done
}

function cat() {
	local IFS=
	for j in $@; do
		while read i; do
			echo $i;
		done <$j;
	done
}
