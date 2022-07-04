DOCSRCS = contents.h Help.h Tutorial.h copying.h 
EXTRA_DIST = Doxyfile.in removenavrow1.sed $(DOCSRCS) Version.h.in

Version.h : Version.h.in 
	$(top_builddir)/config.status --file=Version.h:Version.h.in

../latex/refman.tex ../html/index.html : Doxyfile $(DOCSRCS) Version.h
	-rm -rf ../latex
	-rm -rf ../html
	$(DOXYGEN) Doxyfile
	-rm -f ../html/files.html
	-rm -f ../html/pages.html
	-rm -f ../html/*8h_source.html
	for f in ../html/*.html; do \
	  echo "$$f => $$f-new" ;\
	  sed -f $(srcdir)/removenavrow1.sed <$$f >$$f-new ;\
	  mv $$f $$f-orig ;\
          mv $$f-new $$f ;\
        done
          
