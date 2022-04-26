      program hello
      implicit none
      include 'mpif.h'
      integer nproc, myid, ierr, nprime, ntest, nloc, nr, nd ,i
c---------------------------------------------------------- 
c        start MPI
c----------------------------------------------------------
      call MPI_INIT( ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, nproc, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )
      nprime = 118845 
      ntest = sqrt(real(nprime))+1
      nloc = (ntest+nproc-1)/nproc
      nd = 0
      do i = max(3,myid*nloc+1),min(ntest,(myid+1)*nloc)
         nr = mod(nprime,i)
         if (nr.eq.0) then
            nd = nd+1
            write(6,*)myid,' : ', i
         end if
      end do
      write (6,*)'task',myid,' found',nd,'divisors for', nprime
      call MPI_FINALIZE (ierr)
      
      stop
      end






















