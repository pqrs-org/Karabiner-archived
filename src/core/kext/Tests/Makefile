all:
	@for d in `find * -type d`; do echo "[Test] $$d"; make -C $$d run; done

clean:
	@for d in `find * -type d`; do make -C $$d clean; done
