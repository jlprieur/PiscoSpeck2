#!/bin/csh
#########################################################
#
#sed --expression="s/include \"mongo.h\"/include \"jlp_splot_def.h\"/g" $i > $i.0 
#sed --expression="s/include <fftw.h>/include "\""fftw.h"\""/g" $i > $i.0 
#########################################################
set list=`find . -name \*.cpp -print`

foreach i ($list)
sed --expression="s/fsp2/asp2/g" $i > $i.0 
mv "$i.0" $i
end
goto end1

end1:
