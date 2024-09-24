DIS=./build/x86_disasm

case $1 in
  -d)
    flags="-d"
    shift
    ;;
esac

dis() {
	echo "\$ ${DIS} ${flags} $*"
	${DIS} ${flags} $*
}

dis 4c 13 3d ad de ad 1e
dis 4c bf 01 02 03 04 05 06 07 08
dis 49 0f c7 0f
dis c5 7f e6 3d 40 84 c8 0c
dis 62 f2 7d 48 7e 15 11 22 33 44
