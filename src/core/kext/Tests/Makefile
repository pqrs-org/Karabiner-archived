all:
	@for d in `find * -type d`; do echo "[Test] $$d"; make -C $$d run || exit 1; done

clean:
	@for d in `find * -type d`; do make -C $$d clean; done
