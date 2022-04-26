      program p2p_vector
      implicit none
!  FORTRAN MPI-INCLUDE-file
      include 'mpif.h'
      integer    ierr, nproc, myid,
     :           i, nsend, nrecv, startsend, startrecv
      real*8 vec(10)
!  declare status data structure:
      integer stat(MPI_STATUS_SIZE)

!  initialize MPI, get nproc (# of procs) and myid (local proc. id)
      call MPI_INIT( ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, nproc, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )

      do i = 1 , 10
         vec(i) = myid*i 
      end do
      write(6,111)myid,vec
 111  format(i3,'>> initial:    vec=',10f3.0)
           
      if (myid.eq.1) then
         startsend = 5; nsend = 3
         call MPI_SEND( vec(startsend), nsend, MPI_DOUBLE_PRECISION, 0, 
     :                  11, MPI_COMM_WORLD, ierr )
      end if
      
      if (myid.eq.0) then
         startrecv = 2; 
         call MPI_PROBE(MPI_ANY_SOURCE,MPI_ANY_TAG, 
     :                  MPI_COMM_WORLD, stat, ierr)
         call MPI_GET_COUNT(stat, MPI_DOUBLE_PRECISION, nrecv, ierr)
         write(6,*)'number of elements to be received : ',nrecv          
!  status argument "stat" instead of "MPI_STATUS_IGNORE"
         call MPI_RECV(vec(startrecv), nrecv, MPI_DOUBLE_PRECISION, 
     :                 MPI_ANY_SOURCE, MPI_ANY_TAG, 
     :                 MPI_COMM_WORLD, stat, ierr) 
         write(6,113)myid,vec
 113     format(i3,'>> after revc: vec=',10f3.0)
      end if

      call MPI_FINALIZE( ierr )

      stop
      end



 
