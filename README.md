# replace_partf

Simple code to replace partition function data in webnucleo nuclear data xml
file with that in a comma-separated file (csv).  To run, change into your
nucnet-tools-code directory and create a my_examples directory:

cd nucnet-tools-code
mkdir -p my_examples

Change into the directory and clone the repository:

cd my_examples
git clone https://github.com/mbradle/replace_partf.git

Change into the directory and make the code:

cd replace_partf
make replace_partf

Get the data:

make data
cp fe.csv_example fe.csv

Run the code (add optional source string such as " + new partf data"):

./replace_partf ../../data_pub/my_net.xml fe.csv out.xml " + new partf data"

The updated data are in out.xml.  To view, use libnucnet example codes
(see
https://sourceforge.net/u/mbradle/blog/2012/07/running-libnucnet-examples/):

cd ../../build
make -f Makefile.libnucnet all_libnucnet
cd ../my_examples/replace_partf

Print out the nuclides that changed:

../../libnucnet/print_nuclides out.xml "[source[contains(.,'new partf')]]"

Print out a partition function:

../../libnucnet/compute_partf_by_name out.xml fe46

You may now use out.xml as nuclear data xml input to NucNet Tools codes.

Note that you may clone replace_partf into another directory.  If you do, you
need to set the environment variable NUCNET_TARGET to point to the absolute
path of your installation of nucnet-tools-code.  For example,

export NUCNET_TARGET=/Users/bradleymeyer/nucnet-tools-code

Note that data and object files will be placed under the directory
pointed to by NUCNET_TARGET.
