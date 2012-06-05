#!/gbin/bash

for b in `git for-each-ref --format="%(refname)" refs/heads/ | sed 's/refs\/heads\///g'`
do 
	echo $b
	(git clean -dxf && git checkout $b && bash ../stitch.sh ) || exit 1
done

git prune -v && git gc

