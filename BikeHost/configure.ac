AC_INIT(BikeHost,1.0,[support@deepsoft.com], BikeHost)
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_SUBST(build)
AC_CANONICAL_TARGET
AC_SUBST(target)
AC_CONFIG_SRCDIR(Scripts/BikeHost.tcl)
AM_INIT_AUTOMAKE([-Wno-portability])
        
AC_CONFIG_MACRO_DIR([m4])
if test "x${prefix}" = "xNONE"; then
    prefix=/usr/local
fi
if test "x${exec_prefix}" = "xNONE"; then
    exec_prefix="${prefix}"
fi
        
BUILD_DIR=`pwd`
TOP_DIR=${BUILD_DIR}

AC_SUBST(TOP_DIR)
AC_SUBST(BUILD_DIR)

srcdir=`dirname $0`
cd $srcdir
SRC_DIR=`pwd`
cd $BUILD_DIR

AC_SUBST(SRC_DIR)
AC_SUBST(target)

m4_pattern_allow(PKG_)
AX_PKG_DOXYGEN(1.6.0, [], [ AC_MSG_ERROR([DOXYGEN 1.6.0 or later is required to build..]) ])


AC_PATH_PROG(PDFLATEX,pdflatex,/bin/true)
AC_PATH_PROGS(HTLATEX,[htlatex htlatex.sh],/bin/true)
AC_PATH_PROG(LATEX,latex,/bin/true)
AC_PATH_PROG(MAKEINDEX,makeindex,/bin/true)
AC_PATH_PROG(BIBTEX,bibtex,/bin/true)
AC_PATH_PROG(DVIPS,dvips,/bin/true)
AC_PATH_PROG(PS2PDF,ps2pdf,/bin/true)


TEA_INIT([3.5])
TEA_PATH_TCLCONFIG
TEA_LOAD_TCLCONFIG
TEA_PROG_TCLSH
dnl Check for external Tcl/Tk libraries
AX_SNIT
AC_MSG_CHECKING([Checking target tclkit])
case $target_cpu in 
    x86_64)
        AC_SUBST(TCLKITRUN, [tclkit-8.6.11-x86_64-Img1.4.7])
        ;;
    i*86)
        AC_SUBST(TCLKITRUN, [tclkit-8.6.11-i386-Img1.4.7])
        ;;
    arm*)
        AC_SUBST(TCLKITRUN, [tclkit-8.6.11-armv7l-Img1.4.8])
        ;;
    aarch64)
        AC_SUBST(TCLKITRUN, [tclkit-8.6.10-aarch64-Img1.4.9])
        ;;
    *)
        AC_MSG_ERROR([Unsupported Linux arch: $target_cpu])
        ;;
esac
AC_MSG_RESULT([TCLKITRUN is $TCLKITRUN])
                                        
AC_MSG_CHECKING([Checking build tclkit])
case $build_cpu in
      x86_64)
        AC_SUBST(TCLKITBSRC, [tclkit-8.6.11-x86_64-Img1.4.7])
        ;;
      i*86)
        AC_SUBST(TCLKITBSRC, [tclkit-8.6.11-i386-Img1.4.7])
        ;;
      arm*)
        AC_SUBST(TCLKITBSRC, [tclkit-8.6.11-armv7l-Img1.4.8])
        ;;
      aarch64)
        AC_SUBST(TCLKITBSRC, [tclkit-8.6.10-aarch64-Img1.4.9])
        ;;
      *)
        AC_MSG_ERROR([Unsupported Linux arch: $build_cpu])
        ;;
esac
AC_MSG_RESULT([TCLKITBSRC is $TCLKITBSRC])

AC_OUTPUT(Makefile Scripts/Makefile BuildData/Makefile Docs/Makefile \
          Docs/Doxyfile BuildScripts/Makefile)
