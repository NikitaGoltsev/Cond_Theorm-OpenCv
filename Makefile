format:
	clang-format -i *.c

git_avg:
	git add *
	git commit -m "avg_save"
	git push

