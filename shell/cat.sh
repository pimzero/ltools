function cat() {
	local IFS=
	for j in $@; do
		while read i; do
			echo $i;
		done <$j;
	done
}
