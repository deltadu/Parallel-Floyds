How to run the program

1. make
2. 
mpiexec -n <#processers> ./mpi_floyd < matrix_file

or

mpirun --mca btl vader,self,tcp -np <#processors>  --hostfile <hostfile> ./mpi_floyd < matrix_file

example:
mpiexec -n 4 ./mpi_floyd < matrix.txt 

mpirun --mca btl vader,self,tcp -np 8  --hostfile ./hosts ./mpi_floyd < matrix.txt 
