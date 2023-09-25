 MPIopt    = -I/home/kongkong/Software/mpich-4.1.2/include

# MUMPSopt = -I/... (where to find files to be included by 
#                     applications using MUMPS)
#MUMPSopt = -I/home/kongkong/Software/MUMPS_5.6.1/include
#MUMPSopt =

# MUMPSlib  = -l...  (link reference for MUMPS)
#MUMPSlib = -L/home/kongkong/Software/MUMPS_5.6.1/lib -ldmumps -lmumps_common 
MUMPSlib = -L/home/kongkong/Software/MUMPS_5.6.1/lib -ldmumps -lmumps_common \
-L/home/kongkong/Software/parmetis-4.0.3/lib -lparmetis \
-L/home/kongkong/Software/metis-5.1.0/lib -lmetis \
-L/home/kongkong/Software/MUMPS_5.6.1/lib -lpord

#MUMPSlib =

# SCALAP    = -l...  (link reference for SCALAPACK, needed by MUMPS)
SCALAP = -L/usr/local/lib64 -lscalapack

# BLASLAPACK= -l...  (link reference for LAPACK & BLAS)
BLASLAPACK=-L/usr/local/lib64 -llapack -lblas

# MPIlib    = -l...  (link reference for MPI)
MPIlib = -L/home/kongkong/Software/mpich-4.1.2/lib -lmpi

# AGMG parallel library
AGMGLIB_PAR = -L/home/kongkong/LibraryObject/LIB_AGMG_PAR -lAGMG_PAR

# your FORTRAN compiler
F90=mpifort

# c/cxx compiler
C = mpicc

# compilation options
opt = -Ofast -fallow-argument-mismatch
opt_c = -g -Wall

# directory where AGMG sources are located
agmgdir  = ../SRC

incl=$(MPIopt) $(MUMPSopt)

lib = $(AGMGLIB_PAR) $(MUMPSlib) $(SCALAP) $(MPIlib) $(BLASLAPACK)
path_lib_c = -L/usr/local/lib64 \
-L/home/kongkong/Software/mpich-4.1.2/lib \
-L/home/kongkong/LibraryObject/LIB_AGMG_PAR \
-L/home/kongkong/Software/MUMPS_5.6.1/lib \
-L/home/kongkong/Software/parmetis-4.0.3/lib \
-L/home/kongkong/Software/metis-5.1.0/lib
flag_lib_c = -lAGMG_PAR -ldmumps -lmumps_common \
-lparmetis -lmetis -lpord \
-lscalapack -lmpi -llapack -lblas \
-lm -lmpifort -lgfortran

list=Example_par.o
#list_c = Example_par_c.o
list_c = test_example_par.o

Example_par: Example_par.o
	$(F90) $(opt) -o Example_par $(list) $(lib)

Example_par.o: Example_par.f90 
	$(F90) $(opt) $(incl) -c Example_par.f90

test_example_par: test_example_par.o
	$(C) $(opt_c) $(incl) -o test_example_par \
	$(list_c) $(path_lib_c) $(flag_lib_c)

test_example_par.o: test_example_par.c
	$(C) $(opt_c) -c test_example_par.c -o test_example_par.o

clean:
	rm -rf *.o Example_par Example_par_c test_example_par
