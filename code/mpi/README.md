How to run the program

1. make
2. 
mpiexec -n <#processers> ./floyd < matrix_file

or

mpirun --mca btl vader,self,tcp -np <#processors>  --hostfile <hostfile> ./floyd < matrix_file

example:
mpiexec -n 4 ./floyd < matrix.txt 

mpirun --mca btl vader,self,tcp -np 8  --hostfile ./hosts ./floyd < matrix.txt 
