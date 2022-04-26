      program intsum
      implicit none
c  FORTRAN MPI-INCLUDE-file
      include 'mpif.h'
      integer    ierr, nproc, myid
      integer    n, i, l
      integer suml, sum

c  initialize MPI, get nproc (# of procs) and myid (local proc. id)
      call MPI_INIT( ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, nproc, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )
      call MPI_BARRIER( MPI_COMM_WORLD, ierr )

      if (myid.eq.0) then
         write(6,*)'n ?'
         read(5,*) n
      end if
      call MPI_BCAST(n,1,MPI_INTEGER,0,MPI_COMM_WORLD,ierr)

! distribute the summation of integers from 1 to n
! over nproc tasks
      suml = 0.
      do i = myid+1,n,nproc
         suml = suml +i
      end do
      call MPI_REDUCE(suml,sum,1,MPI_INTEGER, MPI_SUM,
     :                0,MPI_COMM_WORLD, ierr )

      if (myid.eq.0) then                
         write(6,*)  'sum of i from 1 to',n,' is: ',sum
         write(6,*)  'correct result is: ',n*(n+1)/2 
      end if


      call MPI_FINALIZE( ierr )

      stop
      end



 
